
#include <iostream> 
#include <queue>
#include <sys/ioctl.h>
#include <utility>
#include "tdl/Event/Event.hpp"
#include "tdl/Window/windowBase.hpp"
#include "tdl/Input/inputKeyboard.hpp"
#include "tdl/Input/inputMouse.hpp"
#include "tdl/Event/Mouse/Imouse.hpp"
#include "tdl/Event/Mouse/linux/mouse.hpp"
#include "tdl/Event/Mouse/EventMouseData.hpp"
#include <regex>
#include <functional>
#include <cstring>

tdl::WindowBase::WindowBase( std::string fdPath)
    : _fdPath(std::move(fdPath))
{
    _input = InputKeyboard();
    _mouse = new tdl::mouse();
}

tdl::WindowBase::~WindowBase()
{
    delete _mouse;
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
bool tdl::WindowBase::pollEvent(tdl::Event &event, std::regex *custom)
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
