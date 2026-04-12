#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <geometry_msgs/msg/twist.hpp>

rclcpp::Node::SharedPtr node;
rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_pub;
int rotate_cycle = 0;

void lidar_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
    float front_distance = msg->ranges[180];
    RCLCPP_INFO(node->get_logger(), "The front distance is: %f meters", front_distance);

    geometry_msgs::msg::Twist vel_msg;

    if (rotate_cycle > 0)
    {
        rotate_cycle--;
        return;
    }

    // too close will collide, turn left
    if (front_distance < 1)
    {
        // give the command to turn left
        vel_msg.angular.z = 2.0;
        rotate_cycle = 9;
    }
    else{
        // give the command to go forward
        vel_msg.linear.x = 1.0;
    }

    vel_pub->publish(vel_msg);
}

int main(int argc, char *argv[])
{
    // init rclcpp
    rclcpp::init(argc, argv);

    // print hello and introduction
    printf("Hello World, I am the lidar node!\n");

    // create node
    node = rclcpp::Node::make_shared("lidar_node");

    // create publisher
    vel_pub = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    // create subscriber
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr sub = node->create_subscription<sensor_msgs::msg::LaserScan>(
        "/scan",
        10,
        lidar_callback
    );

    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}
