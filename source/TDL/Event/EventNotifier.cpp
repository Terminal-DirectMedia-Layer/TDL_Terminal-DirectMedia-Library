#include "TDL/Event/EventNotifier.hpp"

namespace tdl {
    EventNotifier::EventNotifier()
        :
        _keyboardThread([this]() {
         _keyboard = new Keyboard(_keyboardQueue);
         _keyboard->pollKeyboard();
       }),
      _mouseThread([this]() {
          _mouse = new Mouse(_mouseQueue);
          _mouse->pollMouse();
      })
    {
        _mouse = nullptr;
        _keyboard = nullptr;
    }

    EventNotifier::~EventNotifier()
    {
        _keyboardQueue.read = false;
        if (_keyboardThread.joinable()) {
            _keyboardThread.join();
        }
        delete _keyboard;

        _mouseQueue.read = false;
        if (_mouseThread.joinable()) {
            _mouseThread.join();
        }
        delete _mouse;
    }

    void EventNotifier::changeFocus(Event &event, std::vector<Window *> &windows)
    {
        if (event.type == TDL_KEYPRESSED && event.key == TDL_KEY_Q) {
            Window *new_focus = windows.front();
            windows.erase(windows.begin());
            windows.push_back(new_focus);
        }
        if (event.type == TDL_MOUSEPRESSED && !windows.back()->isClickIn(Vector2u(event.mouseButton.x, event.mouseButton.y))) {
            for (auto it = windows.rbegin(); it != windows.rend(); ++it) {
                Window *win = *it;
                if (win->isClickIn(Vector2u(event.mouseButton.x, event.mouseButton.y)) != OUTSIDE) {
                    Window *new_focus = win;
                    windows.erase(std::find(windows.begin(), windows.end(), win));
                    windows.push_back(new_focus);
                    break;
                }
            }
        }
    }

    bool EventNotifier::notify(std::vector<Window *> &windows, Event &event)
    {
        if (_keyboardQueue.poll(event)) {
            changeFocus(event, windows);
            windows.back()->pushEvent(event);
            return true;
        }
        if (_mouseQueue.poll(event)) {
            changeFocus(event, windows);
            windows.back()->pushEvent(event);
            return true;
        }
        return false;
    }

    void EventNotifier::pushEvent(const Event &event)
    {
        _keyboardQueue.push(event);
        _mouseQueue.push(event);
    }
}