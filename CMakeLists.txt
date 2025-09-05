cmake_minimum_required(VERSION 3.10)
project(SixNimmtContest)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type to Release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Include directories
include_directories(include)

# Source files
set(GAME_SOURCES
    src/game.cpp
    src/random_agent.cpp
    src/lowest_card_first_agent.cpp
    src/highest_card_first_agent.cpp
    src/bulls_heads_first_agent.cpp
)

# Create the main executable
add_executable(sixnimmt_contest src/contest.cpp ${GAME_SOURCES})

# Create a library for easy linking
add_library(sixnimmt_lib ${GAME_SOURCES})

# Example: Create a simple test executable
add_executable(test_game src/test_game.cpp ${GAME_SOURCES})
target_link_libraries(test_game sixnimmt_lib)

# Installation
install(TARGETS sixnimmt_contest DESTINATION bin)
install(TARGETS sixnimmt_lib DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
