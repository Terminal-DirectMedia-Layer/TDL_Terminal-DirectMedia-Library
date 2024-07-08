#include "tdl/Window.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <utility>
#include <vector>
#include <fcntl.h>
#include <termios.h>
#include "tdl/Window/windowBase.hpp"
#include "tdl/Signal/SignalHandler.hpp"
#include "tdl/Matrix/PixelMatrix.hpp"

tdl::Window::Window(std::string  title, std::string const& ttyPath) :WindowBase(ttyPath), _title(std::move(title)), _frameRate(60), Drawable() {
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
