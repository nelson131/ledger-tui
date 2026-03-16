#pragma once

#include <openssl/sha.h>
#include <sqlite3.h>

#include <string>

#include "../utils/error_handler.hpp"
#include "database.hpp"

namespace DBHandler {

Error init_auth_table(Database* database);
Error init_user_table(Database* database, const int& user_id);

sqlite3_stmt* prepare(Database* database, const std::string& query);

const int get_id(Database* database, const std::string& username);

};  // namespace DBHandler
