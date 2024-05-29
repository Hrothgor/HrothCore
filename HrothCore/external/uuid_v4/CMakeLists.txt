cmake_minimum_required(VERSION 3.10)

add_library(uuid_v4 STATIC include/uuid_v4.h src/uuid_v4.cpp)
target_include_directories(uuid_v4 PUBLIC include)