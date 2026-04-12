#include <rclcpp/rclcpp.hpp>
#include <custom_msgs/msg/custom_msg.hpp>

rclcpp::Node::SharedPtr node;

void custom_msg_callback(const std::shared_ptr<custom_msgs::msg::CustomMsg> msg)
{
    RCLCPP_INFO(node->get_logger(),
                "I am the custom msg sub node and received a message:");
    RCLCPP_WARN(node->get_logger(),
                "grade: %s",
                msg->grade.c_str());
    RCLCPP_WARN(node->get_logger(),
                "start: %d",
                msg->star);
    RCLCPP_INFO(node->get_logger(),
                "data: %s",
                msg->data.c_str());
}



int main(int argc, char const *argv[])
{
    rclcpp::init(argc, argv);

    printf("Hello World, I am the custom msg sub node!\n");

    node = rclcpp::Node::make_shared("custom_msg_sub_node");

    rclcpp::Subscription<custom_msgs::msg::CustomMsg>::SharedPtr sub = node->create_subscription<custom_msgs::msg::CustomMsg>(
        "custom_msg_topic",
        10,
        custom_msg_callback);


    rclcpp::spin(node);

    return 0;
}
