#include "auth.hpp"

#include <iostream>

Auth::Auth(Database* database) : database(database), user_id(-1) {}

Error Auth::register_user(const std::string& username,
                          const std::string& password) {
    const std::string query =
        "INSERT INTO users_data (username, password_sha256) VALUES (?, ?);";

    sqlite3_stmt* table = DBHandler::prepare(database, query);
    if (!table) {
        return {ERR_NULLPTR_OBJECT, "auth:register_user:table"};
    }

    std::string password_sha256 = sha256(password);

    sqlite3_bind_text(table, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(table, 2, password_sha256.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(table);
    if (rc == SQLITE_CONSTRAINT) {
        sqlite3_finalize(table);
        return {ERR_DATABASE_EXISTS, "auth:register_user:user"};
    }

    if (rc != SQLITE_DONE) {
        sqlite3_finalize(table);
        return {ERR_DATABASE_FAILED, "auth:register_user:"};
    }

    sqlite3_finalize(table);

    return {OK, ""};
}

Error Auth::login_user(const std::string& username,
                       const std::string& password) {
    const std::string query =
        "SELECT id, password_sha256 FROM users_data WHERE username = ?;";

    sqlite3_stmt* table = DBHandler::prepare(database, query);
    if (!table) {
        return {ERR_NULLPTR_OBJECT, "auth:login_user:table"};
    }

    sqlite3_bind_text(table, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(table);

    if (rc == SQLITE_DONE) {
        sqlite3_finalize(table);
        return {ERR_DATABASE_NOT_FOUND, "auth:login_user:user"};
    }

    if (rc != SQLITE_ROW) {
        sqlite3_finalize(table);
        return {ERR_DATABASE_FAILED, "auth:login_user:"};
    }

    int user_id = sqlite3_column_int(table, 0);

    const char* hash =
        reinterpret_cast<const char*>(sqlite3_column_text(table, 1));
    std::string password_sha256 = sha256(password);

    if (password_sha256 != hash) {
        sqlite3_finalize(table);
        return {ERR_WRONG_PASSWORD, "auth:login_user:password"};
    }

    sqlite3_finalize(table);

    this->user_id = user_id;
    this->username = username;
    return {OK, ""};
}

const int& Auth::get_id() const { return user_id; }

const std::string& Auth::get_username() const { return username; }
