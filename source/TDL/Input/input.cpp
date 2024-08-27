/*
** EPITECH PROJECT, 2024
** libncurse
** File description:
** input.cpp
*/

#include "TDL/Input/InputKeyboard.hpp"
#include <unistd.h>
#include <sys/ioctl.h>
#include "TDL/Window/AWindow.hpp"

tdl::InputKeyboard::InputKeyboard(): _shared_data(KeyCodes::KEY_END)
{
}

tdl::InputKeyboard::~InputKeyboard() = default;

int tdl::InputKeyboard::readInputKeyboard(AWindow *win, char *buffer, int _nread) {
    int consumed = 0;
    if (_nread != 0) {
        if (buffer[0] == 27 && buffer[1] == 91) {
            switch (buffer[2]) {
                case 65:
                    _shared_data = KEY_UP;
                    break;
                case 66:
                    _shared_data = KEY_DOWN;
                    break;
                case 67:
                    _shared_data = KEY_RIGHT;
                    break;
                case 68:
                    _shared_data = KEY_LEFT;
                    break;
                default:
                    return 3;
            }
            consumed += 3;
        } else {
            _shared_data = charToKeyCodes(buffer[0]);
            consumed += 1;
        }
        _key_states[_shared_data] = true;
        Event event;
        event.type = Event::EventType::KEYPRESSED;
        event.key.code = _shared_data;
        win->pushEvent(event);
    } else {
        for (auto &key: _key_states) {
            if (key.second) {
                _prev_key_states[key.first] = true;
                key.second = false;
                Event event;
                event.type = Event::EventType::KEYRELEASED;
                event.key.code = charToKeyCodes(key.first);
                win->pushEvent(event);
            } else {
                _prev_key_states[key.first] = false;
            }
        }
    }
    return consumed;
}


tdl::KeyCodes tdl::InputKeyboard::charToKeyCodes(char key)
{
    return static_cast<KeyCodes>(key);
}