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
        //Todo : delete this function
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