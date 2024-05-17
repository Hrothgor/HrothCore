cmake_minimum_required(VERSION 3.10)
project(Sandbox VERSION 0.1.0 LANGUAGES CXX)

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
	set(BIN_TYPE_DIR Debug)
else()
	set(BIN_TYPE_DIR Release)
endif()

###########################################
# SET WORKING DIRECTORY FOR ASSETS
###########################################
add_definitions(-DASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../HrothCore")
###########################################

# Create the Sandbox executable
add_executable(Sandbox ${SOURCE_FILES} ${HEADER_FILES})

# Include directory for header files
target_include_directories(Sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

###########################################
# EXTERNAL
###########################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../HrothCore ${CMAKE_CURRENT_BINARY_DIR}/HrothCore)

target_link_libraries(Sandbox PRIVATE
						HrothCore)

###########################################

