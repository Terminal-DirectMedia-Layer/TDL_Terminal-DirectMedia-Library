
#ifndef EVENT_HPP
#define EVENT_HPP

#include <iostream>
#include <regex>
#include <queue>
#include <fstream>
#include <string>
#include <cstring>

#include "TDL/Event/Mouse/IMouse.hpp"
#include "TDL/Event/InputEventCode.hpp"

namespace tdl {
    /**
     * @struct Event
     * @brief The event structure. It's used to handle the event in the window
     * @note if you want to add a new event, you have to add it in the union and in the enum EventType if you want to store data for the event you can add it in the struct
     */

    class Event
    {
    public:

        /**
         * @brief Construct a new Event object
         *
         */
        Event() = default;

        Event(const Event &event) = default;

        Event &operator=(const Event &event) = default;


        bool pollEvent(Event &event, std::regex *custom = nullptr);

        /**
         * @brief push an event in the queue
         *
         * @param event the event to push
         */
        void pushEvent(const Event &event);


        std::queue<Event> _events; /*!< the event queue */

        /**
         * @brief The key event structure
         * it permited to register the key that is pressed or released
         */
        struct keyEvent {
            const char * code;

            bool operator==(const char *other) const {
                return strcmp(code, other) == 0;
            }
        };

        /**
         * @brief The size event structure
         * it permited to register the size of the window
         */
        struct sizeEvent {
            int width;
            int height;
        };

        /**
         * @brief The mouse event structure
         * it permited to register the position of the mouse
         */
        struct mouseMoveEvent {
            int x;
            int y;
        };

        struct mouseButtonEvent {
            int button;
            int x;
            int y;
        };

        struct mouseScrollEvent {
            int direction;
            int x;
            int y;
        };

        struct customEvent {
            char *data;
        };

        	int type  = TDL_RESERVED; /*!< the type of the event */

            /**
             * @brief The union that contains all the event data
             *
             */
        union {
            keyEvent key; /* !< the key event */
            sizeEvent size; /* !< the size event */
            mouseMoveEvent mouseMove; /* !< the mouse Move event */
            mouseButtonEvent mouseButton; /* !< the mouse button event */
            mouseScrollEvent mouseScroll; /* !< the mouse scroll event */
            customEvent custom; /* !< the custom event */
        };
    };
}

#endif //EVENT_HPP