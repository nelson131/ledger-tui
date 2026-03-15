#pragma once

#include <string>

#include "db/db_handler.hpp"
#include "utils/error_handler.hpp"
#include "utils/sha256.hpp"

class Auth {
   public:
    Auth(Database* database);

    Error register_user(const std::string& username,
                        const std::string& password);
    Error login_user(const std::string& username, const std::string& password);

    const int& get_id() const;

   private:
    Database* database;

    int user_id;
};
