
#ifndef TTYDISPLAY_HPP
    #define TTYDISPLAY_HPP


#include <string>
#include <list>
#include <iostream>
#include <array>
#include <vector>
#include <iostream>
#include <queue>
#include <regex>

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "TDL/Pixel/Pixel.hpp"
#include "TDL/Math/Vector.hpp"
#include "TDL/Matrix/PixelMatrix.hpp"
#include "TDL/Drawable/Drawable.hpp"
#include "TDL/Event/Event.hpp"
#include "TDL/Input/InputKeyboard.hpp"
#include "TDL/Event/Mouse/Linux/GpmMouse.hpp"
#include "TDL/Window/AWindow.hpp"

namespace tdl {
    /**
     * @class Window
     * @brief Window class
     */
    class TtyDisplay : public AWindow {

        public:

/**
 * \section Constructor / Destructor
 *
 */

        /**
         * @brief Create a TtyDisplay object
         *
         * @param title the title of the window
         * @param ttyPath the path to the tty by default it's /dev/tty
         * @return Window* a pointer to the window
         */
            static TtyDisplay* createTtyDisplay(std::string const& title, std::string const& ttyPath = "/dev/tty");

        /**
         * @brief Destroy the Window object
         *
         */
            ~TtyDisplay();


        void update(bool all = false);

        void clearPixel();

        void draw() override;
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

            Vector2u getTerminalSize() override {return _terminalSize;}

        private:

/**
 * \section Private Methods
 *
 */

        /**
         * @brief Construct a new tdl::TtyDisplay::Window object
         *
         * @param title The title of the window
         * @param tty_path The path to the tty to lauch the window, the available tty are /dev/tty or /dev/pts/0.../dev/pts/x
         * @note the SignalHandler.getInstance is used to register the window on the signal manager it permited to automatically resize the window when the terminal is resized
         */
            TtyDisplay(std::string  title, std::string const& ttyPath);

/**
 * \section Attributes
 *
 */
            InputKeyboard _input; /*!< the input keyboard */
            GpmMouse *_mouse; /*!< the input mouse */
            struct fb_var_screeninfo _vinfo; /*!< the variable screen information */
            struct fb_fix_screeninfo _finfo; /*!< the fixed screen information */
            long int _screensize; /*!< the size of the screen in bytes */
            char *_fbp; /*!< the framebuffer pointer */
            int _fbfd; /*!< the file descriptor of the framebuffer */
            char *_blackScreen; /*!< the black screen */
            char *_displayScreen; /*!< the screen to display */
            Vector2u _terminalSize; /*!< the terminal size */
    };
}

#endif //TTYDISPLAY_HPP
