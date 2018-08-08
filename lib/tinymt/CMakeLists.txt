cmake_minimum_required(VERSION 2.8)

project(tinymt)

add_library(tinymt tinymt32.c)

target_include_directories(tinymt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})