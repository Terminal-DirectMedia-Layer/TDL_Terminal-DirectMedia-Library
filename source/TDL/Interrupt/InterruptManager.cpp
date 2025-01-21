#include "TDL/Interrupt/InterruptManager.hpp"
#include "TDL/Graphics/Display/Display.hpp"

namespace tdl {

    void mouseCallBack(tdl::Event &event);
    void changeFocusOnMouse(tdl::Event &event);
    void changeFocusOnKey(tdl::Event &event);
    void grabWindow(tdl::Event &event);
    void releaseWindow(tdl::Event &event);
    void moveWindow(tdl::Event &event);
    Vector2i _clickDelta; // for internal use

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
        this->addInterrupt(mouseCallBack,TDL_MOUSEMOVED, 1);
        this->addInterrupt(changeFocusOnMouse,TDL_MOUSEPRESSED, 1);
        this->addInterrupt(changeFocusOnKey,TDL_KEYPRESSED, 1);
        this->addInterrupt(grabWindow,TDL_MOUSEPRESSED, 2);
        this->addInterrupt(releaseWindow,TDL_MOUSERELEASED, 2);
        this->addInterrupt(moveWindow,TDL_MOUSEMOVED, 2);
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

    // section for internal interupt

    void mouseCallBack(tdl::Event &event) {
        if (event.type == TDL_MOUSEMOVED) {
            tdl::Display::getInstance()._cursor.setNextPos(tdl::Vector2u(event.mouseMove.x , event.mouseMove.y));
        }
    }

    void changeFocusOnMouse(tdl::Event &event) {
        std::vector<Window *> &windows = tdl::Display::getInstance().getWindowsList();
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

    void changeFocusOnKey(tdl::Event &event) {
        std::vector<Window *> &windows = tdl::Display::getInstance().getWindowsList();
        if (event.type == TDL_KEYPRESSED && event.key == TDL_KEY_Q) {
            Window *new_focus = windows.front();
            windows.erase(windows.begin());
            windows.push_back(new_focus);
        }
    }

    void grabWindow(tdl::Event &event) {
        Window *win = tdl::Display::getInstance().getWinFocus();
        if (event.type == TDL_MOUSEPRESSED && win->isClickIn(Vector2u(event.mouseButton.x, event.mouseButton.y)) == BORDER) {
            win->grab = true;
            _clickDelta = Vector2i(event.mouseButton.x - win->getPosition().x(), event.mouseButton.y - win->getPosition().y());
        }
    }
    void releaseWindow(tdl::Event &event) {
        Window *win = tdl::Display::getInstance().getWinFocus();
        if (event.type == TDL_MOUSERELEASED) {
            win->grab = false;
            _clickDelta = Vector2i(0, 0);
        }
    }
    void moveWindow(tdl::Event &event) {
        Window *win = tdl::Display::getInstance().getWinFocus();
        if (win->grab) {
            win->setPosition(Vector2u(event.mouseMove.x - _clickDelta.x(), event.mouseMove.y - _clickDelta.y()));
        }
    }


}
