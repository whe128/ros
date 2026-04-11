#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

int main(int argc, char const *argv[])
{
    rclcpp::init(argc, argv);


    printf("Hello World, I am the ultra sound node!\n");

    rclcpp::Node::SharedPtr node = rclcpp::Node::make_shared("ultra_sound_node");

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub = node->create_publisher<std_msgs::msg::String>("ultra_sound_topic", 10);

    std_msgs::msg::String msg;
    msg.data = "Hello, I send ultra sound data!";

    rclcpp::Rate loop_rate(1);

    while(rclcpp::ok())
    {
        pub->publish(msg);

        loop_rate.sleep();
    }

    return 0;
}
