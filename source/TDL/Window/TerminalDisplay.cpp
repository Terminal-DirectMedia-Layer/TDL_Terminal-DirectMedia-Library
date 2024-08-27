
#include <iostream>
#include <utility>
#include <vector>
#include <cstring>

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <pwd.h>
#include <limits.h>

#include "TDL/Signal/SignalHandler.hpp"
#include "TDL/Matrix/PixelMatrix.hpp"
#include "TDL/Event/Mouse/Linux/Mouse.hpp"
#include "TDL/Event/Mouse/EventMouseData.hpp"
#include "TDL/Event/Event.hpp"
#include "TDL/Window/TerminalDisplay.hpp"

namespace tdl {

    TerminalDisplay::TerminalDisplay(std::string const& title, std::string const& ttyPath, u_int32_t height, std::string const& shellPath)
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
        _mouse = new tdl::Mouse();
        _cursorPos = Vector2u(0, height);
        _subShell.openSubShell(shellPath);
    }

    TerminalDisplay::~TerminalDisplay()
    {
        SignalHandler::getInstance().unRegisterWindow(this);
        _subShell.closeSubShell();
        delete _mouse;
        close(_fd);
    }

    TerminalDisplay* TerminalDisplay::createTerminalDisplay(std::string const& title, std::string const& ttyPath, u_int32_t height, std::string const& shellPath) {
        try {
            auto * win = new TerminalDisplay(title, ttyPath, height, shellPath);
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
        if (_cursorPos.y() < _size.y() / 3 - 1 ) {
            _cursorPos.y() = _size.y() / 3;
            _subShell.writeOnSubShell("\n");
        }
        update(true);
    }

    void TerminalDisplay::printAtCursorPos(std::string const& str) {
        std::string s = "\033[" + std::to_string(_cursorPos.y()) + ";" + std::to_string(_cursorPos.x()) + "H" + "\033[0m" + str;
        write(_fd, s.c_str(), s.size());
    }

    void TerminalDisplay::removeCurrentLine() {

        std::string moveCursor = "\033[2K";
        write(_fd, moveCursor.c_str(), moveCursor.size());


        std::string moveCursorStart = "\033[G";
        write(_fd, moveCursorStart.c_str(), moveCursorStart.size());
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
                    index += _input.readInputKeyboard(this, buffer, _nread);
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
                            ev.type = Event::EventType::CUSTOM;
                            int length = strlen(buffer + index);

                            ev.custom.data = new char[length + 1];

                            strcpy(ev.custom.data, buffer + index);
                            _events.push(ev);
                            index += _nread;
                            continue;
                        }

                        if (buffer[index] == '\n') {
                            printAtCursorPos(" ");
                            _cursorPos.x() = 0;
                            _cursorPos.y() += 1;
                            if (_commandMap.find(_cmd.substr(0, _cmd.find(' '))) != _commandMap.end()) {
                                std::vector<std::string> args;
                                std::string arg;
                                for (char c : _cmd) {
                                    if (c == ' ') {
                                        args.push_back(arg);
                                        arg = "";
                                    } else {
                                        arg += c;
                                    }
                                }
                                args.push_back(arg);

                                char **argv = new char*[args.size() + 1];
                                for (int i = 0; i < args.size(); i++) {
                                    argv[i] = new char[args[i].size() + 1];
                                    strcpy(argv[i], args[i].c_str());
                                }
                                argv[args.size()] = nullptr;
                                _commandMap[argv[0]](this, args.size(), argv);
                                for (int i = 0; i < args.size(); i++) {
                                    delete[] argv[i];
                                }
                                delete[] argv;
                                _subShell.writeOnSubShell("\n");
                            } else {
                                _subShell.writeOnSubShell(_cmd + "\n");
                                _subShell.updateActivePath(_cmd);
                            }
                            _cmd = "";
                        }
                        if (buffer[index] == 127) {
                            if (_cursorPos.x() > 0 && _cmd.size() > 0) {
                                _cursorPos.x() -= 1;
                                printAtCursorPos("  "); // two space for remove the cursor and the char at the same time
                                _cmd.pop_back();
                            }
                        }
                        if (std::isprint(buffer[index])) {
                            printAtCursorPos(buffer + index);
                            _cmd += buffer[index];
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
            std::pair<int, int> pos = getCursorPosition();
            if (res.find('\n') != std::string::npos) {
                int count = std::count(res.begin(), res.end(), '\n');
                _cursorPos.y() += count;
                _cursorPos.x() = pos.second;
            } else {
                _cursorPos.x() = pos.second;
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

    std::pair<int, int> TerminalDisplay::getCursorPosition() {

        write(_fd, "\033[6n", 4);
        char buf[32];
        int ret = read(STDIN_FILENO, buf, sizeof(buf) - 1);
        buf[ret] = '\0';

        int rows, cols;
        if (sscanf(buf, "\033[%d;%dR", &rows, &cols) != 2) {
            return {-1, -1}; // Error
        }
        return {rows, cols};
    }

    void TerminalDisplay::registerCommand(std::string const& cmd, CommandFunction func) {
        _commandMap[cmd] = func;
    }

    void TerminalDisplay::unregisterCommand(std::string const& cmd) {
        _commandMap.erase(cmd);
    }
}
