
#ifndef MOUSE_HPP
    #define MOUSE_HPP

    #include <iostream>
    #include <regex>
    #include <map>
    #include <linux/input.h>

    #include "TDL/Window/AWindow.hpp"
    #include "TDL/Input/Event.hpp"

    #define MAX_EVENTS 64

    namespace tdl {

    class TTYMouse : public InputEvent
    {
        public:
            TTYMouse();
            ~TTYMouse();

            void pollMouse(AWindow* window);

            private:

              Event pollAbsEvent( struct input_event *ev);

            int _x;
            int _y;
            int _xrel;
            int _yrel;
            int _xTracking;
            int _yTracking;
            bool _isTouchTracking = false;

    };
    }

#endif //MOUSE_HPP
