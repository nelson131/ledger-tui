#include "error_handler.hpp"

#include <iostream>

const std::vector<const char*> error_messages = {
    "Failed",
    "Literally nothing",
    "OK",
    "Nullptr object",
    "Empty object",
    "Failed to find the file",
    "Something went wrong with db operation",
    "Failed to open database",
    "Failed to execute database operation",
    "Failed to get database table",
    "Something exists in the database",
    "Failed to find something in the database",
    "Failed to login: wrong password"};
