
# pragma once

#include <iostream>
#include <queue>
#include <regex>
#include "tdl/Event/Event.hpp"
#include "tdl/Input/inputKeyboard.hpp"
#include "tdl/Event/Mouse/Imouse.hpp"

namespace tdl {

    /**
     * @class WindowBase
     * @brief WindowBase class
     * 
     */
    class WindowBase : public Event
    {
        public:

    /**
     * @brief Construct a new WindowBase object
     * 
     * @param fdPath the path to the fd
     */
        explicit WindowBase(std::string fdPath);

    /**
     * @brief Destroy the Window Base object
     * 
     */
            ~WindowBase();

    /**
     * @brief poll the event
     * 
     * @param event the event to poll
     * @return true if event is left in the queue
     * @return false if no event is left in the queue
     */
            bool pollEvent(Event &event, std::regex *custom = nullptr);

    /**
     * @brief Get the Fd object
     * 
     * @return int the fd of the window
     */
            [[nodiscard]] int getFd() const { return _fd; }

        protected:
            
            InputKeyboard _input; /*!< the input keyboard */
            Imouse *_mouse; /*!< the input mouse */
            int _fd{}; /*!< the fd of the window */
            std::string _fdPath; /*!< the path to the fd */
    };
}