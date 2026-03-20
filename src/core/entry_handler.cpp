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
    bool        is_recur = false;
    std::string query =
        "INSERT INTO transactions (date, source, amount, currency, is_recur, "
        "tag) "
        "VALUES (?, ?, ?, ?, ?, ?)";

    Error insert_err = DBHandler::insert(db, query, timestamp, source, amount,
                                         currency, is_recur, tag);
    if (insert_err.code != 1) {
        return insert_err;
    }

    return {NONE};
}

Error EntryHandler::delete_transaction(const int& id) {
    if (id < 0) {
        return {ERR_WRONG_OR_NEGATIVE_NUM,
                "entry_handler:delete_transaction:id"};
    }

    const std::string query = "DELETE FROM transactions WHERE user_id = ?";
    sqlite3_stmt*     stmt = DBHandler::query(db, query, id);
    if (!stmt) {
        return {ERR_NULLPTR_OBJECT, "entry_handler:delete_transaction:stmt"};
    }

    return {NONE};
}

Error EntryHandler::create_tag(const std::string& tag) {
    if (tag.empty()) {
        return {ERR_EMPTY_OBJECT, "entry_handler:create_tag:tag"};
    }

    std::string query = "INSERT INTO tags(user_id, name) VALUES (?, ?)";
    Error       err = DBHandler::insert(db, query, user_id, tag);
    if (err.code != 1) {
        return err;
    }

    return {NONE};
}

Error EntryHandler::make_recurring(const std::string& source,
                                   const double&      amount,
                                   const std::string& currency,
                                   const std::string& tag,
                                   const size_t&      day_interval) {
    if (source.empty() || currency.empty() || tag.empty()) {
        return {ERR_EMPTY_OBJECT, "entry_handler:make_reccuring:arg"};
    }

    if (amount <= 0) {
        return {ERR_WRONG_OR_NEGATIVE_NUM,
                "entry_handler:make_reccuring:amiunt"};
    }

    std::string query =
        "INSERT INTO recur_sources (user_id, source, amount, currency, tag, "
        "interval_days) VALUES (?, ?, ?, ?, ?, ?)";

    Error err = DBHandler::insert(db, query, user_id, source, amount, currency,
                                  tag, (int)day_interval);
    if (err.code != 1) {
        return err;
    }

    return {NONE};
}
