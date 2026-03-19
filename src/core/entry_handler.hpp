#pragma once

#include <vector>

#include "../db/db_handler.hpp"
#include "../utils/error_handler.hpp"
#include "../utils/time.hpp"

class EntryHandler {
   public:
    EntryHandler();
    EntryHandler(Database* db, const int& user_id);

    Error init(Database* db, const int& user_id);

    Error make_transaction(const std::string& source, const double& amount,
                           const std::string& currency, const std::string& tag);
    Error delete_transaction(const int& id);
    Error edit_transaction(const int& id);

    Error create_tag(const std::string& tag);
    Error make_recurring(const std::string& source, const double& amount,
                         const std::string& currency, const std::string& tag,
                         const size_t& day_interval);

   private:
    Database* db;
    int       user_id;
};
