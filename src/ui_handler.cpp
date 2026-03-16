#include "ui_handler.hpp"

#include <iostream>

namespace UIHandler {

Element make_header(const std::string& app_name, const std::string& app_version,
                    const std::string& username) {
    return hbox({text(app_name + " | "), text(app_version + " | "), filler(),
                 text("Current user: " + username)}) |
           bold | border;
}

};  // namespace UIHandler
