cmake_minimum_required(VERSION 3.8)
project(cfVisualizer)

# Find Doxygen library
find_package(Doxygen REQUIRED)

# Default to C++17
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

# Add executable
add_executable(trace src/trace.cpp)

# Link dependencies
ament_target_dependencies(trace
  rclcpp
  tf2_ros
  visualization_msgs
  geometry_msgs
)

# Install the executable
install(TARGETS
  trace
  DESTINATION lib/${PROJECT_NAME}
)

# Linting and testing
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# Export package
ament_package()

# Generate Doxygen documentation
if(${DOXYGEN_FOUND})
    doxygen_add_docs(doxygen 
    ${PROJECT_SOURCE_DIR}/include/ 
    ${PROJECT_SOURCE_DIR}/src/ 
    ALL
)
endif()