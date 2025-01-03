
#ifndef MOUSE_HPP
    #define MOUSE_HPP

    #include <iostream>
    #include <regex>
    #include <map>
    #include <linux/input.h>

    #include "TDL/Event/Input/InputEvent.hpp"
    #include "TDL/Event/Event.hpp"
    #include "TDL/Utils/Thread/ThreadSafeQueue.hpp"

    #define MAX_EVENTS 64

    namespace tdl {

        class EventNotifier;

    class Mouse : public InputEvent
    {
        public:
            Mouse(ThreadSafeQueue<Event> &events);
            ~Mouse();

            void pollMouse();

            private:

              Event pollAbsEvent( struct input_event *ev);

            int _x;
            int _y;
            int _xrel;
            int _yrel;
            int _xTracking;
            int _yTracking;
            bool _isTouchTracking = false;

            ThreadSafeQueue<Event> &_events;

    };
    }

#endif //MOUSE_HPP
