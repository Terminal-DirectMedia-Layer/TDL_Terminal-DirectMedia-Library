
#ifndef TDL_TERMINALDISPLAY_HPP
    #define TDL_TERMINALDISPLAY_HPP

#include <string>
#include <regex>
#include <functional>
#include <map>

#include <termios.h>

#include "TDL/Pixel/Pixel.hpp"
#include "TDL/Math/Vector.hpp"
#include "TDL/Matrix/PixelMatrix.hpp"
#include "TDL/Drawable/Drawable.hpp"
#include "TDL/Event/Event.hpp"
#include "TDL/Input/InputKeyboard.hpp"
#include "TDL/Event/Mouse/IMouse.hpp"
#include "TDL/Window/AWindow.hpp"
#include "TDL/Shell/SubShell.hpp"

namespace tdl
{
    class TerminalDisplay: public AWindow, SubShell
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
        static TerminalDisplay *createTerminalDisplay(std::string const &title, std::string const &ttyPath = "/dev/tty", u_int32_t height = 0, std::string const& shellPath = "/bin/bash");

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

            void setHeight(u_int32_t height) 
            {
                _size.y() = height;
                updateTermSize();
            }

            u_int32_t getHeight() const { return _size.y(); }

        /**
         * @brief update the terminal size
         * @warning this function is called at the creation of the window and when the terminal size change
         * by the signale handler 
         */
            void updateTermSize();


        using CommandFunction = std::function<void(TerminalDisplay*, int, char**)>;

        /**
         * @brief register an new command
         * 
         */
            void registerCommand(std::string const& cmd, CommandFunction func);

        /**
         * @brief unregister a command
         * 
         */
            void unregisterCommand(std::string const& cmd);

        /**
        * @brief get the subshell active path
        *
        */
            std::string getActivePath() const { return _subShell.getActivePath(); }

            Vector2u getTerminalSize() override { return _size; }


        private:

/**
 * \section Attributes
 * 
 */
            InputKeyboard _input; /*!< the input keyboard */
            IMouse *_mouse; /*!< the input mouse */
            SubShell _subShell; /*!< the subshell for command line execution*/
            tdl::Vector2u _cursorPos; /*!< the cursor position */
            std::string _cmd = ""; /*!< the command line */
            std::map<std::string, CommandFunction> _commandMap;
            TerminalDisplay(std::string const& title, std::string const& ttyPath = "/dev/tty", u_int32_t height = 20, std::string const& shellPath = "/bin/bash");

            void printAtCursorPos(std::string const& str);
            void removeCurrentLine();
            std::pair<int, int> getCursorPosition();
    };
    
} // namespace 

#endif // TDL_TERMINALDISPLAY_HPP