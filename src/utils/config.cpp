#include "config.hpp"

#include <iostream>

Config config;

Config::Config() {
    file = std::ifstream("resources/config.ini");
    if (!file.is_open()) {
        raise_error(Error{ERR_FILE_NOT_FOUND, "config:init"});
    }
}
