
#include <iostream>
#include <regex>
#include <queue>
#include <fstream>

#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>

#include "TDL/Input/Event.hpp"

namespace tdl {

    std::list<std::string> InputEvent::findEventBus() const
    {
        std::ifstream file("/proc/bus/input/devices");
        if (!file.is_open()) {
            std::cerr << "Unable to open /proc/bus/input/devices" << std::endl;
            return {};
        }

        std::string line;
        std::regex event_regex("event\\d+");
        std::regex type_regex;
        std::smatch match;
        std::list<std::string> event_path = {};

       switch (_type) {
            case KEYBOARD:
                type_regex = std::regex("keyboard");
                break;
            case MOUSE:
                type_regex = std::regex("mouse");
                break;
            default:
                break;
        }

        while (std::getline(file, line)) {
            if (std::regex_search(line, match, type_regex)) {
                do { // hugly but permited to found the event if the name is on the same line
                    if (std::regex_search(line, match, event_regex)) {
                        std::string _event = "/dev/input/" + match.str();
                        event_path.push_back(_event);
                        break;
                    }
                } while (std::getline(file, line));
            }
        }
        return event_path;
    }

    InputEvent::InputEvent(inputEventType type) : _type(type)
    {
        int fd;
        _eventBus = findEventBus();
        if (_eventBus.empty()) {
            std::cerr << "Unable to find the event bus" << std::endl;
            return;
        }
        FD_ZERO(&_set);
        _timeout.tv_sec = 0;
        _timeout.tv_usec = 0;
        for (auto &event : _eventBus) {
            fd = open(event.c_str(), O_RDONLY | O_NONBLOCK);
            if (fd == -1) {
                std::cerr << "Unable to open the event bus" << std::endl;
                return;
            }
            _fds.push_back(fd);
            FD_SET(fd, &_set);
        }
    }

    InputEvent::~InputEvent()
    {
        for (auto &fd : _fds) {
            close(fd);
        }
    }

    std::unique_ptr<struct input_event> InputEvent::getInputEvent()
    {
        auto ev = std::make_unique<struct input_event>();

        for (auto &fd : _fds) {
            ssize_t size = read(fd, ev.get(), sizeof(struct input_event));
            if (size == -1) {
                continue;
            } else if (size == 0) {
                continue;
            }
            return ev;
        }
        return nullptr;
    }
} // namespace tdl