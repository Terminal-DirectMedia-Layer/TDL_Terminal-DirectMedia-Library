
#include "TDL/Event/Mouse/linux/mouse.hpp"
#include "TDL/Event/Mouse/EventMouseData.hpp"
#include <iostream>
#include <fcntl.h>
#include <string.h>
#include <gpm.h>
#include "TDL/Window/AWindow.hpp"

tdl::mouse::mouse()
{
}

bool tdl::mouse::mouseMove(AWindow *window, EventMouseData data)
{
    if (data._button == 32 || data._button == 35) {
        Event event;
        event.type = Event::EventType::MouseMoved;
        event.mouseMove.x = data._x * 2;
        event.mouseMove.y = data._y * 3;
        window->pushEvent(event);
        return true;
    }
    return false;
}

bool tdl::mouse::mousePessed(AWindow *window, EventMouseData data)
{
    Event event;
    if (data._buffer.find("M") == std::string::npos) {
        return false;
    }
    if (data._button == 0) {
        event.type = Event::EventType::MouseButtonPressed;
        event.mouseButton.button = MouseButton::LEFT;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 1) {
        event.type = Event::EventType::MouseButtonPressed;
        event.mouseButton.button = MouseButton::MIDDLE;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 2) {
        event.type = Event::EventType::MouseButtonPressed;
        event.mouseButton.button = MouseButton::RIGHT;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    }
    return false;
}

bool tdl::mouse::mouseReleased(AWindow *window, EventMouseData data)
{
    Event event;
    if (data._buffer.find("m") == std::string::npos) {
        return false;
    }
    if (data._button == 0) {
        event.type = Event::EventType::MouseButtonReleased;
        event.mouseButton.button = MouseButton::LEFT;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 1) {
        event.type = Event::EventType::MouseButtonReleased;
        event.mouseButton.button = MouseButton::MIDDLE;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 2) {
        event.type = Event::EventType::MouseButtonReleased;
        event.mouseButton.button = MouseButton::RIGHT;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    }
    return false;
}

bool tdl::mouse::mouseScroll(AWindow *window, EventMouseData data)
{
    Event event;
    if (data._button == 64) {
        event.type = Event::EventType::MouseScrolled;
        event.mouseScroll.direction = MouseButton::UP;
        event.mouseScroll.x = data._x * 2;
        event.mouseScroll.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 65) {
        event.type = Event::EventType::MouseScrolled;
        event.mouseScroll.direction = MouseButton::DOWN;
        event.mouseScroll.x = data._x * 2;
        event.mouseScroll.y = data._y * 3;
        window->pushEvent(event);
        return true;
    }
    return false;
}