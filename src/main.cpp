#include <iostream>
#include <memory>

#include "app.hpp"
#include "utils/config.hpp"
#include "utils/error_handler.hpp"

int main() {
    std::unique_ptr<Application> app = std::make_unique<Application>();
    std::unique_ptr<Config>      config = std::make_unique<Config>();

    const std::string app_name = config->get<std::string>("general", "name");
    const std::string app_version =
        config->get<std::string>("general", "version");

    Error init_err = app->init(app_name, app_version);
    if (init_err.code != 1) {
        raise_error(init_err);
        return 0;
    }

    Error login_err = app->welcome();
    if (login_err.code != 1) {
        raise_error(login_err);
        return 0;
    }

    return 0;
}
