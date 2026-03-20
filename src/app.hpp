#pragma once

#include <sqlite3.h>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "auth.hpp"
#include "core/entry_handler.hpp"
#include "db/database.hpp"
#include "db/db_handler.hpp"
#include "ui_handler.hpp"
#include "utils/error_handler.hpp"

using namespace ftxui;

class Application {
   public:
    Application();

    Error init(const std::string& name, const std::string& version);
    Error welcome();

    void dashboard();
    void transactions();
    void statistics();

   private:
    std::string name;
    std::string version;

    Database     db;
    Auth         auth;
    EntryHandler entry_handler;
};
