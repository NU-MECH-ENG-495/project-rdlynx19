#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <visualization_msgs/msg/marker_array.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <builtin_interfaces/msg/duration.hpp>
#include <memory>
#include <string>
#include <vector>

/**
 * @class Trace
 * @brief A ROS 2 node that traces the positions of Crazyflie drones and publishes them as visualization markers.
 *
 * This node listens to TF transforms for a set of Crazyflie drones, creates visualization markers for their positions,
 * and publishes them as a MarkerArray message.
 */
class Trace : public rclcpp::Node
{
public:
    /**
     * @brief Constructor for the Trace node.
     *
     * Initializes the node, sets up the TF buffer and listener, creates a publisher for visualization markers,
     * and starts a timer to periodically update the markers.
     */
    Trace()
        : Node("trace_node"), tf_buffer_(this->get_clock()), tf_listener_(tf_buffer_)
    {
        RCLCPP_INFO(this->get_logger(), "My ROS 2 node has been initialized!");
        rclcpp::QoS qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)).reliable().durability_volatile();
        marker_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>("trace", qos_profile);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&Trace::timer_callback, this));
    }

private:
    visualization_msgs::msg::MarkerArray marker_array; ///< Marker array to store the visualization markers.
    int64_t markerCount = 0;                           ///< Counter to keep track of the number of markers created.

    /**
     * @brief Timer callback function.
     *
     * This function is called periodically by the timer. It looks up the TF transforms for each Crazyflie drone,
     * creates a visualization marker for its position, and adds it to the marker array. The marker array is then published.
     */
    void timer_callback()
    {
        std::vector<std::string> cf_names = {"cf231", "cf5", "cf7", "cf9"}; // Crazyflie Names

        for (size_t i = 0; i < cf_names.size(); ++i)
        {
            try
            {
                // Attempt tf lookup
                geometry_msgs::msg::TransformStamped transform = tf_buffer_.lookupTransform("world", cf_names[i], tf2::TimePointZero);

                // Create a marker for the drone's position
                visualization_msgs::msg::Marker marker;
                marker.header.frame_id = "world";
                marker.header.stamp = this->now();
                marker.ns = std::to_string(markerCount);
                marker.id = markerCount;
                marker.type = visualization_msgs::msg::Marker::SPHERE;
                marker.action = visualization_msgs::msg::Marker::ADD;
                marker.pose.position.x = transform.transform.translation.x;
                marker.pose.position.y = transform.transform.translation.y;
                marker.pose.position.z = transform.transform.translation.z;
                marker.pose.orientation = transform.transform.rotation;
                marker.lifetime = rclcpp::Duration(0, 0); // Forever
                marker.scale.x = 0.1;
                marker.scale.y = 0.1;
                marker.scale.z = 0.1;
                marker.color.r = 1.0;
                marker.color.g = 0.0;
                marker.color.b = 0.0;
                marker.color.a = 1.0;

                marker_array.markers.push_back(marker);

                markerCount++;
            }
            catch (tf2::TransformException &ex)
            {
                RCLCPP_ERROR(this->get_logger(), "Failed to lookup tf for %s, ex: %s", cf_names[i].c_str(), ex.what());
            }
        }
        marker_pub_->publish(marker_array);
    }

    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_; ///< Publisher for the marker array.
    tf2_ros::Buffer tf_buffer_;                                                     ///< TF buffer to store transforms.
    tf2_ros::TransformListener tf_listener_;                                        ///< TF listener to receive transforms.
    rclcpp::TimerBase::SharedPtr timer_;                                            ///< Timer to periodically trigger the callback.
};

/**
 * @brief Main function to initialize and run the Trace node.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return int Exit status.
 */
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