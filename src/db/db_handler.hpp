#pragma once

#include <openssl/sha.h>
#include <sqlite3.h>

#include <string>

#include "../utils/error_handler.hpp"
#include "database.hpp"

namespace DBHandler {

Error init_auth_table(Database* database);
Error init_user_table(Database* database);

sqlite3_stmt* prepare(Database* database, const std::string& query);

};  // namespace DBHandler
