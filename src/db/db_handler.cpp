#include "db_handler.hpp"

#include <iostream>

using namespace DBHandler;

namespace DBHandler {

Error init_tables(Database* database) {
    if (!database) {
        return {ERR_NULLPTR_OBJECT, "db_handler:init_auth_table:database"};
    }

    Error err = database->init("resources/ledger.db");
    if (err.code != 1) {
        return err;
    }

    // Creating the main table with auth data and balance + last login at
    // account time
    err = database->execute(R"(
    CREATE TABLE IF NOT EXISTS users(
    id UNSIGNED INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE,
    password_sha256 TEXT,
    balance DOUBLE,
    last_login TIMESTAMP
    );
    )");
    if (err.code != 1) {
        return err;
    }

    // Creating the transactions table
    err = database->execute(R"(
    CREATE TABLE IF NOT EXISTS transactions(
    id UNSIGNED INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id UNSIGNED INTEGER,
    date TIMESTAMP,
    source TEXT,
    amount DOUBLE,
    is_recur BOOL,
    currency TEXT,
    tag TEXT
    )
    )");
    if (err.code != 1) {
        return err;
    }

    // Creating the recurring sources table
    // contains the sources that should be applied automaticly
    // relative to interval time
    err = database->execute(R"(
    CREATE TABLE IF NOT EXISTS recur_sources(
    user_id UNSIGNED INTEGER,
    source TEXT,
    amount DOUBLE,
    currency TEXT,
    tag TEXT,
    interval_days INTEGER
    )
    )");
    if (err.code != 1) {
        return err;
    }

    // Creating the tags table, every tag is attached to specific user
    err = database->execute(R"(
    CREATE TABLE IF NOT EXISTS tags(
    user_id UNSIGNED INTEGER,
    name TEXT
    )
    )");
    if (err.code != 1) {
        return err;
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

const int get_id(Database* database, const std::string& username) {
    if (!database) {
        raise_error({ERR_NULLPTR_OBJECT, "db_handler:get_id:database"});
        return -1;
    }

    if (username.empty()) {
        raise_error({ERR_EMPTY_OBJECT, "db_handler:get_id:username"});
        return -1;
    }

    const std::string query = "SELECT id FROM users_data WHERE username = ?;";

    sqlite3_stmt* table = DBHandler::prepare(database, query);
    if (!table) {
        raise_error({ERR_NULLPTR_OBJECT, "db_handler:get_id:table"});
        return -1;
    }

    sqlite3_bind_text(table, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(table);

    if (rc == SQLITE_DONE) {
        sqlite3_finalize(table);
        raise_error({ERR_DATABASE_NOT_FOUND, "db_handler:get_id:user"});
        return -1;
    }

    if (rc != SQLITE_ROW) {
        sqlite3_finalize(table);
        raise_error({ERR_DATABASE_FAILED, "db_handler:get_id:"});
        return -1;
    }

    int user_id = sqlite3_column_int(table, 0);
    sqlite3_finalize(table);
    return user_id;
}

void bind(sqlite3_stmt* stmt, int index, int value) {
    sqlite3_bind_int(stmt, index, value);
}

void bind(sqlite3_stmt* stmt, int index, double value) {
    sqlite3_bind_double(stmt, index, value);
}

void bind(sqlite3_stmt* stmt, int index, const std::string& value) {
    sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
}

void bind(sqlite3_stmt* stmt, int index, const char* value) {
    sqlite3_bind_text(stmt, index, value, -1, SQLITE_TRANSIENT);
}

void bind(sqlite3_stmt* stmt, int index, std::time_t time) {
    sqlite3_bind_int64(stmt, index, time);
}

};  // namespace DBHandler
