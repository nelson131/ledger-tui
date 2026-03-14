#pragma once

#include <string>

enum ErrorCode { FAILED, NONE, OK, ERR_NULLPTR_OBJECT, ERR_EMPTY_OBJECT };

struct Error {
    ErrorCode   code;
    std::string message = "";
};

void raise_error(Error error);
