
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

tdl::Window::Window(std::string  title, tdl::Vector2u size, tdl::Vector2i pos, Pixel background) : FrameBuffer(size, background)
{
    //_input = new Keyboard();
    setPosition(pos);
    _size = size;
    tdl::Display::getInstance().getFont().setSize(20);

    _title.setFont(tdl::Display::getInstance().getFont());
    _title.setText(title);
    _title.setColor(Pixel(100, 100, 100, 0));
    _background = background;
    setDimensions(RectI(pos.x() - TDL_WINDOW_BORDER_LEFT_B, pos.y() - TDL_WINDOW_BORDER_TOP_B, size.x() + TDL_WINDOW_BORDER_LEFT_B + TDL_WINDOW_BORDER_RIGHT_B, size.y() + TDL_WINDOW_BORDER_TOP_B + TDL_WINDOW_BORDER_BOTTOM_B));
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


float getLuminance(tdl::Pixel p)
{
    return 0.2126 * GET_R(p.color) + 0.7152 * GET_G(p.color) + 0.0722 * GET_B(p.color);
}

bool isEdge(tdl::FrameBuffer &buffer, u_int32_t x, u_int32_t y)
{
    tdl::Pixel p = buffer.getPixel(x, y);
    float luminance = getLuminance(p);
    float sum = 0;
    sum += std::abs(luminance - getLuminance(buffer.getPixel(x - 1, y)));
    sum += std::abs(luminance - getLuminance(buffer.getPixel(x + 1, y)));
    sum += std::abs(luminance - getLuminance(buffer.getPixel(x, y - 1)));
    sum += std::abs(luminance - getLuminance(buffer.getPixel(x, y + 1)));
    return sum > 0.5;
}

tdl::Pixel applyFxaa(tdl::FrameBuffer &buffer, u_int32_t x, u_int32_t y)
{
    tdl::Pixel p = buffer.getPixel(x, y);
    if (!isEdge(buffer, x, y)) {
        return p;
    }
    tdl::Pixel p1 = buffer.getPixel(x - 1, y);
    tdl::Pixel p2 = buffer.getPixel(x + 1, y);
    tdl::Pixel p3 = buffer.getPixel(x, y - 1);
    tdl::Pixel p4 = buffer.getPixel(x, y + 1);
    tdl::Pixel center = buffer.getPixel(x, y);

    tdl::Pixel sum(
            (GET_R(p1.color) + GET_R(p2.color) + GET_R(p3.color) + GET_R(p4.color) + GET_R(center.color)) / 5,
            (GET_G(p1.color) + GET_G(p2.color) + GET_G(p3.color) + GET_G(p4.color) + GET_G(center.color)) / 5,
            (GET_B(p1.color) + GET_B(p2.color) + GET_B(p3.color) + GET_B(p4.color) + GET_B(center.color)) / 5,
            255
    );
    return sum;
}


void tdl::Window::draw(tdl::Display &d)
{
    //plot config cache misses
    u_int32_t screenWidth = d.getSize().x();

    for (auto &drawable : _drawables) {
        drawable->draw(this);
    }

    setDimensions(RectI(getPosition().x() - TDL_WINDOW_BORDER_LEFT_B, getPosition().y() - TDL_WINDOW_BORDER_TOP_B, getSize().x() + TDL_WINDOW_BORDER_LEFT_B + TDL_WINDOW_BORDER_RIGHT_B, getSize().y() + TDL_WINDOW_BORDER_TOP_B + TDL_WINDOW_BORDER_BOTTOM_B));
    u_int32_t size_x = getSize().x();
    u_int32_t size_y = getSize().y();
    u_int32_t pos_win_x = getPosition().x();
    u_int32_t pos_win_y = getPosition().y();
    //fxaa(this);

    RectI decRect = getDimensions();
    Pixel *decData = decGetRawData();
    for (int y = decRect.y(); y < decRect.y() + decRect.height(); y++) {
        for (int x = decRect.x(); x < decRect.x() + decRect.width() ; x++) {
            if (x < 0 || y < 0 || x >= d.getSize().x() || y >= d.getSize().y()) {
                continue;
            }
            d.setPixel(x, y, decData[(y - decRect.y()) * decRect.width() + (x - decRect.x())]);
        }
    }
    for (u_int32_t y = 0; y < size_y && y < d.getSize().y(); y++) {
        if (y + pos_win_y < 0 || y + pos_win_y >= d.getSize().y()) {
            continue;
        }
        u_int32_t drawWidth = std::min(size_x, screenWidth - pos_win_x);
        memcpy(d.getRawData() + ((pos_win_y + y) * d.getSize().x() + pos_win_x) * sizeof(Pixel),
               getRawData() + (y * size_x) * sizeof(Pixel),
               drawWidth * sizeof(Pixel));
    }
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
