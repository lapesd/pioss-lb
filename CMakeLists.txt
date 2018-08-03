cmake_minimum_required(VERSION 3.9)

project(pioss)

include_directories(., ./lib/tinymt)

file(GLOB SOURCES "./*.c" "./lib/tinymt/*.c")

add_executable(pioss ${SOURCES})