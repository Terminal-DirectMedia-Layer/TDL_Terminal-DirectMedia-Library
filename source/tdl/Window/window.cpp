#include "tdl/Window.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <utility>
#include <vector>
#include <fcntl.h>
#include <termios.h>
#include <cstring>
#include "tdl/Signal/SignalHandler.hpp"
#include "tdl/Matrix/PixelMatrix.hpp"
#include "tdl/Event/Mouse/linux/mouse.hpp"
#include "tdl/Event/Mouse/EventMouseData.hpp"
#include "tdl/Event/Event.hpp"



tdl::Window::Window(std::string  title, std::string const& ttyPath) : _title(std::move(title)), _frameRate(60), Drawable() {
    struct winsize w{};
    int param;
    _fd = open(ttyPath.c_str(), O_RDWR);
    if (_fd == -1)
        throw std::runtime_error("Can't open tty");
    ioctl(_fd, TIOCGWINSZ, &w);
    if (w.ws_col == 0 || w.ws_row == 0)
        throw std::runtime_error("Can't get terminal size");
    param = ioctl(_fd, F_GETFL, 0);
    fcntl(_fd, F_SETFL, param | O_NONBLOCK );
    _size = Vector2u((w.ws_col + 1) * 2, ((w.ws_row + 1) * 3));
    getMatrix() = PixelMatrix(_size);
    getOldMatrix() = PixelMatrix(_size);
    SignalHandler::getInstance().registerWindow(this);
    start = std::chrono::system_clock::now();
    _input = InputKeyboard();
    _mouse = new tdl::mouse();
}
/**
 * @brief Destroy the tdl::Window::Window object and unregister the window from the signal manager
 * 
 */
tdl::Window::~Window()
{
    SignalHandler::getInstance().unRegisterWindow(this);
    close(_fd);
}

/**
 * @brief Create a new Window object, initialize the window and return it
 * 
 * @param title the title of the window
 * @param tty_path the path to the tty to lauch the window, the available tty are /dev/tty or /dev/pts/0.../dev/pts/x
 * @return tdl::Window* 
 * @note this is only herre where the window constructor is called because it throw an error if the tty can't be open
 * also we initialize the window here that means that the black background should be printed on the terminal
 */
tdl::Window* tdl::Window::CreateWindow(std::string const& title, std::string const& ttyPath) {
    try {
        auto * win = new Window(title, ttyPath);
        win->alternateScreenBuffer();
        win->disableEcho();
        win->removeMouseCursor();
        win->enableMouseClick();
        win->enableMouseMove();
        return win;
    } catch (std::runtime_error& e) {
        std::cerr << e.what();
        return nullptr;
    }
}

void tdl::Window::disableEcho()
{
    tcgetattr(_fd, &_tty);
    _tty.c_lflag &=(~ICANON & ~ECHO);
    _tty.c_cc[VMIN] = 1;
    _tty.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO,TCSANOW,&_tty);
}


void tdl::Window::draw()
{
    if (!_content.empty()) {
        //moveCursor(Vector2u(0, 0));
        write(_fd, _content.c_str(), _content.size());
        _content = "";
    }
}

void tdl::Window::printFrameRate() {
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    if (elapsed_seconds.count() >= 1) {
        std::cerr << "Frame rate : " << framecounter << std::endl;
        framecounter = 0;
        start = std::chrono::system_clock::now();
    }
    framecounter++;
}


/**
 * @brief Polls the event
 * this function will return the event and pop it from the queue
 * before that it will check if any event flag is set
 * 
 * @param event the event to return
 * @return true if an event is found
 * @return false if no event is found
 */
bool tdl::Window::pollEvent(tdl::Event &event, std::regex *custom)
{
    if (_events.empty()){
        int _nread = 0;
        int index = 0;

        std::regex e("\\d+;\\d+;\\d+[mM]");
        ioctl(getFd(), FIONREAD, &_nread);
        char buffer[_nread];
        read(getFd(), buffer, _nread);
        buffer[_nread] = 0;

        if (_nread == 0) {
            index =+ _input.readInputKeyboard(this, buffer, _nread);
            return false;
        }
        while (buffer[index] != 0 && _nread != index) {
            auto it = std::find_if(buffer + index, buffer + _nread, [](char c) {
                return c == 'm' || c == 'M';
            });
            std::string str(buffer + index, it + 1);
            if (std::regex_match(str, e)) {
                EventMouseData data(str);
                if (_mouse->mouseMove(this, data) || _mouse->mousePessed(this, data) || _mouse->mouseReleased(this, data) || _mouse->mouseScroll(this, data)) {
                    index += str.size();
                    continue;
                }
            }
            if (custom != nullptr && std::regex_match(buffer + index, *custom))
            {
                std::cerr << "custom event" << std::endl;
                Event ev;
                ev.type = Event::EventType::Custom;
                std::cerr << "buffer at index" << buffer + index << std::endl;
                int length = strlen(buffer + index);

                ev.custom.data = new char[length + 1];

                strcpy(ev.custom.data, buffer + index);
                _events.push(ev);
                index += _nread;
                continue;
            }
            index += _input.readInputKeyboard(this, buffer + index, _nread);
        }
    }
    if (!_events.empty()){
        event = _events.front();
        _events.pop();
        return true;
    }
    return false;
}

