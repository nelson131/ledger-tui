#include "database.hpp"

#include <iostream>

Database::Database() : db(nullptr) {}

Database::Database(const std::string& db_file) {
    Error err = init(db_file);
    if (err.code != 1) {
        raise_error(err);
    }
}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

Error Database::init(const std::string& db_file) {
    if (db_file.empty()) {
        return {ERR_EMPTY_OBJECT, "database:init:db_file"};
    }

    if (sqlite3_open(db_file.c_str(), &db)) {
        return {ERR_DATABASE_OPEN, std::string("database:init:db:") +
                                       std::string(sqlite3_errmsg(db))};
    }

    return {NONE};
}

Error Database::execute(const std::string& query) {
    char* err_msg = nullptr;
    int   rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        Error err = {ERR_DATABASE_EXECUTE,
                     std::string("database:execute:") + std::string(err_msg)};
        sqlite3_free(err_msg);
        return err;
    }

    return {NONE};
}

sqlite3* Database::get_connection() { return db; }
