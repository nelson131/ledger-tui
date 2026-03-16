#include "db_handler.hpp"

#include <iostream>

using namespace DBHandler;

namespace DBHandler {

// Creating user data table(id, name, password in sha256)
Error init_auth_table(Database* database) {
    if (!database) {
        return {ERR_NULLPTR_OBJECT, "db_handler:init_auth_table:database"};
    }

    Error init_err = database->init("resources/ledger-auth.db");
    if (init_err.code != 1) {
        return init_err;
    }

    Error user_err = database->execute(R"(
    CREATE TABLE IF NOT EXISTS users_data(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE,
    password_sha256 TEXT
    );
    )");

    if (user_err.code != 1) {
        return user_err;
    }

    return {NONE};
}

// Creating two tables: user transactions (date, source, amount, currency, is
// recur, tag) and user singletons like balance, last time auth and sources
// which must be recur
Error init_user_table(Database* database, const int& user_id) {
    if (!database) {
        return {ERR_NULLPTR_OBJECT, "db_handler:init_user_table:database"};
    }

    if (!database->get_connection()) {
        Error init_err = database->init("resources/ledger-users-data.db");
        if (init_err.code != 1) {
            return init_err;
        }
    }

    const std::string prefix = "user-" + std::to_string(user_id);

    const std::string query_transactions =
        "CREATE TABLE IF NOT EXISTS " + prefix + "-transactions" +
        "(date TIMESTAMP, source TEXT, amount DOUBLE, currency TEXT, is_recur "
        "BOOL, tag TEXT)";
    Error tr_err = database->execute(query_transactions);
    if (tr_err.code != 1) {
        return tr_err;
    }

    const std::string query_singletons =
        "CREATE TABLE IF NOT EXISTS " + prefix + "-singletons" +
        "(balance DOUBLE, last_date TIMESTAMP, recur_source TEXT)";
    Error sngl_err = database->execute(query_transactions);
    if (sngl_err.code != 1) {
        return sngl_err;
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

};  // namespace DBHandler
