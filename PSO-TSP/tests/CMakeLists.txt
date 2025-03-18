cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1    
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(unit_tests
    unit/testPSO.cpp
    unit/testCity.cpp
)

target_link_libraries(unit_tests
    PRIVATE
    GTest::gtest 
    GTest::gtest_main
    psoDefinition   
)

enable_testing()

add_test(NAME unit_tests COMMAND unit_tests)
gtest_discover_tests(unit_tests)