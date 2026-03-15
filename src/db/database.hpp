#pragma once

#include <sqlite3.h>

#include <string>

#include "../utils/error_handler.hpp"

class Database {
   public:
    Database();
    Database(const std::string& db_file);

    ~Database();

    Error init(const std::string& db_file);

    Error    execute(const std::string& query);
    sqlite3* get_connection();

   private:
    sqlite3* db;
};
