#include "ui_handler.hpp"

#include <ftxui/dom/elements.hpp>
#include <iostream>

namespace UIHandler {

Element make_header(const std::string& app_name, const std::string& app_version,
                    const std::string& username) {
    return hbox({text(app_name + " | "), text(app_version + " | "), filler(),
                 text("Current user: " + username)}) |
           bold | border;
}

Component make_top_menu() {
    auto dashboard_button = Button("Dashboard", [&] {});
    auto transactions_button = Button("Transactions", [] {});
    auto stats_button = Button("Statistics", [&] {});

    auto container = Container::Horizontal(
        {dashboard_button, transactions_button, stats_button});

    auto render_button = [](Component& button, const std::string& label) {
        return Renderer(button, [button = button, label] {
            auto e = text(label);
            if (button->Focused()) {
                e = e | color(Color::Cyan) | bold;
            }
            return e;
        });
    };

    return Container::Horizontal(
        {render_button(dashboard_button, "  Dashboard  "),
         render_button(transactions_button, "  Transactions  "),
         render_button(stats_button, "  Statistics  ")});
};

};  // namespace UIHandler
