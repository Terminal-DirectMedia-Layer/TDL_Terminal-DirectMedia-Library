
#include <algorithm>

#include "TDL/Utils/Signal/SignalHandler.hpp"
#include "TDL/Event/Event.hpp"

tdl::SignalHandler &tdl::SignalHandler::getInstance()
{
    static SignalHandler instance;
    return instance;
}

void tdl::SignalHandler::registerWindow(Display *win)
{
    _windows.push_back(win);
}

void tdl::SignalHandler::unRegisterWindow(Display *win)
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
    for (auto &disp : _windows) {
        disp->updateSize();
        Event event;
        event.type = TDL_WINDOWRESIZED;
        event.size.width = disp->getSize().x();
        event.size.height = disp->getSize().y();
        disp->getNotifier().pushEvent(event);
    }
}