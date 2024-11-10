
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <map>
#include <linux/input.h>
#include <zlib.h>

#include "TDL/Input/Module/Keyboard.hpp"
#include "TDL/Window/AWindow.hpp"


namespace tdl {
    Keyboard::Keyboard() : InputEvent(KEYBOARD)
    {
        _charset = findCharset();
        loadKeymap();
    }

    Keyboard::~Keyboard()
    {
    }

    void Keyboard::pollKeyboard(AWindow* window)
    {
        std::unique_ptr<struct input_event> ev;
        ev = getInputEvent();
        if (ev == nullptr) {
            return;
        }
        Event event;
        if (ev->type == EV_KEY) {
            if (_keymap.find(ev->code) != _keymap.end()) {
            switch (ev->value) {
                case 0:
                    event.type = TDL_KEYPRESSED;
                    break;
                case 1:
                    event.type = TDL_KEYRELEASED;
                    break;
                case 2:
                    event.type = TDL_KEYREPEAT;
                    break;
                default:
                    break;
                }
                event.key.code = _keymap[ev->code].c_str();
                window->pushEvent(event);
            }
        }

    }

    std::string Keyboard::findCharset() const
    {
        std::ifstream file("/etc/default/keyboard");
        if (!file.is_open()) {
            std::cerr << "Unable to open /etc/default/keyboard" << std::endl;
            return "";
        }

        std::string line;
        std::regex charset_regex("XKBLAYOUT=\"[a-zA-Z]+\"");
        std::smatch match;
        std::string charset = "";

        while (std::getline(file, line)) {
            if (std::regex_search(line, match, charset_regex)) {
                charset = match.str();
                charset = charset.substr(11, charset.size() - 12);
                break;
            }
        }
        return charset;
    }

    void Keyboard::loadKeymap()
    {
        std::string keymap_path = keymap.at(_charset);
        gzFile file = gzopen(keymap_path.c_str(), "r");
        if (file == NULL) {
            std::cerr << "Unable to open " << keymap_path << std::endl;
            return;
        }

        std::stringstream ss;
        char buffer[128];
        int bytesRead = 0;
        while ((bytesRead = gzread(file, buffer, sizeof(buffer))) > 0) {
            ss.write(buffer, bytesRead);
        }
        gzclose(file);

        std::string line;
        std::regex keymap_regex(R"(keycode\s+(\d+)\s*=\s*([^\s]+))");
        std::smatch match;
        int keycode;
        std::string key;

        while (std::getline(ss, line)) {
            if (std::regex_search(line, match, keymap_regex)) {
                keycode = std::stoi(match[1]);
                key = match[2];
				key.erase(std::remove(key.begin(), key.end(), '+'), key.end());
                if (_keymap.find(keycode) != _keymap.end()) {
                    continue;
                }
                _keymap[keycode] = key;
                //std::cerr << "Keycode: " << keycode << " Key " << key << std::endl;
            }
        }
    }

} // namespace tdl
