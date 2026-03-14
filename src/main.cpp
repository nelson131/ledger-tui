#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <memory>

#include "app.hpp"

int main() {
    std::unique_ptr<Application> app = std::make_unique<Application>();

    Error err = app->login();
    if (err.code != 1) {
        return 0;
    }

    while (app->is_running()) {
        app->handle_input();
        app->render();
    }

    return 0;
}
