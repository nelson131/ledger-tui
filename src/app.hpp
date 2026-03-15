#pragma once

#include <sqlite3.h>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "db/database.hpp"
#include "db/db_handler.hpp"
#include "utils/error_handler.hpp"

using namespace ftxui;

class Application {
   public:
    Application();

    Error init(const std::string& name, const std::string& version);
    Error welcome();

    void render();
    void handle_input();

    const bool& is_running() const;

   private:
    std::string name;
    std::string version;
    bool        run;

    ftxui::ScreenInteractive screen;
    Database                 database;
};
