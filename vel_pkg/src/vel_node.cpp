#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    printf("Hello World, I am the vel node!\n");

    rclcpp::Node::SharedPtr node = rclcpp::Node::make_shared("vel_node");
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    geometry_msgs::msg::Twist vel_msg;

    vel_msg.linear.x = 0.1;
    vel_msg.linear.y = 0.0;
    vel_msg.linear.z = 0.0;
    vel_msg.angular.x = 0.0;
    vel_msg.angular.y = 0.0;
    vel_msg.angular.z = 0.1;

    rclcpp::Rate loop_rate(30);

    while(rclcpp::ok())
    {
        pub->publish(vel_msg);

        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}
