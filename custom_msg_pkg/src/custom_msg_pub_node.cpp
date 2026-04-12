#include <rclcpp/rclcpp.hpp>
#include <custom_msgs/msg/custom_msg.hpp>

int main(int argc, char const *argv[])
{
    rclcpp::init(argc, argv);


    printf("Hello World, I am the custom msg pub node!\n");

    rclcpp::Node::SharedPtr node = rclcpp::Node::make_shared("custom_msg_pub_node");

    rclcpp::Publisher<custom_msgs::msg::CustomMsg>::SharedPtr pub = node->create_publisher<custom_msgs::msg::CustomMsg>(
        "custom_msg_topic",
        10
    );

    custom_msgs::msg::CustomMsg msg;

    msg.grade = "A+";
    msg.star = 50;
    msg.data = "Hello, I send custom msg data!";

    rclcpp::Rate loop_rate(1);

    while(rclcpp::ok())
    {
        pub->publish(msg);

        loop_rate.sleep();
    }

    return 0;
}