/**
 * @brief reset the pixel table of the window with black pixel
 * 
 */
void tdl::Window::clearPixel()
{
    if (getUpdate()) {
        getMatrix().clear();
        setUpdate(false);
    }
}

void tdl::Window::updateTermSize()
{
    struct winsize w{};
    int timeout = 10;
    w.ws_col = 0;
    w.ws_row = 0;
    while (w.ws_col == 0 && w.ws_row == 0 && timeout > 0) {
        ioctl(_fd, TIOCGWINSZ, &w);
        timeout--;
    }
    _size = Vector2u((w.ws_col + 1) * 2, ((w.ws_row + 1) * 3));
    getMatrix().resize(_size);
    getOldMatrix().resize(_size);
    update(true);
}

void tdl::Window::update(bool all) {
    CharColor charColor;
    Vector2u pos = Vector2u(0, 0);
    Vector2u oldPos = Vector2u(0, 0);
    Pixel pixels[6] = {Pixel(0, 0, 0, 0)};
    Pixel oldForeColor = Pixel(0, 0, 0, 0);
    Pixel oldBackColor = Pixel(0, 0, 0, 0);

    if (_update)
        _update = false;
    setUpdate(true);
    for (u_int32_t i = 0; i < _size.y(); i += 3) {
        for (u_int32_t j = 0; j < _size.x(); j += 2) {
            try {
                getMatrix().getPixelChar(Vector2u(j, i), pixels);
            } catch (std::out_of_range &e) {
                continue;
            }
            getOldMatrix().setPixelChar(Vector2u(j, i), pixels);
            charColor = getMatrix().computeCharColor(Vector2u(j, i), pixels);
            if (!charColor.shape)
                continue;
            if (pos.x() != oldPos.x() + 2) {
                moveCursor(pos);
            }
            if (charColor.ForeGround != oldForeColor) {
                setRGBFrontGround(charColor.ForeGround);
                oldForeColor = charColor.ForeGround;
            }
            if ((charColor.BackGround != oldBackColor)) {
                setRGBBackGround(charColor.BackGround);
                oldBackColor = charColor.BackGround;
            }
            printPixel(charColor.shape);
            oldPos = pos;
            pos.x() += 2;
        }
        pos.x() = 0;
        pos.y() += 2;
    }
}

void tdl::Window::setRGBFrontGround(Pixel pixel)
{
    _content += "\033[38;2;" + std::to_string(GET_R(pixel.color)) + ";" + std::to_string(GET_G(pixel.color)) + ";" + std::to_string(GET_B(pixel.color)) + "m";
}

void tdl::Window::setRGBBackGround(Pixel pixel)
{
    _content += "\033[48;2;" + std::to_string(GET_R(pixel.color)) + ";" + std::to_string(GET_G(pixel.color)) + ";" + std::to_string(GET_B(pixel.color)) + "m";
}

void tdl::Window::clearScreen()
{
    _content += "\033[2J";
}

void tdl::Window::moveCursor(Vector2u pos)
{
    _content += "\033[" + std::to_string(pos.y() / 2) + ";" + std::to_string(pos.x()) + "H";
}

void tdl::Window::printPixel(const char *shape)
{
    if (shape == nullptr)
        return;
    _content += std::string(reinterpret_cast<const char*>(shape));
}

void tdl::Window::alternateScreenBuffer()
{
    _content += "\033[?1049h";
}

void tdl::Window::removeMouseCursor()
{
    _content += "\033[?25l";
}

void tdl::Window::enableMouseMove()
{
    _content += "\033[?1003h";
    _content += "\033[?1006h";
}

void tdl::Window::disableMouseMove()
{
    _content += "\033[?1003l";
    _content += "\033[?1006l";
}

void tdl::Window::enableMouseClick()
{
    _content += "\033[?1000h";
}

void tdl::Window::disableMouseClick()
{
    _content += "\033[?1000l";
}