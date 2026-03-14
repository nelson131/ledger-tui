#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "utils/error_handler.hpp"

class Application {
   public:
    Application();

    Error login();

    void render();
    void handle_input();

    const bool& is_running() const;

   private:
    bool run;

    ftxui::ScreenInteractive* screen;
};
