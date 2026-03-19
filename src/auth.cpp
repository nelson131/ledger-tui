#include "auth.hpp"

#include <iostream>

Auth::Auth(Database* db) : db(db), user_id(-1) {}

Error Auth::register_user(const std::string& username,
                          const std::string& password) {
    const std::string query =
        "INSERT INTO users_data (username, password_sha256, balance, "
        "last_login) VALUES (?, ?, ?, ?);";
    std::string password_sha256 = sha256(password);
    std::time_t last_login = get_time();

    Error insert_err =
        DBHandler::insert(db, query, username, password_sha256, 0, last_login);
    if (insert_err.code != 1) {
        return insert_err;
    }

    return {OK, ""};
}

Error Auth::login_user(const std::string& username,
                       const std::string& password) {
    const std::string query =
        "SELECT id, password_sha256 FROM users_data WHERE username = ?;";

    sqlite3_stmt* stmt = DBHandler::query(db, query, username);
    if (!stmt) {
        return {ERR_NULLPTR_OBJECT, "auth:login_user:stmt"};
    }

    int user_id = sqlite3_column_int(stmt, 0);

    const char* hash =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    std::string password_sha256 = sha256(password);

    if (password_sha256 != hash) {
        sqlite3_finalize(stmt);
        return {ERR_WRONG_PASSWORD, "auth:login_user:password"};
    }

    sqlite3_finalize(stmt);

    this->user_id = user_id;
    this->username = username;
    return {OK, ""};
}

const int& Auth::get_id() const { return user_id; }

const std::string& Auth::get_username() const { return username; }
