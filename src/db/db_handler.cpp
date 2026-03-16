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
Error init_user_table(const size_t& id, Database* database) {
    if (!database) {
        return {ERR_NULLPTR_OBJECT, "db_handler:init_user_table:database"};
    }

    if (!database->get_connection()) {
        Error init_err = database->init("resources/ledger-users-data.db");
        if (init_err.code != 1) {
            return init_err;
        }
    }

    std::string prefix = "user-" + std::to_string(id);

    std::string query_transactions =
        "CREATE TABLE IF NOT EXISTS " + prefix + "-transactions" +
        "(date TIMESTAMP, source TEXT, amount DOUBLE, currency TEXT, is_recur "
        "BOOL, tag TEXT)";
    Error tr_err = database->execute(query_transactions);
    if (tr_err.code != 1) {
        return tr_err;
    }

    std::string query_singletons =
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

};  // namespace DBHandler
