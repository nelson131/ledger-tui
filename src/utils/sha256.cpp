#include "sha256.hpp"

#include <iostream>

std::string bytes_to_hex(const unsigned char* bytes, size_t length) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; i++) {
        ss << std::setw(2) << static_cast<int>(bytes[i]);
    }

    return ss.str();
}

std::string sha256(const std::string& input) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX    sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.size());
    SHA256_Final(digest, &sha256);

    return bytes_to_hex(digest, SHA256_DIGEST_LENGTH);
}
