
#ifndef TDL_INPUT_MOUSE_HPP
    #define TDL_INPUT_MOUSE_HPP

#include <thread>
#include <mutex>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "EInput.hpp"
#include <map>

namespace tdl {

    class WindowBase;

/**
 * @class inputMouse
 * @brief Class to handle the mouse input
 * 
 */

    class inputMouse {
    public:

        inputMouse();

        ~inputMouse();

        int readInputMouse(WindowBase *window, std::string buffer, int _nread);
    };

} // namespace tdl

#endif //TDL_INPUT_MOUSE_HPP