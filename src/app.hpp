#pragma once

#include "utils/error_handler.hpp"

class Application {
   public:
    Application();

    Error init();

    void render();
    void handle_input();

    const bool& is_running() const;

   private:
    bool run;
};
