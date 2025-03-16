// To use this file, it should go into the crazyswarm2 ROS workspace.
// In the workspace it should be located at /ros2_ws/src/crazyswarm2/crazyflie/src/trace.cpp
// Add the following lines to the CMakelists.txt file in the same directory:
// add_executable(trace
//   src/trace.cpp
// )
// ament_target_dependencies(trace
//   rclcpp
//   crazyflie_interfaces
// )
// ^ Undernearth the other add_executable. Then add 'trace' to the list of install(TARGETS ...).
// Add the following lines to launch.py after the creation of the other nodes:
// Node(
//     package = 'crazyflie',
//     executable = 'trace')
#include "rclcpp/rclcpp.hpp"

class Trace : public rclcpp::Node
{
public:
    Trace()
        : Node("trace_node")
    {
        RCLCPP_INFO(this->get_logger(), "My ROS 2 node has been initialized!");
    }

private:
    // Add any private members or methods here
};

int main(int argc, char *argv[])
{
    // Initialize the ROS 2 system
    rclcpp::init(argc, argv);

    // Create an instance of node
    auto node = std::make_shared<Trace>();

    // Spin the node to keep it running and processing callbacks
    rclcpp::spin(node);

    // Shutdown the ROS 2 system
    rclcpp::shutdown();

    return 0;
}