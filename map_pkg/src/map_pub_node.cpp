#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    printf("Hello World, I am the map pub node!\n");

    std::shared_ptr<rclcpp::Node> node = std::make_shared<rclcpp::Node>("map_pub_node");

    std::shared_ptr<rclcpp::Publisher<nav_msgs::msg::OccupancyGrid>> pub =
        node->create_publisher<nav_msgs::msg::OccupancyGrid>(
            "/map",
            10
        );

    nav_msgs::msg::OccupancyGrid msg;
    msg.header.frame_id = "map";

    msg.info.resolution = 1.0;
    msg.info.width = 4;
    msg.info.height = 2;
    msg.info.origin.position.x = 0.0;
    msg.info.origin.position.y = 0.0;

    msg.data.resize(msg.info.width * msg.info.height, 0);
    msg.data[0] = 100;
    msg.data[1] = 100;
    msg.data[2] = 0;
    msg.data[3] = -1;


    rclcpp::Rate loop_rate(1);
    while(rclcpp::ok())
    {
        msg.header.stamp = node->get_clock()->now();
        pub->publish(msg);
        loop_rate.sleep();
    }

    return 0;
}
