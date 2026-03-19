#include "app.hpp"

#include <ftxui/dom/elements.hpp>
#include <iostream>

Application::Application() : name(""), version(""), auth(&db) {}

Error Application::init(const std::string& name, const std::string& version) {
    this->name = name;
    this->version = version;

    Error err = DBHandler::init_tables(&db);
    if (err.code != 1) {
        return err;
    }

    return {NONE};
}

Error Application::welcome() {
    auto        screen = ScreenInteractive::Fullscreen();
    std::string username;
    std::string password;
    std::string status = "";

    auto username_input = Input(&username, "username");
    auto password_input = Input(&password, "password");

    auto login_button = Button("Login", [&] {
        Error login_res = auth.login_user(username, password);
        if (login_res.code == 2) {
            status = "success: login";
            screen.ExitLoopClosure()();
        } else {
            status = login_res.message;
        }
    });

    auto reg_button = Button("Register", [&] {
        Error reg_res = auth.register_user(username, password);
        if (reg_res.code == 2) {
            status = "success: register";
        } else {
            status = reg_res.message;
        }
    });

    auto buttons = Container::Horizontal({login_button, reg_button});
    auto inputs =
        Container::Vertical({username_input, password_input, buttons});

    auto renderer = Renderer(inputs, [&] {
        return vbox({text(name) | bold | center, text(version) | bold | center,
                     separator(),

                     hbox({text("Username: ") | size(WIDTH, EQUAL, 12),
                           username_input->Render() | flex}),

                     hbox({text("Password: ") | size(WIDTH, EQUAL, 12),
                           password_input->Render() | flex}),

                     text(""),

                     hbox({filler(), login_button->Render(), text(" "),
                           reg_button->Render(), filler()}),

                     text(""), separator(),

                     hbox({text("Status: "),
                           text(status) | color(Color::Red)})}) |
               border | size(WIDTH, GREATER_THAN, 40) | center;
    });

    screen.Loop(renderer);

    return {NONE};
}

void Application::dashboard() {
    auto screen = ScreenInteractive::Fullscreen();

    auto header = UIHandler::make_header(name, version, auth.get_username());
    auto top_menu = UIHandler::make_top_menu();

    auto layout = Container::Vertical({top_menu});

    auto renderer =
        Renderer(layout, [&] { return vbox({header, layout->Render()}); });

    screen.Loop(renderer);
}
