# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(coveragePP VERSION 1.0)

# Find Doxygen library
find_package(Doxygen REQUIRED)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SRC_DIR src)

# Main source file
set(MAIN_SRC ${SRC_DIR}/main.cpp)

# Supporting source files (all .cpp files except main.cpp)
file(GLOB SUPPORTING_SRCS ${SRC_DIR}/*.cpp)
list(REMOVE_ITEM SUPPORTING_SRCS ${MAIN_SRC})

add_library(coveragePP ${MAIN_SRC} ${SUPPORTING_SRCS})

# Set output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Create the executable
add_executable(runCPP ${MAIN_SRC} ${SUPPORTING_SRCS})
target_link_libraries(runCPP PRIVATE coveragePP)

# Clean target (optional, since CMake generates a clean target by default)
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${EXECUTABLE_OUTPUT_PATH}
)

# Enable Testing
enable_testing()
add_subdirectory(tests)

# Generate Doxygen documentation
if(${DOXYGEN_FOUND})
    doxygen_add_docs(doxygen 
    ${PROJECT_SOURCE_DIR}/include/ 
    ${PROJECT_SOURCE_DIR}/src/ 
    ALL
)
endif()
