cmake_minimum_required(VERSION 3.24)

project(flashcli)

add_executable(flashcli)

target_sources(flashcli PRIVATE
    src/main.cxx
    src/SeedKey.cxx
    src/SeedKey.hxx
    )

target_link_libraries(flashcli PRIVATE
    diagnosis-tester
    uds-secacc
    )