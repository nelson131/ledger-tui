#pragma once

#include <openssl/sha.h>
#include <sqlite3.h>

#include <string>

#include "database.hpp"

namespace DBHandler {

sqlite3_stmt* prepare(Database* database, const std::string& query);

};
