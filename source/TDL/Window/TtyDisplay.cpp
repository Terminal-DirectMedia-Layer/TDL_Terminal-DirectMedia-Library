
#include "TDL/Window/TtyDisplay.hpp"
#include <iostream>
#include <utility>
#include <vector>
#include <cstring>

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "TDL/Signal/SignalHandler.hpp"
#include "TDL/Matrix/PixelMatrix.hpp"
#include "TDL/Event/Mouse/Linux/GpmMouse.hpp"
#include "TDL/Event/Mouse/EventMouseData.hpp"
#include "TDL/Event/Event.hpp"
#include "TDL/Pixel/Pixel.hpp"

tdl::TtyDisplay* tdl::TtyDisplay::createTtyDisplay(std::string const& title, std::string const& ttyPath)
{
    try {
        auto * win = new TtyDisplay(title, ttyPath);
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

tdl::TtyDisplay::TtyDisplay(std::string title, std::string const& ttyPath)
{
    struct winsize w{};
    _title = std::move(title);
    _frameRate = 60;
    _fd = open(ttyPath.c_str(), O_RDWR);
    if (_fd == -1)
        throw std::runtime_error("Can't open tty");
    ioctl(_fd, TIOCGWINSZ, &w);
    if (w.ws_col == 0 || w.ws_row == 0)
        throw std::runtime_error("Can't get terminal size");
    int param = ioctl(_fd, F_GETFL, 0);
    fcntl(_fd, F_SETFL, param | O_NONBLOCK );
    SignalHandler::getInstance().registerWindow(this);
    start = std::chrono::system_clock::now();
    _input = InputKeyboard();
    _mouse = new tdl::GpmMouse();
    _terminalSize = Vector2u(w.ws_col, w.ws_row);
    _fbfd = open("/dev/fb0", O_RDWR);
    if (_fbfd == -1) {
        throw std::runtime_error("opening /dev/fb0");
        return;
    }

    if (ioctl(_fbfd, FBIOGET_FSCREENINFO, &_finfo)) {
        throw std::runtime_error("Error reading fixed information.");
        return;
    }

    if (ioctl(_fbfd, FBIOGET_VSCREENINFO, &_vinfo)) {
        throw std::runtime_error("Error reading variable information.");
        return;
    }
    _screensize = _vinfo.xres * _vinfo.yres * _vinfo.bits_per_pixel / 8;
    _fbp = (char *)mmap(0, _screensize, PROT_READ | PROT_WRITE, MAP_SHARED, _fbfd, 0);
    if ((intptr_t)_fbp == -1) {
        throw std::runtime_error("Error mapping framebuffer device to memory");
        return;
    }
    _size = Vector2u(_vinfo.xres, _vinfo.yres);
    getMatrix() = PixelMatrix(_size);
    getOldMatrix() = PixelMatrix(_size);
    std::cerr << "_size x : " << _size.x() << " y : " << _size.y() << std::endl;
    _blackScreen = (char *)malloc(_screensize);
    if (_blackScreen == NULL) {
        throw std::runtime_error("Error allocating memory");
        munmap(_fbp, _screensize);
        close(_fbfd);
        return;
    }
    memset(_blackScreen, 0, _screensize);
    _displayScreen = (char *)malloc(_screensize);
    if (_displayScreen == NULL) {
        throw std::runtime_error("Error allocating memory");
        munmap(_fbp, _screensize);
        close(_fbfd);
        return;
    }
    memcpy(_fbp, _blackScreen, _screensize);
}

tdl::TtyDisplay::~TtyDisplay()
{
    SignalHandler::getInstance().unRegisterWindow(this);
    delete _mouse;
    close(_fd);
    munmap(_fbp, _screensize);
    close(_fbfd);
}

void tdl::TtyDisplay::clearPixel()
{
        getMatrix().clear();
        memcpy(_displayScreen, _blackScreen, _screensize);
}

void tdl::TtyDisplay::draw()
{
   if( memcpy(_fbp, _displayScreen, _screensize) == NULL)
       throw std::runtime_error("Error copying the display screen to the framebuffer");
}

void tdl::TtyDisplay::update(bool all)
{
    Pixel* pixelData = getMatrix().getRawPixelData();
    memcpy(_displayScreen, pixelData, _screensize);
}

bool tdl::TtyDisplay::pollEvent(tdl::Event &event, std::regex *custom)
{
    if (_events.empty()){
        int _nread = 0;
        int index = 0;

        _mouse->mouseMove(this);
        _mouse->mousePessed(this);
        _mouse->mouseReleased(this);
        _mouse->mouseScroll(this);
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


            if (custom != nullptr && std::regex_match(buffer + index, *custom))
            {
                Event ev;
                ev.type = Event::EventType::CUSTOM;
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

void tdl::TtyDisplay::updateTermSize()
{
    struct winsize w{};
    int timeout = 10;
    w.ws_col = 0;
    w.ws_row = 0;
    while (w.ws_col == 0 && w.ws_row == 0 && timeout > 0) {
        ioctl(_fd, TIOCGWINSZ, &w);
        timeout--;
    }
    _size = Vector2u(w.ws_col, w.ws_row);
}