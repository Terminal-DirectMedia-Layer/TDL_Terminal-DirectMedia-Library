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



tdl::Window::Window(std::string  title, std::string const& ttyPath) {
    struct winsize w{};
    int param;
    _title = std::move(title);
    _frameRate = 60;
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
    delete _mouse;
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

        std::regex e("\\x1b\\[<\\d+;\\d+;\\d+[mM]");

        ioctl(getFd(), FIONREAD, &_nread);
        char buffer[_nread + 1];
        int ret = read(getFd(), buffer, _nread);
        if (ret == -1) {
            return false;
        }
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
                Event ev;
                ev.type = Event::EventType::Custom;
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
