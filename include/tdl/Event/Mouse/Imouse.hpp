
#include <string>
#pragma once

namespace tdl {

    class WindowBase;
    class EventMouseData;

    class Imouse {
        public:
            virtual ~Imouse() = default;

            virtual bool mouseMove(WindowBase  *window, EventMouseData data) = 0;
            virtual bool mousePessed(WindowBase  *window, EventMouseData data) = 0;
            virtual bool mouseReleased(WindowBase  *window, EventMouseData data) = 0;
            virtual bool mouseScroll(WindowBase  *window, EventMouseData data) = 0;
    };
}