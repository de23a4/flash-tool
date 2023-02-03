cmake_minimum_required(VERSION 3.24)
project(flash_tool)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(platform/linux/lib/diagnosis-tester)
add_subdirectory(platform/linux/lib/queue)
add_subdirectory(platform/linux/lib/utils)
add_subdirectory(platform/linux/bin/flashcli)
