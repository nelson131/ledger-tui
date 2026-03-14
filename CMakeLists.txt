cmake_minimum_required(VERSION 4.1.2)
set(CMAKE_CXX_COMPILER g++)
project(ledger-tui VERSION 0.1.0 LANGUAGES C CXX)

find_package(ftxui REQUIRED)

add_executable(ledger-tui
    src/main.cpp
    src/app.cpp
    src/utils/error_handler.cpp
)

target_link_libraries(ledger-tui
    ftxui::screen
    ftxui::dom
    ftxui::component
    SQLite::SQLite3
)

add_custom_command(TARGET ledger-tui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:ledger-tui>/resources)
