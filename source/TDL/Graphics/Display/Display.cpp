#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <regex>
#include <unistd.h>
#include <termios.h>
#include <string>
#include <fcntl.h>

#include "TDL/Graphics/Display/Display.hpp"
#include "TDL/Graphics/Display/Strategy/AsciiMethode.hpp"
#include "TDL/Graphics/Display/Strategy/TtyMethode.hpp"
#include "TDL/Graphics/Display/Strategy/SixelMethode.hpp"
#include "TDL/Graphics/Widget/Widget.hpp"


namespace tdl
{
    Display::Display(DisplayType type) : FrameBuffer() , _drawMethode(nullptr), _cursor("cursor","assets/Default/Cursor.json")
    {
        if (type != DisplayType::AUTO) {
              _type = type;
              switch (type) {
                  case DisplayType::TTY:
                      _drawMethode = new TtyMethode(*this);
                      break;
                  case DisplayType::SIXEL:
                      _drawMethode = new SixelMethode(*this);
                      break;
                  case DisplayType::ASCII:
                      _drawMethode = new AsciiMethode(*this);
                      break;
                  default:
                      break;
              }
              return;
        }
        if (isGraphical()) {
            if (supportsSixel()) {
                _type = DisplayType::SIXEL;
                _drawMethode = new SixelMethode(*this);
            } else {
                _type = DisplayType::ASCII;
                _drawMethode = new AsciiMethode(*this);
            }
        } else {
            _type = DisplayType::TTY;
            _drawMethode = new TtyMethode(*this);
        }
    }

    void Display::setDrawMethode(IDrawMethode *drawMethode)
    {
        _drawMethode = drawMethode;
    }

    IDrawMethode *Display::getDrawMethode()
    {
        return _drawMethode;
    }

    void Display::draw()
    {
        for (auto &win : _windows) {
            win->draw(*this);
        }
        _cursor.draw(*this);

        _drawMethode->draw(*this);
    }

    bool Display::isGraphical()
    {
        const char* display = std::getenv("DISPLAY");
        return display != nullptr && std::regex_search(display, std::regex(":[0-9]+"));
    }

    bool Display::supportsSixel() {
        const char* escape = "\x1b";
        std::string query = std::string(escape) + "[c";
        write(STDOUT_FILENO, query.c_str(), query.size());

        termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        char response[1024];
        ssize_t count = read(STDIN_FILENO, response, sizeof(response) - 1);

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        if (count > 0) {
            response[count] = '\0';
            std::string response_str(response);
            std::cout << response_str << std::endl;
            // Check if the response contains Sixel support
            return response_str.find(";4;") != std::string::npos ||
           response_str.find("?4;") != std::string::npos ||
           response_str.find(";4c") != std::string::npos ||
           response_str.find("?4") != std::string::npos;
        }
        return false;
    }

    void updateMousePosition(Vector2f &currentPosition, Vector2f newMousePosition, float filterStrength) {
        currentPosition.x() = currentPosition.x() * (1.0 - filterStrength) + newMousePosition.x() * filterStrength;
        currentPosition.y() = currentPosition.y() * (1.0 - filterStrength) + newMousePosition.y() * filterStrength;
    }

    void Display::pollEvent()
    {
        Event event;
        while (_eventNotifier.notify(_windows, event)) {
            if (event.type == TDL_MOUSEMOVED) {
                if (_windows.back()->grab) {
                    _windows.back()->setPosition(Vector2u(event.mouseMove.x + clickDelta.x(), event.mouseMove.y + clickDelta.y()));
                }
                _cursor.setPosition(Vector2u(event.mouseMove.x, event.mouseMove.y));
            }
            if (event.type == TDL_MOUSEPRESSED) {
                if (_windows.back()->isClickIn(Vector2u(event.mouseButton.x, event.mouseButton.y)) == BORDER) {
                    clickDelta = Vector2i(_windows.back()->getPosition().x() - event.mouseButton.x, _windows.back()->getPosition().y() - event.mouseButton.y);
                    _windows.back()->grab = true;
                }
            }
            if (event.type == TDL_MOUSERELEASED) {
                _windows.back()->grab = false;
                clickDelta = Vector2i(0, 0);
            }
            if (onEvent != nullptr)
                onEvent(event);
        }
        for (auto &win : _windows) {
            win->pollEvent();
        }
    }
} // namespace tdl