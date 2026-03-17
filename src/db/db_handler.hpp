#pragma once

#include <openssl/sha.h>
#include <sqlite3.h>

#include <string>
#include <utility>

#include "../utils/error_handler.hpp"
#include "database.hpp"

namespace DBHandler {

Error init_auth_table(Database* database);
Error init_user_table(Database* database, const int& user_id);

sqlite3_stmt* prepare(Database* database, const std::string& query);

const int get_id(Database* database, const std::string& username);

void bind(sqlite3_stmt* stmt, int index, int value);
void bind(sqlite3_stmt* stmt, int index, double value);
void bind(sqlite3_stmt* stmt, int index, const std::string& value);
void bind(sqlite3_stmt* stmt, int index, const char* value);

template <typename... targs>
Error insert(Database* database, const std::string& query, targs&&... args) {
    sqlite3_stmt* stmt = prepare(database, query);
    if (!stmt) {
        return {ERR_NULLPTR_OBJECT, "db_handler:insert:stmt"};
    }

    int index = 1;
    ((bind(stmt, index++, std::forward<targs>(args))), ...);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return {ERR_DATABASE_EXECUTE, "db_handler:insert:sqlite3_step"};
    }

    sqlite3_finalize(stmt);
    return {NONE};
}

template <typename... targs>
sqlite3_stmt* query(Database* database, const std::string& query,
                    targs&&... args) {
    sqlite3_stmt* stmt = prepare(database, query);
    if (!stmt) {
        raise_error({ERR_NULLPTR_OBJECT, "db_handler:query:stmt"});
        return nullptr;
    }

    int index = 1;
    ((bind(stmt, index++, std::forward<targs>(args))), ...);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return nullptr;
    }

    return stmt;
}

};  // namespace DBHandler
