#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "error_handler.hpp"

class Config {
   public:
    Config();

    template <typename T>
    T get(const std::string& field, const std::string& key) {
        if (field.empty() || key.empty()) {
            return T{};
        }

        if (!file.is_open()) {
            raise_error(Error{ERR_FILE_NOT_FOUND, "config:get:file"});
        }

        file.clear();
        file.seekg(0);

        std::string line = "";
        bool        has_field = false;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            if (line[0] == '[') {
                if (has_field) break;
                line.erase(0, 1);
                line.erase(line.size() - 1);
                has_field = (field == line);
                continue;
            }

            if (has_field) {
                size_t pos = line.find("=");
                if (pos != std::string::npos) {
                    if (line.substr(0, pos) == key) {
                        std::string        value = line.substr(pos + 1);
                        std::istringstream str(value);

                        T res{};
                        str >> res;
                        if (str.fail()) {
                            raise_error(Error{
                                FAILED, std::string("config:get:convert:") +
                                            std::string(value)});
                            return T{};
                        }
                        return res;
                    }
                }
            }
        }
        raise_error(
            Error{ERR_FILE_NOT_FOUND,
                  std::string("config:get:") + std::string(field + "|" + key)});

        return T{};
    }

   private:
    std::ifstream file;
};

extern Config config;
