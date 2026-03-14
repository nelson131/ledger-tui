#include "app.hpp"

#include <iostream>

Application::Application() : run(false) {}

Error Application::init() {}

void Application::render() {}

void Application::handle_input() {}

const bool& Application::is_running() const { return run; }
