
#ifndef EVENTNOTIFIER_HPP
    #define EVENTNOTIFIER_HPP

#include "TDL/Event/Event.hpp"

namespace tdl {

    class EventNotifier {
    public:

        void changeFocus();
        void notify();
        void pushEvent(const Event &event);

    private:
        std::queue<Event> _events; /*!< the event queue */

    };
}

#endif //EVENTNOTIFIER_HPP
