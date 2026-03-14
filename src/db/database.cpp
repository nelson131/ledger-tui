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
        return {ERR_OPEN_DATABASE, std::string("database:init:db:") +
                                       std::string(sqlite3_errmsg(db))};
    }

    return {NONE};
}
