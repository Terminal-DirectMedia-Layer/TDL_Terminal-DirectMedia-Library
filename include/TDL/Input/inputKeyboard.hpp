
#ifndef LIBNCURSE_INPUT_KEYBOARD_HPP
    #define LIBNCURSE_INPUT_KEYBOARD_HPP

#include <thread>
#include <mutex>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "EInput.hpp"
#include <map>

namespace tdl {

    class AWindow;

/**
 * @class InputKeyboard
 * @brief Class to handle the keyboard input
 *
 */
    class InputKeyboard {
    public:
/**
 * @brief Construct a new InputKeyboard object
 *
 */
        InputKeyboard();

/**
 * @brief Destroy the InputKeyboard object
 *
 */
        ~InputKeyboard();

/**
 * @brief Read the input from the keyboard and push the event in the window event queue
 *
 * @param window the window where the event will be pushed and read
 */
        int readInputKeyboard(AWindow *window, char *buffer, int _nread);

/**
 * @brief return a cast between a char and a TDLKeyCodes
 *
 * @param key the char to cast
 * @return tdl::TDLKeyCodes the casted value
 */
        static KeyCodes charToKeyCodes(char key);

    protected:
        KeyCodes _shared_data;
        std::map<char, bool> _key_states;
        std::map<char, bool> _prev_key_states;  
    };
} // namespace tdl

#endif //LIBNCURSE_INPUT_KEYBOARD_HPP
