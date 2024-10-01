
#include <iostream>
#include <regex>
#include <map>
#include <linux/input.h>
#include <TDL/Input/Module/Mouse.hpp>
#include <TDL/Window/AWindow.hpp>
#include <TDL/Event/Event.hpp>
#include <TDL/Event/InputEventCode.hpp>

namespace tdl {

    TTYMouse::TTYMouse() : InputEvent(MOUSE)
    {
        _x = 0;
        _y = 0;
        _xrel = 0;
        _yrel = 0;
    }

    TTYMouse::~TTYMouse()
    {
    }

    void pollAbsEvent(Event &event) {

      }

    void TTYMouse::pollMouse(AWindow* window)
    {
        std::unique_ptr<struct input_event> ev = nullptr;
        Event moveEvent;
        Event clickEvent;
        Event wheelEvent;
        wheelEvent.type = TDL_NONE;
        clickEvent.type = TDL_NONE;
        moveEvent.type = TDL_NONE;
		bool isTouchTracking = false;
        do {
            ev = getInputEvent();
            if (ev.get() == nullptr) {
                return;
            }
            switch(ev->type) {
                case EV_REL:
                    if(ev->code == REL_X ) {
                        moveEvent.type = TDL_MOUSEMOVED;
                        _xrel = ev->value;
                        _x += _xrel;
                        moveEvent.mouseMove.x = _x;
                        moveEvent.mouseMove.y = _y;
                    }
                    if(ev->code == REL_Y) {
                        moveEvent.type = TDL_MOUSEMOVED;
                        _yrel = ev->value;
                        _y += _yrel;
                        moveEvent.mouseMove.x = _x;
                        moveEvent.mouseMove.y = _y;
                    }
                    break;
                case EV_ABS:
                    if (ev->code == ABS_MT_TRACKING_ID) {
           		        if (ev->value == -1) {
                            isTouchTracking = false;
                        } else {
                            isTouchTracking = true;
                        }
                    }
                    if(ev->code == ABS_X) {
                        if (!isTouchTracking) {
                        moveEvent.type = TDL_MOUSEMOVED;
                        _x += ev->value - _xTracking;
                        _xTracking = ev->value;
                        moveEvent.mouseMove.x = _x;
                        moveEvent.mouseMove.y = _y;
                        } else {
                        _xTracking = ev->value;
                        }
                    }
                    if(ev->code == ABS_Y) {
                        if (!isTouchTracking) {
                        moveEvent.type = TDL_MOUSEMOVED;
                        _y += ev->value - _yTracking;
                        _yTracking = ev->value;
                        moveEvent.mouseMove.x = _x;
                        moveEvent.mouseMove.y = _y;
                        } else {
                            _yTracking = ev->value;
                        }
                    }
                    break;
               case EV_KEY:
                    if(ev->code == BTN_LEFT) {
                        clickEvent.mouseButton.x = _x;
                        clickEvent.mouseButton.y = _y;
                        clickEvent.mouseButton.button = TDL_MOUSELEFT;
                        if(ev->value == 0) {
                            clickEvent.type = TDL_MOUSERELEASED;
                        } else {
                            clickEvent.type = TDL_MOUSEPRESSED;
                        }
                    }
                    if(ev->code == BTN_RIGHT) {
                        clickEvent.mouseButton.x = _x;
                        clickEvent.mouseButton.y = _y;
                        clickEvent.mouseButton.button = TDL_MOUSERIGHT;
                        if(ev->value == 0) {
                            clickEvent.type = TDL_MOUSERELEASED;
                        } else {
                            clickEvent.type = TDL_MOUSEPRESSED;
                        }
                    }
                    if(ev->code == BTN_MIDDLE) {
                        clickEvent.mouseButton.x = _x;
                        clickEvent.mouseButton.y = _y;
                        clickEvent.mouseButton.button = TDL_MOUSEMIDDLE;
                        if(ev->value == 0) {
                            clickEvent.type = TDL_MOUSERELEASED;
                        } else {
                            clickEvent.type = TDL_MOUSEPRESSED;
                        }
                    }
                    break;
                case EV_SYN:
                    if(ev->code == SYN_REPORT) {
                        if (moveEvent.type != TDL_NONE)
                            window->pushEvent(moveEvent);
                        if (clickEvent.type != TDL_NONE)
                            window->pushEvent(clickEvent);
                        if (wheelEvent.type != TDL_NONE)
                            window->pushEvent(wheelEvent);
                        return;
                    }
                    break;
                default:
                    break;
            }
        } while (ev->type != EV_SYN);
    }
}