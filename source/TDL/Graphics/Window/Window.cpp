
#include <iostream>
#include <utility>
#include <vector>
#include <cstring>

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <thread>

#include "TDL/Graphics/Display/Display.hpp"
#include "TDL/Graphics/Window/Window.hpp"
#include "TDL/Utils/Signal/SignalHandler.hpp"
#include "TDL/Event/Event.hpp"
#include "Tracy.hpp"

tdl::Window::Window(std::string  title, tdl::Vector2u size, tdl::Vector2u pos, Pixel background) : FrameBuffer(size, background),
_win(size + Vector2u(TDL_X_WINDOW_OFFSET, TDL_Y_WINDOW_OFFSET), tdl::Pixel(255, 255, 255, 255))
{
    //_input = new Keyboard();
    setPosition(pos);
    _size = size;
    _winPos.setPosition(Vector2u(pos.x() + TDL_WINDOW_BORDER_LEFT_B, pos.y() + TDL_WINDOW_BORDER_TOP_B));
    _title = title;
    _background = background;
}
/**
 * @brief Destroy the tdl::Window::Window object and unregister the window from the signal manager
 * 
 */
tdl::Window::~Window()
{
    //SignalHandler::getInstance().unRegisterWindow(this);
    //delete _input;
}

void tdl::Window::draw(tdl::Display &d)
{
    //plot config cache misses

    for (auto &drawable : _drawables) {
        drawable->draw(this);
    }

    _winPos.setPosition(Vector2u(getPosition().x() - TDL_WINDOW_BORDER_LEFT_B, getPosition().y() - TDL_WINDOW_BORDER_TOP_B));
    Transform winT = _winPos.getTransform();
    Transform t = getTransform();
    u_int32_t size_x = _win.getSize().x();
    u_int32_t size_y = _win.getSize().y();
    Pixel p;
    Pixel white(255, 255, 255, 255);
    for (u_int32_t y = 0; y < size_y; y++) {
        for (u_int32_t x = 0; x < size_x; x++) {

            if (x > TDL_WINDOW_BORDER_LEFT_B
                && x < size_x - TDL_WINDOW_BORDER_RIGHT_B
                && y > TDL_WINDOW_BORDER_TOP_B
                && y < size_y - TDL_WINDOW_BORDER_BOTTOM_B)
            {
                d.setPixel(
                    t.transformPoint(x - TDL_WINDOW_BORDER_LEFT_B, y - TDL_WINDOW_BORDER_TOP_B),
                    getPixel(x - TDL_WINDOW_BORDER_LEFT_B, y - TDL_WINDOW_BORDER_TOP_B)
                );
                continue;
            }
            d.setPixel(
                winT.transformPoint(x, y),
            _win.getPixel(x, y)
            );
        }
    }

/*
    auto drawChunk = [&](u_int32_t startY, u_int32_t endY) {
        Vector2f pos;
        for (u_int32_t y = startY; y < endY; y++) {
            for (u_int32_t x = 0; x < size_x; x++) {
                if (x > TDL_WINDOW_BORDER_LEFT_B
                        && x < size_x - TDL_WINDOW_BORDER_RIGHT_B
                        && y > TDL_WINDOW_BORDER_TOP_B
                        && y < size_y - TDL_WINDOW_BORDER_BOTTOM_B)
                {
                    pos = t.transformPoint(x - TDL_WINDOW_BORDER_LEFT_B, y - TDL_WINDOW_BORDER_TOP_B);
                    d.setPixel(static_cast<u_int32_t>(pos.x()), static_cast<u_int32_t>(pos.y()), getPixel(x - TDL_WINDOW_BORDER_LEFT_B, y - TDL_WINDOW_BORDER_TOP_B));
                    continue;
                }
                pos = winT.transformPoint(x, y);
                d.setPixel(static_cast<u_int32_t>(pos.x()), static_cast<u_int32_t>(pos.y()), _win.getPixel(x, y));
            }
        }
    };

    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    u_int32_t chunkSize = size_y / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        u_int32_t startY = i * chunkSize;
        u_int32_t endY = (i == numThreads - 1) ? size_y : startY + chunkSize;
        threads.emplace_back(drawChunk, startY, endY);
    }

    for (auto &t : threads) {
        t.join();
    }
    */
}

void tdl::Window::pushEvent(const tdl::Event &event)
{
    _events.push(event);
}

void tdl::Window::pollEvent() {
    while (!_events.empty()) {
        Event ev = _events.front();
        if (onEvent != nullptr) {
            onEvent(ev, this);
        }
        _events.pop();
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
/*
bool tdl::Window::pollEvent(tdl::Event &event, std::regex *custom)
{
    if (_events.empty()){
        int _nread = 0;
        int index = 0;
        _input->pollKeyboard(this);
        std::regex e("\\x1b\\[<\\d+;\\d+;\\d+[mM]");

        ioctl(getFd(), FIONREAD, &_nread);
        char buffer[_nread + 1];
        int ret = read(getFd(), buffer, _nread);
        if (ret == -1) {
            return false;
        }
        buffer[_nread] = 0;
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
                ev.type = Event::EventType::CUSTOM;
                int length = strlen(buffer + index);

                ev.custom.data = new char[length + 1];

                strcpy(ev.custom.data, buffer + index);
                _events.push(ev);
                index += _nread;
                continue;
            }
        }
    }
    if (!_events.empty()){
        event = _events.front();
        _events.pop();
        return true;
    }
    return false;
}
*/
