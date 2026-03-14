#pragma once

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
    ERR_DATABASE_OPEN,
    ERR_DATABASE_EXECUTE
};

struct Error {
    ErrorCode   code;
    std::string message = "";
};

extern const std::vector<const char*> error_messages;

template <typename... targs>
void raise_error(Error error, const targs&... args) {}
