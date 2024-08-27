
#include "TDL/Event/Mouse/linux/Mouse.hpp"
#include "TDL/Event/Mouse/EventMouseData.hpp"
#include <iostream>
#include <fcntl.h>
#include <string.h>
#include <gpm.h>
#include "TDL/Window/AWindow.hpp"

tdl::Mouse::Mouse()
{
}

bool tdl::Mouse::mouseMove(AWindow *window, EventMouseData data)
{
    if (data._button == 32 || data._button == 35) {
        Event event;
        event.type = Event::EventType::MOUSEMOVED;
        event.mouseMove.x = data._x * 2;
        event.mouseMove.y = data._y * 3;
        window->pushEvent(event);
        return true;
    }
    return false;
}

bool tdl::Mouse::mousePessed(AWindow *window, EventMouseData data)
{
    Event event;
    if (data._buffer.find("M") == std::string::npos) {
        return false;
    }
    if (data._button == 0) {
        event.type = Event::EventType::MOUSEBUTTONPRESSED;
        event.mouseButton.button = MouseButton::LEFT;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 1) {
        event.type = Event::EventType::MOUSEBUTTONPRESSED;
        event.mouseButton.button = MouseButton::MIDDLE;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 2) {
        event.type = Event::EventType::MOUSEBUTTONPRESSED;
        event.mouseButton.button = MouseButton::RIGHT;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    }
    return false;
}

bool tdl::Mouse::mouseReleased(AWindow *window, EventMouseData data)
{
    Event event;
    if (data._buffer.find("m") == std::string::npos) {
        return false;
    }
    if (data._button == 0) {
        event.type = Event::EventType::MOUSEBUTTONRELEASED;
        event.mouseButton.button = MouseButton::LEFT;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 1) {
        event.type = Event::EventType::MOUSEBUTTONRELEASED;
        event.mouseButton.button = MouseButton::MIDDLE;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 2) {
        event.type = Event::EventType::MOUSEBUTTONRELEASED;
        event.mouseButton.button = MouseButton::RIGHT;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    }
    return false;
}

bool tdl::Mouse::mouseScroll(AWindow *window, EventMouseData data)
{
    Event event;
    if (data._button == 64) {
        event.type = Event::EventType::MOUSESCROLLED;
        event.mouseScroll.direction = MouseButton::UP;
        event.mouseScroll.x = data._x * 2;
        event.mouseScroll.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 65) {
        event.type = Event::EventType::MOUSESCROLLED;
        event.mouseScroll.direction = MouseButton::DOWN;
        event.mouseScroll.x = data._x * 2;
        event.mouseScroll.y = data._y * 3;
        window->pushEvent(event);
        return true;
    }
    return false;
}