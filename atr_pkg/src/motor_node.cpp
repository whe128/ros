#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

rclcpp::Node::SharedPtr node;

void ultra_sound_callback(const std_msgs::msg::String msg)
{
    RCLCPP_INFO(node->get_logger(),
                "I am the motor node and received a message: %s",
                msg.data.c_str());
}

void aml_callback(const std_msgs::msg::String msg)
{
    RCLCPP_WARN(node->get_logger(),
                "I am the motor node and received a message: %s",
                msg.data.c_str());
}


int main(int argc, char const *argv[])
{
    rclcpp::init(argc, argv);

    printf("Hello World, I am the motor node!\n");

    node = rclcpp::Node::make_shared("motor_node");

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_ultra_sound = node->create_subscription<std_msgs::msg::String>("ultra_sound_topic", 10, ultra_sound_callback);
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_aml = node->create_subscription<std_msgs::msg::String>("aml_topic", 10, aml_callback);


    rclcpp::spin(node);

    return 0;
}
