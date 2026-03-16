#include "db_handler.hpp"

#include <iostream>

using namespace DBHandler;

namespace DBHandler {

Error init_tables(Database* database) {
    if (!database) {
        return {ERR_NULLPTR_OBJECT, "db_handler:init_tables:database"};
    }

    Error init_err = database->init("resources/ledger.db");
    if (init_err.code != 1) {
        return init_err;
    }

    Error exec_err = database->execute(R"(
    CREATE TABLE IF NOT EXISTS users_data(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE,
    password_sha256 TEXT
    );
    )");

    if (exec_err.code != 1) {
        return exec_err;
    }

    return {NONE};
}

sqlite3_stmt* prepare(Database* database, const std::string& query) {
    if (!database) {
        raise_error({ERR_NULLPTR_OBJECT, "db_handler:prepare:database"});
        return nullptr;
    }

    if (query.empty()) {
        raise_error({ERR_EMPTY_OBJECT, "db_handler:prepare:query"});
        return nullptr;
    }

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
