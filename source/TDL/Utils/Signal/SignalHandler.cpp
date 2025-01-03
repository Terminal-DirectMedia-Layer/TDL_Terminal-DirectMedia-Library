
#include <algorithm>

#include "TDL/Utils/Signal/SignalHandler.hpp"
#include "TDL/Event/Event.hpp"

tdl::SignalHandler &tdl::SignalHandler::getInstance()
{
    static SignalHandler instance;
    return instance;
}

void tdl::SignalHandler::registerWindow(Window *win)
{
    _windows.push_back(win);
}

void tdl::SignalHandler::unRegisterWindow(Window *win)
{
    _windows.erase(std::remove(_windows.begin(), _windows.end(), win), _windows.end());
}

void tdl::SignalHandler::handleSignal(int sig)
{
    if (sig == SIGWINCH)
        getInstance().handleSignalInstance();
}

void tdl::SignalHandler::handleSignalInstance()
{
    /*
    for (auto &win : _windows) {
        win->updateTermSize();
        Event event;
        event.type = TDL_WINDOWRESIZED;
        event.size.width = win->getWidth();
        event.size.height = win->getHeight();
        win->pushEvent(event);
    }
    */
}