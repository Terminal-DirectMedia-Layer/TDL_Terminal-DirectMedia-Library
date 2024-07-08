
#include <string>
#pragma once

namespace tdl {

    class Window;
    class EventMouseData;

    class Imouse {
        public:
            virtual ~Imouse() = default;

            virtual bool mouseMove(Window  *window, EventMouseData data) = 0;
            virtual bool mousePessed(Window  *window, EventMouseData data) = 0;
            virtual bool mouseReleased(Window  *window, EventMouseData data) = 0;
            virtual bool mouseScroll(Window  *window, EventMouseData data) = 0;
    };
}