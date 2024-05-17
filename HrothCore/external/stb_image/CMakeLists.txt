cmake_minimum_required(VERSION 3.10)

add_library(stb_image STATIC include/stb_image.h src/stb_image.c)
target_include_directories(stb_image PUBLIC include)