
#ifndef KEYBOARD_HPP
    #define KEYBOARD_HPP

    #include <iostream>
    #include <regex>
    #include <map>
    #include <linux/input.h>

    #include "TDL/Window/AWindow.hpp"
    #include "TDL/Input/Event.hpp"
    namespace tdl {

    const std::map<std::string, std::string> keymap = {
        {"fr", "/usr/share/kbd/keymaps/i386/azerty/fr-latin9.map.gz",},
        {"us", "/usr/share/kbd/keymaps/i386/qwerty/us-acentos.map.gz",}
    };

    class Keyboard : public InputEvent
    {
        public:
            Keyboard();
            ~Keyboard();

            void pollKeyboard(AWindow* window);
            private:

            std::string findCharset() const;
            void loadKeymap();

            std::string _charset;
            std::map<int, std::string> _keymap;
    };
    }

#endif //KEYBOARD_HPP
