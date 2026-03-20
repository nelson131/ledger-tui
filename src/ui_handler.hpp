#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <functional>
#include <string>

using namespace ftxui;

namespace UIHandler {

Element make_header(const std::string& app_name, const std::string& app_version,
                    const std::string& username);

Component make_top_menu(ScreenInteractive*    screen,
                        std::function<void()> dashboard,
                        std::function<void()> transactions,
                        std::function<void()> stats);

Component make_transactions_table();

};  // namespace UIHandler
