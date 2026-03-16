cmake_minimum_required(VERSION 4.1.2)
set(CMAKE_CXX_COMPILER g++)
project(ledger-tui VERSION 0.1.0 LANGUAGES C CXX)

find_package(ftxui REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(ledger-tui
    src/main.cpp
    src/app.cpp
    src/auth.cpp
    src/ui_handler.cpp
    src/utils/error_handler.cpp
    src/utils/config.cpp
    src/utils/sha256.cpp
    src/db/database.cpp
    src/db/db_handler.cpp
)

target_link_libraries(ledger-tui
    ftxui::screen
    ftxui::dom
    ftxui::component
    SQLite::SQLite3
    OpenSSL::Crypto
)

add_custom_command(TARGET ledger-tui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:ledger-tui>/resources)
