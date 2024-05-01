cmake_minimum_required(VERSION 3.10)
project(Sandbox VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directory for engine headers
include_directories(${CMAKE_SOURCE_DIR}/../HrothCore/src)

# Include directory for header files
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add all Source Files
file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Add all Header Files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp
)

# Debug build
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
	set(BIN_TYPE_DIR Debug)
endif()

# Release build
if(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DRELEASE)
	set(BIN_TYPE_DIR Release)
endif()

# Create the Sandbox executable
add_executable(Sandbox ${SOURCE_FILES} ${HEADER_FILES})

# Link with engine library
target_link_directories(Sandbox PRIVATE ${CMAKE_SOURCE_DIR}/../bin/${BIN_TYPE_DIR})
target_link_libraries(Sandbox PRIVATE HrothCore)