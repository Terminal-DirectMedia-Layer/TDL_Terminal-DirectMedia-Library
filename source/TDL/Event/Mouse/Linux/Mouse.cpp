
#include <iostream>

#include <fcntl.h>
#include <string.h>

#include "TDL/Event/Mouse/Linux/Mouse.hpp"
#include "TDL/Event/Mouse/EventMouseData.hpp"
#include "TDL/Window/AWindow.hpp"

tdl::Mouse::Mouse() {}

bool tdl::Mouse::mouseMove(AWindow *window, EventMouseData data)
{
    if (data._button == 32 || data._button == 35) {
        Event event;
        event.type = TDL_MOUSEMOVED;
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
        event.type = TDL_MOUSEPRESSED;
        event.mouseButton.button = TDL_MOUSELEFT;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 1) {
        event.type = TDL_MOUSEPRESSED;
        event.mouseButton.button = TDL_MOUSEMIDDLE;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 2) {
        event.type = TDL_MOUSEPRESSED;
        event.mouseButton.button = TDL_MOUSERIGHT;
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
        event.type = TDL_MOUSERELEASED;
        event.mouseButton.button = TDL_MOUSELEFT;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 1) {
        event.type = TDL_MOUSERELEASED;
        event.mouseButton.button = TDL_MOUSEMIDDLE;
        event.mouseButton.x = data._x * 2;
        event.mouseButton.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 2) {
        event.type = TDL_MOUSERELEASED;
        event.mouseButton.button = TDL_MOUSERIGHT;
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
        event.type = TDL_MOUSEWHEEL;
        event.mouseScroll.direction = TDL_WHEELUP;
        event.mouseScroll.x = data._x * 2;
        event.mouseScroll.y = data._y * 3;
        window->pushEvent(event);
        return true;
    } else if (data._button == 65) {
        event.type = TDL_MOUSEWHEEL;
        event.mouseScroll.direction = TDL_WHEELUP;
        event.mouseScroll.x = data._x * 2;
        event.mouseScroll.y = data._y * 3;
        window->pushEvent(event);
        return true;
    }
    return false;
}