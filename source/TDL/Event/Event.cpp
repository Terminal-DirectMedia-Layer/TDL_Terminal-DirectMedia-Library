
#include "TDL/Event/Event.hpp"
#include <regex>
#include <iostream>
#include <queue>

namespace tdl
{

/**
 * @brief push an event into the queue
 * that permited to register an event when you want to
 * 
 * @param event the event to push in the queue
 */
void Event::pushEvent(const tdl::Event &event)
{
    _events.push(event);
}
    

bool pollEvent(Event &event, std::regex *custom = nullptr)
{
    return false;
}

} // namespace tdl