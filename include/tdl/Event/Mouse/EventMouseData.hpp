#include <string>
#include <sstream>

namespace tdl {
    class EventMouseData {
        public:
            EventMouseData(std::string buffer) {
                _buffer = buffer;
                std::stringstream ss(std::string(buffer.begin(), buffer.end()- 1));
                std::string temp;
                std::getline(ss, temp, ';');
                _button = std::stoi(temp);
                std::getline(ss, temp, ';');
                _x = std::stoi(temp);
                std::getline(ss, temp, ';');
                _y = std::stoi(temp);
            }
            ~EventMouseData() = default;

            int _x;
            int _y;
            int _button;
            std::string _buffer;
    };
}