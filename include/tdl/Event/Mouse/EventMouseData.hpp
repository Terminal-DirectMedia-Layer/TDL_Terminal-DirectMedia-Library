#include <string>
#include <sstream>
#include <regex>

namespace tdl {
    class EventMouseData {
        public:
            EventMouseData(std::string buffer) {
                _buffer = buffer;
                std::regex re("(\\d+)");
                std::sregex_iterator begin(buffer.begin(), buffer.end(), re);
                std::sregex_iterator end;
                std::smatch match;
                int i = 0;
                for (std::sregex_iterator it = begin; it != end; ++it) {
                    match = *it;
                    if (i == 0) {
                        std::stringstream ss(match.str());
                        ss >> _button;
                    } else if (i == 1) {
                        std::stringstream ss(match.str());
                        ss >> _x;
                    } else if (i == 2) {
                        std::stringstream ss(match.str());
                        ss >> _y;
                    }
                    i++;
                }
            }
            ~EventMouseData() = default;

            int _x;
            int _y;
            int _button;
            std::string _buffer;
    };
}