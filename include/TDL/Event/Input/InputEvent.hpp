
#ifndef INPUT_EVENT_HPP
    #define INPUT_EVENT_HPP

#include <iostream>
#include <regex>
#include <array>
#include <list>

#include <linux/input.h>
#include <sys/select.h>

namespace tdl {

    enum inputEventType {
        KEYBOARD,
        MOUSE,
    };

    /**
     * @struct Event
     * @brief The event structure. It's used to handle the event in the window
     * @note if you want to add a new event, you have to add it in the union and in the enum EventType if you want to store data for the event you can add it in the struct
     */
    class InputEvent
    {
            public:

            InputEvent(inputEventType type);
            ~InputEvent();

            std::unique_ptr<struct input_event> getInputEvent();

            private:

                std::list<std::string> findEventBus() const;

                inputEventType _type;
                std::list<std::string> _eventBus;
                fd_set _set;
                std::list<int> _fds;
                struct timeval _timeout;

      };
}
#endif //INPUT_EVENT_HPP
