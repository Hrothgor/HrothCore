cmake_minimum_required(VERSION 3.10)
project(HrothCore VERSION 0.1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add all Source Files
file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# Add all Header Files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
)

# Debug build
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif()

# Release build
if(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DRELEASE)
endif()

# Create library
add_library(HrothCore STATIC ${SOURCE_FILES} ${HEADER_FILES})

# Include directory for header files
target_include_directories(HrothCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

###########################################
# EXTERNAL
###########################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw)

target_link_libraries(HrothCore PUBLIC 
						glfw 
						glad)

###########################################