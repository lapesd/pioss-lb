cmake_minimum_required(VERSION 2.8)

project(pioss)

add_subdirectory(./lib/tinymt)

file(GLOB SOURCES "./*.c")

add_executable(pioss ${SOURCES})

target_link_libraries(pioss LINK_PUBLIC tinymt)