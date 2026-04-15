#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

std::shared_ptr<rclcpp::Node> node;

void NavResultCallback(const std_msgs::msg::String::SharedPtr msg){
    RCLCPP_WARN(node->get_logger(), "Received Result: %s", msg->data.c_str());
}


int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);

    printf("Hello World, I am the wp node!\n");

    node = rclcpp::Node::make_shared("wp_node");

    std::shared_ptr<rclcpp::Publisher<std_msgs::msg::String>> waypoint_pub =
        node->create_publisher<std_msgs::msg::String>("waterplus/navi_waypoint", 10);

    std::shared_ptr<rclcpp::Subscription<std_msgs::msg::String>> result_sub =
        node->create_subscription<std_msgs::msg::String>("waterplus/navi_result", 10, NavResultCallback);

    // sleep
    rclcpp::sleep_for(std::chrono::milliseconds(1000));

    std_msgs::msg::String msg;

    if(argc > 1)
        msg.data = argv[1];
    else
        msg.data = "1";

    waypoint_pub->publish(msg);

    RCLCPP_INFO(node->get_logger(), "Send waypoint name : %s",msg.data.c_str());
    rclcpp::spin(node);

    return 0;
}



