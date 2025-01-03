#ifndef INTERRUPT_MANAGER_HPP
#define INTERRUPT_MANAGER_HPP

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <map>
#include "TDL/Event/Event.hpp"

#include "TDL/Event/Input/Module/Keyboard.hpp"
#include "TDL/Event/Input/Module/Mouse.hpp"
#include "TDL/Utils/Thread/ThreadSafeQueue.hpp"

namespace tdl {
    class InterruptManager {
    public:
        using InterruptHandler = std::function<void(tdl::Event&)>;

        InterruptManager();
        ~InterruptManager();

        void addInterrupt(InterruptHandler handler, int type, int priority);
        void start();
        void stop();

    private:
        void processInterrupts();

        struct Interrupt {
            InterruptHandler handler;
            int priority;

            bool operator<(const Interrupt& other) const {
                return priority < other.priority;
            }
        };

        std::map<int, std::priority_queue<Interrupt>> _interrupts;
        std::vector<std::thread> _threads;
        std::mutex _mutex;
        std::condition_variable _cv;
        bool _running;

        std::thread _keyboardThread;
        ThreadSafeQueue<Event> _keyboardQueue {};
        Keyboard *_keyboard;

        std::thread _mouseThread;
        ThreadSafeQueue<Event> _mouseQueue;
        Mouse *_mouse;
    };
}

#endif // INTERRUPT_MANAGER_HPP