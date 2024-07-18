#include "tdl/Window/terminalDisplay.hpp"
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

namespace tdl {

    TerminalDisplay::TerminalDisplay(std::string const& title, std::string const& ttyPath, u_int32_t height) 
    {
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
        _size = Vector2u((w.ws_col + 1) * 2, (height * 3));
        getMatrix() = PixelMatrix(_size);
        getOldMatrix() = PixelMatrix(_size);
        SignalHandler::getInstance().registerWindow(this);
        start = std::chrono::system_clock::now();
        _input = InputKeyboard();
        _mouse = new tdl::mouse();
        _cursorPos = Vector2u(0, height);
        _subShell.openSubShell();
    }

    TerminalDisplay::~TerminalDisplay()
    {
        SignalHandler::getInstance().unRegisterWindow(this);
        _subShell.closeSubShell();
        delete _mouse;
        close(_fd);
    }

    TerminalDisplay* TerminalDisplay::CreateTerminalDisplay(std::string const& title, std::string const& ttyPath, u_int32_t height) {
        try {
            auto * win = new TerminalDisplay(title, ttyPath, height);
            win->alternateScreenBuffer();
            win->disableEcho();
            win->removeMouseCursor();
            win->enableMouseClick();
            win->enableMouseMove();
            return win;
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return nullptr;
        }
    }

    void TerminalDisplay::updateTermSize() {
        struct winsize w{};
        int timeout = 10;
        w.ws_col = 0;
        w.ws_row = 0;
        while (w.ws_col == 0 && w.ws_row == 0 && timeout > 0) {
            ioctl(_fd, TIOCGWINSZ, &w);
            timeout--;
        }
        _size.x() = (w.ws_col + 1) * 2;
        getMatrix().resize(_size);
        getOldMatrix().resize(_size);
        update(true);
    }

    void TerminalDisplay::printAtCursorPos(std::string const& str) {
        std::string s = "\033[" + std::to_string(_cursorPos.y()) + ";" + std::to_string(_cursorPos.x()) + "H" + "\033[0m" + str;
        write(_fd, s.c_str(), s.size());
    }

    bool TerminalDisplay::pollEvent(Event &event, std::regex *custom)
    {
       if (_events.empty()){
            int _nread = 0;
            int index = 0;

            std::regex e("\\x1b\\[<\\d+;\\d+;\\d+[mM]");
            ioctl(getFd(), FIONREAD, &_nread);
            char buffer[_nread + 1];
            int ret = read(getFd(), buffer, _nread);
            if (ret != -1) {
                
                buffer[_nread ] = 0;
                if (_nread == 0) {
                    index =+ _input.readInputKeyboard(this, buffer, _nread);
                } else {
                    while ( index <= _nread) {

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
                        _subShell.writeOnSubShell(buffer + index);

                        //index += _input.readInputKeyboard(this, buffer + index, _nread);

                        if (buffer[index] == '\n') {
                            _cursorPos.x() = 0;
                            _cursorPos.y() += 1;
                        }
                        if (buffer[index] == 127) {
                            if (_cursorPos.x() > 0) {
                                _cursorPos.x() -= 1;
                                printAtCursorPos("  "); // two space for remove the cursor and the char at the same time
                            }
                        }
                        if (std::isprint(buffer[index])) {
                            printAtCursorPos(buffer + index);
                            _cursorPos.x() += 1;
                        }
                        index++;
                    }
                    return false;
                }

            }
        }
        std::string res;
        res = _subShell.readOnSubShell();
        if (res.size() > 0) {
            printAtCursorPos(res);
            if (res.find('\n') != std::string::npos) {
                int count = std::count(res.begin(), res.end(), '\n');
                int rest = res.size() - res.find_last_of('\n') - 1;
                _cursorPos.y() += count;
                _cursorPos.x() = rest;
            } else {
                _cursorPos.x() = res.size();
            }
        }
        printAtCursorPos("\033[7m \033[0m"); // print the cursor

        if (!_events.empty()) {
            event = _events.front();
            _events.pop();
            return true;
        }
        return false;
    }
}
