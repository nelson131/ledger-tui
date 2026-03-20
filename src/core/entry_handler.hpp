#pragma once

#include <vector>

#include "../db/db_handler.hpp"
#include "../utils/error_handler.hpp"
#include "../utils/time.hpp"

class EntryHandler {
   private:
    Database* db;
    int       user_id;

   public:
    EntryHandler();
    EntryHandler(Database* db, const int& user_id);

    Error init(Database* db, const int& user_id);

    Error make_transaction(const std::string& source, const double& amount,
                           const std::string& currency, const std::string& tag);
    Error delete_transaction(const int& id);

    Error create_tag(const std::string& tag);
    Error make_recurring(const std::string& source, const double& amount,
                         const std::string& currency, const std::string& tag,
                         const size_t& day_interval);

    template <typename T>
    Error edit_transaction(const int& id, const std::string& field, T value) {
        if (id < 0) {
            return {ERR_WRONG_OR_NEGATIVE_NUM,
                    "entry_handler:edit_transaction:id"};
        }

        if (field.empty()) {
            return {ERR_EMPTY_OBJECT, "entry_handler:edit_transaction:field"};
        }

        const std::string query =
            "UPDATE transactions SET " + field + " = ? WHERE user_id = ?;";
        sqlite3_stmt* stmt = DBHandler::query(db, query, value);
        if (!stmt) {
            return {ERR_NULLPTR_OBJECT, "entry_handler:edit_transaction:stmt"};
        }

        sqlite3_finalize(stmt);
        return {NONE};
    }
};
