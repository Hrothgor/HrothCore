cmake_minimum_required(VERSION 3.10)

add_library(imguizmo STATIC include/ImGuizmo.h src/ImGuizmo.cpp)
target_link_libraries(imguizmo PUBLIC imgui)
target_include_directories(imguizmo PUBLIC include)