
#include <string>

#ifndef IMOUSE_HPP
    #define IMOUSE_HPP
namespace tdl {

    class AWindow;
    class EventMouseData;

    class IMouse {
        public:
            virtual ~IMouse() = default;

            virtual bool mouseMove(AWindow  *window, EventMouseData data) = 0;
            virtual bool mousePessed(AWindow  *window, EventMouseData data) = 0;
            virtual bool mouseReleased(AWindow  *window, EventMouseData data) = 0;
            virtual bool mouseScroll(AWindow  *window, EventMouseData data) = 0;
    };
}
#endif // IMOUSE_HPP