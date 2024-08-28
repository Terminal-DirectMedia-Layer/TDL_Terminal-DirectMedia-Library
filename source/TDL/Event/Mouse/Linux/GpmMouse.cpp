
#include <iostream>

#include <fcntl.h>
#include <string.h>
#include <gpm.h>

#include "TDL/Event/Mouse/Linux/GpmMouse.hpp"
#include "TDL/Event/Mouse/EventMouseData.hpp"
#include "TDL/Window/AWindow.hpp"

tdl::GpmMouse::GpmMouse() {
    conn.eventMask = ~0; // Listen to all events
    conn.defaultMask = 0;
    conn.minMod = 0;
    conn.maxMod = 0;
    if (Gpm_Open(&conn, 0) == -1) {
        std::cerr << "Failed to initialize GPM\n";
        return;
    }
}

bool tdl::GpmMouse::mouseMove(AWindow *window)
{
    if (Gpm_GetEvent(&ev) > 0) {
        if (ev.type & GPM_MOVE) {
            Event event;
            event.type = Event::EventType::MOUSEMOVED;
            event.mouseMove.x = ev.x * getCharWidth(window);
            event.mouseMove.y = ev.y * getCharHeight(window);
            window->pushEvent(event);
            return true;
        }
    }
    return false;
}

bool tdl::GpmMouse::mousePessed(AWindow *window)
{
    Event event;
    if (Gpm_GetEvent(&ev) > 0) {
        if (ev.type & GPM_DOWN) {
            event.type = Event::EventType::MOUSEBUTTONPRESSED;
            if (ev.buttons & GPM_B_LEFT) {
                event.mouseButton.button = MouseButton::LEFT;
            } else if (ev.buttons & GPM_B_MIDDLE) {
                event.mouseButton.button = MouseButton::MIDDLE;
            } else if (ev.buttons & GPM_B_RIGHT) {
                event.mouseButton.button = MouseButton::RIGHT;
            }
            event.mouseButton.x = ev.x * getCharWidth(window);
            event.mouseButton.y = ev.y * getCharHeight(window);
            window->pushEvent(event);
            return true;
        }
    }
    return false;
}

bool tdl::GpmMouse::mouseReleased(AWindow *window)
{
    Event event;
    if (Gpm_GetEvent(&ev) > 0) {
        if (ev.type & GPM_UP) {
            event.type = Event::EventType::MOUSEBUTTONRELEASED;
            if (ev.buttons & GPM_B_LEFT) {
                event.mouseButton.button = MouseButton::LEFT;
            } else if (ev.buttons & GPM_B_MIDDLE) {
                event.mouseButton.button = MouseButton::MIDDLE;
            } else if (ev.buttons & GPM_B_RIGHT) {
                event.mouseButton.button = MouseButton::RIGHT;
            }
            event.mouseButton.x = ev.x * getCharWidth(window);
            event.mouseButton.y = ev.y * getCharHeight(window);
            window->pushEvent(event);
            return true;
        }
    }
    return false;
}

bool tdl::GpmMouse::mouseScroll(AWindow *window)
{
    Event event;
    if (Gpm_GetEvent(&ev) > 0) {
        if (ev.type & GPM_DOWN) {
            event.type = Event::EventType::MOUSESCROLLED;
            event.mouseScroll.x = ev.x * getCharWidth(window);
            event.mouseScroll.y = ev.y * getCharHeight(window);
            window->pushEvent(event);
            return true;
        }
    }
    return false;
}

int tdl::GpmMouse::getCharWidth(AWindow *window) {
    return window->getWidth() / window->getTerminalSize().x();
}

int tdl::GpmMouse::getCharHeight(AWindow *window) {
    return window->getHeight() / window->getTerminalSize().y();

}