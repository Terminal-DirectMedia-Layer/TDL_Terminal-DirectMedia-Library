
#ifndef EVENTNOTIFIER_HPP
    #define EVENTNOTIFIER_HPP

#include "TDL/Event/Event.hpp"
#include "TDL/Graphics/Window/Window.hpp"
#include "TDL/Event/Input/Module/Keyboard.hpp"
#include "TDL/Event/Input/Module/Mouse.hpp"
#include "TDL/Utils/Thread/ThreadSafeQueue.hpp"
#include <atomic>

#include <thread>

namespace tdl {
    class EventNotifier {
    public:

        EventNotifier();
        ~EventNotifier();

        void changeFocus(Event &event, std::vector<Window *> &windows);
        bool notify(std::vector<Window *> &windows, Event &event);
        void pushEvent(const Event &event);

        void joinKeyboardThread() {
            _keyboardQueue.read = false;

            _keyboardThread.join();
        }
        void joinMouseThread() {
            _mouseQueue.read = false;
            _mouseThread.join();
        }



    private:
        std::thread _keyboardThread;
        ThreadSafeQueue<Event> _keyboardQueue {};
        Keyboard *_keyboard;

        std::thread _mouseThread;
        ThreadSafeQueue<Event> _mouseQueue;
        Mouse *_mouse;

        std::atomic<bool> _open = true;

    };
}

#endif //EVENTNOTIFIER_HPP
