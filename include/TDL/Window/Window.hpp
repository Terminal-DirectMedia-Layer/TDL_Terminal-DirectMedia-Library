
#ifndef TDL_WINDOW_HPP
    #define TDL_WINDOW_HPP

#include <string>
#include <list>
#include <iostream>
#include <array>
#include <vector>
#include <iostream>
#include <queue>
#include <regex>

#include <termios.h>

#include "TDL/Pixel/Pixel.hpp"
#include "TDL/Math/Vector.hpp"
#include "TDL/Matrix/PixelMatrix.hpp"
#include "TDL/Drawable/Drawable.hpp"
#include "TDL/Event/Event.hpp"
#include "TDL/Input/InputKeyboard.hpp"
#include "TDL/Event/Mouse/IMouse.hpp"
#include "TDL/Window/AWindow.hpp"

namespace tdl {
    /**
     * @class Window
     * @brief Window class
     */
    class Window : public AWindow {
        
        public:

/**
 * \section Constructor / Destructor
 * 
 */

        /**
         * @brief Create a Window object
         * 
         * @param title the title of the window
         * @param ttyPath the path to the tty by default it's /dev/tty
         * @return Window* a pointer to the window
         */
            static Window* createWindow(std::string const& title, std::string const& ttyPath = "/dev/tty");

        /**
         * @brief Destroy the Window object
         * 
         */
            ~Window();

/**
 * \subsection Input and event
 * 
 */

        /**
         * @brief poll the event
         * 
         * @param event the event to poll
         * @return true if event is left in the queue
         * @return false if no event is left in the queue
         */
            bool pollEvent(Event &event, std::regex *custom = nullptr) override;

        /**
         * @brief update the terminal size
         * @warning this function is called at the creation of the window and when the terminal size change
         * by the signale handler 
         */
            void updateTermSize() override;

        private:

/**
 * \section Private Methods
 * 
 */

        /**
         * @brief Construct a new tdl::Window::Window object
         * 
         * @param title The title of the window
         * @param tty_path The path to the tty to lauch the window, the available tty are /dev/tty or /dev/pts/0.../dev/pts/x
         * @note the SignalHandler.getInstance is used to register the window on the signal manager it permited to automatically resize the window when the terminal is resized
         */
            Window(std::string  title, std::string const& ttyPath);

/**
 * \section Attributes
 * 
 */
            InputKeyboard _input; /*!< the input keyboard */
            IMouse *_mouse; /*!< the input mouse */

    };
}

#endif //TDL_WINDOW_HPP
