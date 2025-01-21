
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <map>
#include <linux/input.h>
#include <zlib.h>
#include <linux/input-event-codes.h>

#include "TDL/Event/Input/Module/Keyboard.hpp"
#include "TDL/Event/EventNotifier.hpp"


namespace tdl {
    Keyboard::Keyboard(ThreadSafeQueue<Event> &event) : InputEvent(KEYBOARD), _events(event)
    {
        _charset = findCharset();
        loadKeymap();
    }

    Keyboard::~Keyboard()
    {
    }

    short Keyboard::getModifiers(struct input_event &event)
    {
        std::string key = _keymap[event.code];
        if (key == TDL_KEY_SHIFT) {
            return 1;
        }
        if (key == TDL_KEY_CONTROL) {
            return 2;
        }
        if (key == TDL_KEY_ALT) {
            return 4;
        }
        return 0;
    }

    void Keyboard::pollKeyboard()
    {
        std::optional<std::pair<struct libevdev *, std::unique_ptr<struct input_event>>> ret = std::nullopt;
        struct libevdev *dev = nullptr;
        struct input_event *ev = nullptr;
        while (_events.read) {
            ret = getInputEvent();

            if (!ret.has_value()) {
                continue;
            }
            dev = ret->first;
            ev = ret->second.get();
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
                    _events.push(event);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

        }
    }

    std::string Keyboard::findCharset() const
    {
        std::ifstream file;
        std::vector<std::string> files = {"/etc/default/keyboard", "/etc/vconsole.conf"};
        for (const auto &file_path : files) {
            file.open(file_path);
            if (file.is_open()) {
                break;
            }
        }
        if (!file.is_open()) {
            std::cerr << "Unable to open /etc/default/keyboard or /etc/vconsole.conf" << std::endl;
            return "";
        }
        std::string line;
        std::regex charset_regex("XKBLAYOUT=([^\"]+)");

        std::smatch match;
        std::string charset = "";
        while (std::getline(file, line)) {
            std::cerr << line << std::endl;
            if (std::regex_search(line, match, charset_regex)) {
                charset = match[1].str(); // Directly access the first capturing group
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
            }
        }
    }

} // namespace tdl
