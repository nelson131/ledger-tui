#include "entry_handler.hpp"

#include <iostream>

EntryHandler::EntryHandler() {}

EntryHandler::EntryHandler(Database* db, const int& user_id)
    : db(db), user_id(user_id) {}

Error EntryHandler::init(Database* db, const int& user_id) {
    if (!db) {
        return {ERR_NULLPTR_OBJECT, "entry_handler:init:db"};
    }

    this->db = db;
    this->user_id = user_id;
    return {NONE};
}

Error EntryHandler::make_transaction(const std::string& source,
                                     const double&      amount,
                                     const std::string& currency,
                                     const std::string& tag) {
    if (source.empty() || currency.empty() || tag.empty()) {
        return {ERR_EMPTY_OBJECT, "entry_handler:make_transaction:arg"};
    }

    if (amount <= 0) {
        return {ERR_WRONG_OR_NEGATIVE_NUM,
                "entry_handler:make_transaction:amount"};
    }

    std::time_t timestamp = get_time();
    bool        is_recur = false;  // todo

    std::string query =
        "INSERT INTO transactions date, source, amount, currency, is_recur, "
        "tag) "
        "VALUES (?, ?, ?, ?, ?, ?)";

    Error insert_err = DBHandler::insert(db, query, timestamp, source, amount,
                                         currency, is_recur, tag);
    if (insert_err.code != 1) {
        return insert_err;
    }

    return {NONE};
}
