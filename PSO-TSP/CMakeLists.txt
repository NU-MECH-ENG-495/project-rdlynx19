cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
project(PSO-TSP)

find_package(Doxygen REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(psoDefinition
    src/cityImplementation.cpp 
    src/particleImplementation.cpp 
    src/psoImplementation.cpp 
    src/utils.cpp
)

target_include_directories(psoDefinition PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(pso 
    src/mainSim.cpp
)

target_link_libraries(pso PRIVATE psoDefinition)

if(${DOXYGEN_FOUND})
    doxygen_add_docs(doxygen 
    ${PROJECT_SOURCE_DIR}/include/ 
    ${PROJECT_SOURCE_DIR}/src/ 
    ALL
)
endif()



enable_testing()

add_subdirectory(tests)