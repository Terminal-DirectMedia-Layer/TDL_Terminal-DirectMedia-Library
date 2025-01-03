
#include <iostream>
#include <regex>
#include <map>
#include <linux/input.h>
#include <TDL/Event/Input/Module/Mouse.hpp>
#include <TDL/Event/Event.hpp>
#include <TDL/Event/InputEventCode.hpp>
#include "TDL/Event/EventNotifier.hpp"

#include <libevdev-1.0/libevdev/libevdev.h>
#include <optional>

namespace tdl {

    Mouse::Mouse(ThreadSafeQueue<Event> &events) : InputEvent(MOUSE), _events(events)
    {
        _x = 0;
        _y = 0;
        _xrel = 0;
        _yrel = 0;
    }

    Mouse::~Mouse()
    {
    }

    void pollAbsEvent(Event &event) {

      }

    void Mouse::pollMouse()
    {
        while (_events.read) {
            std::optional<std::pair<struct libevdev *, std::unique_ptr<struct input_event>>> ret = std::nullopt;
            struct libevdev *dev = nullptr;
            struct input_event *ev = nullptr;
            Event moveEvent;
            Event clickEvent;
            Event wheelEvent;

            bool isTouchTracking = false;
            wheelEvent.type = TDL_NONE;
            clickEvent.type = TDL_NONE;
            moveEvent.type = TDL_NONE;
            do {
                ret = getInputEvent();
                if (!ret.has_value()) {
                    break;
                }
                dev = ret->first;
                ev = ret->second.get();
                switch(ev->type) {
                    case EV_REL:
                        if(ev->code == REL_X ) {
                            moveEvent.type = TDL_MOUSEMOVED;
                            _xrel = libevdev_get_event_value(dev, EV_REL, REL_X);
                            _x += _xrel;
                            if (_x < 0) _x = 0;
                            moveEvent.mouseMove.x = _x;
                            moveEvent.mouseMove.y = _y;
                        }
                        if(ev->code == REL_Y) {
                            moveEvent.type = TDL_MOUSEMOVED;
                            _yrel = libevdev_get_event_value(dev, EV_REL, REL_Y);
                            _y += _yrel;
                            if (_y < 0) _y = 0;
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
                                _x += libevdev_get_event_value(dev, EV_ABS, ABS_X) - _xTracking;
                                _xTracking = ev->value;
                                if (_x < 0) _x = 0;
                                moveEvent.mouseMove.x = _x;
                                moveEvent.mouseMove.y = _y;
                            } else {
                                _xTracking = ev->value;
                            }
                        }
                        if(ev->code == ABS_Y) {
                            if (!isTouchTracking) {
                                moveEvent.type = TDL_MOUSEMOVED;
                                _y += libevdev_get_event_value(dev, EV_ABS, ABS_Y) - _yTracking;
                                _yTracking = ev->value;
                                if (_y < 0) _y = 0;
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
                               _events.push(moveEvent);
                           if (clickEvent.type != TDL_NONE)
                               _events.push(clickEvent);
                           if (wheelEvent.type != TDL_NONE)
                               _events.push(wheelEvent);
                       }
                       break;
                   default:
                       break;
                }
            } while (ev->type != EV_SYN);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}