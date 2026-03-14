#include "app.hpp"

#include <iostream>

Application::Application() : run(false), screen(nullptr) {}

Error Application::login() {}

void Application::render() {}

void Application::handle_input() {}

const bool& Application::is_running() const { return run; }
