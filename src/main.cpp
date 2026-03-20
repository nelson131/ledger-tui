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

    Error err = app->init(app_name, app_version);
    if (err.code != 1) {
        raise_error(err);
        return 0;
    }

    err = app->welcome();
    if (err.code != 1 || err.code != 2) {
        raise_error(err);
        return 0;
    }

    app->dashboard();

    return 0;
}
