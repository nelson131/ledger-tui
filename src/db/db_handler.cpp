#include "db_handler.hpp"

#include <iostream>

using namespace DBHandler;

namespace DBHandler {

sqlite3_stmt* prepare(Database* database, const std::string& query) {
    sqlite3_stmt* stmt = nullptr;
    sqlite3*      db = database->get_connection();
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        raise_error(
            Error{ERR_DATABASE_GET_TABLE, std::string("db_hander:get_table:") +
                                              std::string(sqlite3_errmsg(db))});
        return nullptr;
    }

    return stmt;
}

};  // namespace DBHandler
