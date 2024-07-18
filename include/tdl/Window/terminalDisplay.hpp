#include <string>
#include <list>
#include <iostream>
#include <array>
#include <vector>
#include <termios.h>
#include <iostream>
#include <queue>
#include <regex>
#include "tdl/Pixel/Pixel.hpp"
#include "tdl/Vector.hpp"
#include "tdl/Matrix/PixelMatrix.hpp"
#include "tdl/Drawable/Drawable.hpp"
#include "tdl/Event/Event.hpp"
#include "tdl/Input/inputKeyboard.hpp"
#include "tdl/Event/Mouse/Imouse.hpp"
#include "tdl/Window/AWindow.hpp"
#include "tdl/Utils/SubShell.hpp"


#ifndef TDL_TERMINALDISPLAY_HPP
    #define TDL_TERMINALDISPLAY_HPP

namespace tdl
{
    class TerminalDisplay: public AWindow
    {
    public:

        /**
         * @brief Create a TerminalDisplay object
         * 
         * @param title the title of the window
         * @param ttyPath the path to the tty by default it's /dev/tty
         * @param height the height of the window region that you can draw on
         * @return TerminalDisplay* a pointer to the window
         */
        static TerminalDisplay *CreateTerminalDisplay(std::string const &title, std::string const &ttyPath = "/dev/tty", u_int32_t height = 0);

        /**
         * @brief Destroy the TerminalDisplay object
         * 
         */
        ~TerminalDisplay();

        /**
         * @brief poll the event
         * 
         * @param event the event to poll
         * @return true if event is left in the queue
         * @return false if no event is left in the queue
         */
            bool pollEvent(Event &event, std::regex *custom = nullptr);


        /**
         * @brief update the terminal size
         * @warning this function is called at the creation of the window and when the terminal size change
         * by the signale handler 
         */
            void updateTermSize();

        private:

/**
 * \section Attributes
 * 
 */
            InputKeyboard _input; /*!< the input keyboard */
            Imouse *_mouse; /*!< the input mouse */
            subShell _subShell; /*!< the subshell for command line execution*/
            tdl::Vector2u _cursorPos; /*!< the cursor position */
            TerminalDisplay(std::string const& title, std::string const& ttyPath = "/dev/tty", u_int32_t height = 20);

            void printAtCursorPos(std::string const& str);

    };
    
} // namespace 

#endif // TDL_TERMINALDISPLAY_HPP