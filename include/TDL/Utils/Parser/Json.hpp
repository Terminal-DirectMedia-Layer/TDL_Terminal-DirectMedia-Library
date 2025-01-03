
#ifndef JSON_HPP
    #define JSON_HPP

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

namespace tdl {

    class JsonParser {
    public:
        JsonParser() = default;
        ~JsonParser() = default;

        void parseFile(const std::string &file) {
            std::ifstream inputFile(file);
            if (!inputFile.is_open()) {
                throw std::runtime_error("Could not open file");
            }
            inputFile >> jsonData;
        }

        nlohmann::json::value_type key_value(const std::string &key) const {
            if (jsonData.contains(key)) {
                return jsonData.at(key);
            } else {
                throw std::runtime_error("Key not found in JSON data");
            }
        }

    private:
        nlohmann::json jsonData;
    };
}
#endif //JSON_HPP
