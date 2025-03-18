# Include FetchContent for downloading GoogleTest
include(FetchContent)
FetchContent_Declare(
googletest
URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Configure GoogleTest
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
# Add library
# add_library(${PROJECT_NAME} unit/coveragePPTests.cpp)
# Create test executable
add_executable(unit_tests
unit/mainTests.cpp
)
# Link against GoogleTest and your main library
target_link_libraries(unit_tests
PRIVATE
GTest::gtest
GTest::gtest_main
${PROJECT_NAME}
)
# Add tests
add_test(NAME unit_tests COMMAND unit_tests)
# Discover and run tests
gtest_discover_tests(unit_tests)