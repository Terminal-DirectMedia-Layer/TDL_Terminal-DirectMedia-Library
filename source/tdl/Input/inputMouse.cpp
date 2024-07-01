#include "tdl/Input/inputMouse.hpp"
#include <unistd.h>
#include <sys/ioctl.h>
#include "tdl/Window/windowBase.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

tdl::inputMouse::inputMouse()
{

}

tdl::inputMouse::~inputMouse() = default;

int tdl::inputMouse::readInputMouse(WindowBase *win, std::string buffer, int _nread)
{
    int consumed = buffer.size();
    std::vector<int> numbers;
    std::stringstream ss(std::string(buffer.begin(), buffer.end()- 1));

    std::string temp;
    while (std::getline(ss, temp, ';')) {
        if (!temp.empty() && std::all_of(temp.begin(), temp.end(), ::isdigit)) {
            numbers.push_back(std::stoi(temp));
        }
    }
    Event event;
    switch (numbers[0])
    {
    case 32:
    case 35:
        event.type = Event::EventType::MouseMoved;
        event.mouseMove.x = numbers[1] * 2;
        event.mouseMove.y = numbers[2] * 3;
        win->pushEvent(event);
        break;
    case 0:
        if (buffer.find("M") != std::string::npos) {
            event.type = Event::EventType::MouseButtonPressed;
        } else {
            event.type = Event::EventType::MouseButtonReleased;
        }
        event.mouseButton.button = MouseButton::LEFT;
        event.mouseButton.x = numbers[1] * 2;
        event.mouseButton.y = numbers[2] * 3;
        win->pushEvent(event);
        break;
    case 1:
        if (buffer.find("M") != std::string::npos) {
            event.type = Event::EventType::MouseButtonPressed;
        } else {
            event.type = Event::EventType::MouseButtonReleased;
        }
        event.mouseButton.button = MouseButton::MIDDLE;
        event.mouseButton.x = numbers[1]* 2;
        event.mouseButton.y = numbers[2] * 3;
        win->pushEvent(event);
        break;
    case 2:
        if (buffer.find("M") != std::string::npos) {
            event.type = Event::EventType::MouseButtonPressed;
        } else {
            event.type = Event::EventType::MouseButtonReleased;
        }
        event.mouseButton.button = MouseButton::RIGHT;
        event.mouseButton.x = numbers[1] * 2;
        event.mouseButton.y = numbers[2] * 3;
        win->pushEvent(event);
        break;
    case 64:
        event.type = Event::EventType::MouseScrolled;
        event.mouseScroll.direction = MouseButton::UP;
        event.mouseScroll.x = numbers[1] * 2;
        event.mouseScroll.y = numbers[2] * 3;
        win->pushEvent(event);
        break;
    case 65:
        event.type = Event::EventType::MouseScrolled;
        event.mouseScroll.direction = MouseButton::DOWN;
        event.mouseScroll.x = numbers[1] * 2;
        event.mouseScroll.y = numbers[2] * 3;
        win->pushEvent(event);
        break;
    default:
        break;
    }
    return consumed;
}