
#include <iostream>
#include <regex>
#include <queue>
#include <fstream>

#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>

#include "TDL/Event/Input/InputEvent.hpp"

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
            _devs.push_back(libevdev_new());
            libevdev_new_from_fd(fd, &_devs.back());
            std::cerr << "Event bus found: " << event << " fd: " << fd << " dev: " << _devs.back() << std::endl;
            FD_SET(fd, &_set);
        }
    }

    InputEvent::~InputEvent()
    {
        for (auto &dev : _devs) {
            libevdev_free(dev);
        }
        for (auto &fd : _fds) {
            close(fd);
        }
    }

#include <optional>

    std::optional<std::pair<struct libevdev *, std::unique_ptr<struct input_event>>> InputEvent::getInputEvent()
    {
        auto ev = std::make_unique<struct input_event>();

        for (auto dev : _devs) {
            if (libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, ev.get()) == 0) {
                return std::make_pair(dev, std::move(ev));
            }
        }

        return std::nullopt;
    }
} // namespace tdl