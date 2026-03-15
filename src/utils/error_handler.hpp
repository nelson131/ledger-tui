#pragma once

#include <iostream>
#include <string>
#include <vector>

enum ErrorCode {
    FAILED,
    NONE,
    OK,
    ERR_NULLPTR_OBJECT,
    ERR_EMPTY_OBJECT,
    ERR_FILE_NOT_FOUND,
    // Database
    ERR_DATABASE_FAILED,
    ERR_DATABASE_OPEN,
    ERR_DATABASE_EXECUTE,
    ERR_DATABASE_GET_TABLE,
    ERR_DATABASE_EXISTS,
    ERR_DATABASE_NOT_FOUND,
    // Auth
    ERR_WRONG_PASSWORD
};

struct Error {
    ErrorCode   code;
    std::string message = "";
};

extern const std::vector<const char*> error_messages;

template <typename... targs>
void raise_error(Error error, const targs&... args) {
    std::cout << error.message << std::endl;
}
