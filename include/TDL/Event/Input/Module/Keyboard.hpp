
#ifndef KEYBOARD_HPP
    #define KEYBOARD_HPP

    #include <iostream>
    #include <regex>
    #include <map>
    #include <linux/input.h>

    #include "TDL/Event/Input/InputEvent.hpp"
    #include "TDL/Utils/Thread/ThreadSafeQueue.hpp"
    #include "TDL/Event/Event.hpp"

    namespace tdl {

        class EventNotifier;

    const std::map<std::string, std::string> keymap = {
        {"fr", "/usr/share/kbd/keymaps/i386/azerty/fr-latin9.map.gz",},
        {"us", "/usr/share/kbd/keymaps/i386/qwerty/us-acentos.map.gz",}
    };

    class Keyboard : public InputEvent
    {
        public:
            Keyboard(ThreadSafeQueue<Event> &events);
            ~Keyboard();

            void pollKeyboard();
            private:

            short getModifiers(struct input_event &event);

            std::string findCharset() const;
            void loadKeymap();

            std::string _charset;
            std::map<int, std::string> _keymap;
            ThreadSafeQueue<Event> &_events;
    };
    }

#endif //KEYBOARD_HPP
