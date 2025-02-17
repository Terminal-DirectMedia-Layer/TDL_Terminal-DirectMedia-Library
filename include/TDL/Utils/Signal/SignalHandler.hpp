

#ifndef TDL_SIGNALHANDLER_HPP
    #define TDL_SIGNALHANDLER_HPP

#include <csignal>

#include "TDL/Graphics/Display/Display.hpp"

namespace tdl {
/**
 * @class SignalHandler
 * @brief this class is in charge of handling all the signals for the windows
 * the pattern used is singleton
 */
    class SignalHandler {
        public:
/**
* @brief Construct or return the instance of the SignalHandler
*
* @return tdl::SignalHandler& the instance of the SignalHandler
*/
        static SignalHandler &getInstance();

        SignalHandler(SignalHandler const&) = delete; // copy constructor
        void operator=(SignalHandler const&) = delete; // copy operator

/**
 * @brief register a new window to the signal handler
 * when you register an window you can be sure that the window will be updated when the terminal is resized
 *
 * @param win the window to register
 */
        void registerWindow(Display *win);

/**
 * @brief unregister a window from the signal handler
 *
 * @param win the window to unregister
 */
        void unRegisterWindow(Display *win);

/**
 * @brief the function called when a signal is received
 *
 * @param sig the signal received
 */
        static void handleSignal(int sig);

        private:
/**
 * @brief Construct a new Signal Handler object
 *
 */
        SignalHandler() {
            std::signal(SIGWINCH, SignalHandler::handleSignal);
            std::signal(SIGINT, SignalHandler::handleSignal);
            std::signal(SIGSEGV, SignalHandler::handleSignal);
            std::signal(SIGABRT, SignalHandler::handleSignal);
        }

/**
 * @brief the instance off the signal recieve
 * this manage all the window registered to the signal handler
 *
 */
        void handleSignalInstance();

        std::vector<Display*> _windows; /**< the list of windows registered to the signal handler */
    };
}

#endif //TDL_SIGNALHANDLER_HPP
