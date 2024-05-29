cmake_minimum_required(VERSION 3.10)

add_library(entt STATIC include/entt.hpp src/entt.cpp)
target_include_directories(entt PUBLIC include)