#include "app.hpp"

#include <iostream>

Application::Application()
    : name(""),
      version(""),
      run(false),
      screen(ftxui::ScreenInteractive::TerminalOutput()) {}

Error Application::init(const std::string& name, const std::string& version) {
    this->name = name;
    this->version = version;

    Error init_err = database.init("resources/ledger.db");
    if (init_err.code != 1) {
        return init_err;
    }

    Error exec_err = database.execute(R"(
    CREATE TABLE IF NOT EXISTS users_data(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE,
    password_sha256 TEXT
    );
    )");
    if (exec_err.code != 1) {
        return exec_err;
    }

    return {NONE};
}

Error Application::welcome() {
    std::string username;
    std::string password;
    std::string status = "";

    auto username_input = ftxui::Input(&username, "username");
    auto password_input = ftxui::Input(&password, "password");

    auto login_button =
        ftxui::Button("Login", [&] { status = "Login pressed"; });

    auto reg_button =
        ftxui::Button("Register", [&] { status = "Register pressed"; });

    auto buttons = ftxui::Container::Horizontal({login_button, reg_button});
    auto inputs =
        ftxui::Container::Vertical({username_input, password_input, buttons});

    auto renderer = Renderer(inputs, [&] {
        return ftxui::vbox(
                   {ftxui::text("ledger-tui") | ftxui::bold | ftxui::center,
                    ftxui::separator(),

                    ftxui::hbox(
                        {ftxui::text("Username: ") |
                             ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 12),
                         username_input->Render() | ftxui::flex}),

                    ftxui::hbox(
                        {ftxui::text("Password: ") |
                             ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 12),
                         password_input->Render() | ftxui::flex}),

                    ftxui::text(""),

                    ftxui::hbox({ftxui::filler(), login_button->Render(),
                                 ftxui::text(" "), reg_button->Render(),
                                 ftxui::filler()}),

                    ftxui::text(""), ftxui::separator(),

                    ftxui::hbox(
                        {ftxui::text("Status: "),
                         ftxui::text(status) | color(ftxui::Color::Red)})}) |
               ftxui::border |
               ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 40) |
               ftxui::center;
    });

    screen.Loop(renderer);

    return {NONE};
}

void Application::render() {}

void Application::handle_input() {}

const bool& Application::is_running() const { return run; }
