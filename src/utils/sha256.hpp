#pragma once

#include <openssl/sha.h>

#include <iomanip>
#include <sstream>

std::string bytes_to_hex(const unsigned char* bytes, size_t length);
std::string sha256(const std::string& input);
