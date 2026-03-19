#pragma once

#include <ctime>
#include <iostream>

inline std::time_t get_time() { return std::time(nullptr); }
