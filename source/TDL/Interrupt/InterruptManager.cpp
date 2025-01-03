#include "TDL/Interrupt/InterruptManager.hpp"

namespace tdl {
    InterruptManager::InterruptManager() : _running(false),
        _keyboardThread([this]() {
            _keyboard = new Keyboard(_keyboardQueue);
            _keyboard->pollKeyboard();
       }),
        _mouseThread([this]() {
            _mouse = new Mouse(_mouseQueue);
            _mouse->pollMouse();
        }) {}

    InterruptManager::~InterruptManager() {
        stop();
    }

    void InterruptManager::addInterrupt(InterruptHandler handler, int type, int priority) {
        std::lock_guard<std::mutex> lock(_mutex);
        _interrupts[type].push({handler, priority});
        _cv.notify_one();
    }

    void InterruptManager::start() {
        _running = true;
        _threads.emplace_back(&InterruptManager::processInterrupts, this);
    }

    void InterruptManager::stop() {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _running = false;
        }
        _keyboardQueue.read = false;

        _keyboardThread.join();
        _cv.notify_all();
        _mouseQueue.read = false;
        _mouseThread.join();
        for (auto& thread : _threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    void InterruptManager::processInterrupts() {
        while (_running) {
            std::unique_lock<std::mutex> lock(_mutex);
            _cv.wait(lock, [this] { return !_interrupts.empty() || !_running; });

            if (!_running && _interrupts.empty()) {
                break;
            }

            Event ev;
            if (_keyboardQueue.poll(ev)) {
                if (auto it = _interrupts.find(ev.type); it != _interrupts.end()) {
                    std::priority_queue<Interrupt> tempQueue = it->second;
                    while (!tempQueue.empty()) {
                        auto& interrupt = tempQueue.top();
                        interrupt.handler(ev);
                        tempQueue.pop();
                    }
                }
            }
            if (_mouseQueue.poll(ev)) {
                if (auto it = _interrupts.find(ev.type); it != _interrupts.end()) {
                    std::priority_queue<Interrupt> tempQueue = it->second;
                    while (!tempQueue.empty()) {
                        auto& interrupt = tempQueue.top();
                        interrupt.handler(ev);
                        tempQueue.pop();
                    }
                }
            }
        }
    }
}