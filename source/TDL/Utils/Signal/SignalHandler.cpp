
#include <algorithm>

#include "TDL/Utils/Signal/SignalHandler.hpp"
#include "TDL/Event/Event.hpp"
#include <execinfo.h>


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
    std::cerr << "Signal received: " << sig << std::endl;
    if (sig == SIGINT)
        Display::getInstance().close();
    if (sig == SIGWINCH)
        getInstance().handleSignalInstance();
    if (sig == SIGSEGV || sig == SIGABRT) {
        std::cerr << "Segmentation fault" << std::endl;

        // Print stack trace
        void *array[10];
        size_t size = backtrace(array, 10);
        backtrace_symbols_fd(array, size, STDERR_FILENO);
        Display::destroyInstance();
        exit(1);
    }

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