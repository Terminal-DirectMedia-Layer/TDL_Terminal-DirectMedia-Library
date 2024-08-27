
#include <string>
#pragma once

namespace tdl {

    class AWindow;
    class EventMouseData;

    class Imouse {
        public:
            virtual ~Imouse() = default;

            virtual bool mouseMove(AWindow  *window, EventMouseData data) = 0;
            virtual bool mousePessed(AWindow  *window, EventMouseData data) = 0;
            virtual bool mouseReleased(AWindow  *window, EventMouseData data) = 0;
            virtual bool mouseScroll(AWindow  *window, EventMouseData data) = 0;
    };
}