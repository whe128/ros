#include <rclcpp/rclcpp.hpp>

int main(int argc, char const *argv[])
{
    rclcpp::init(argc, argv);


    printf("Hello World, I like you!\n");

    while(rclcpp::ok())
    {
        printf("yes\n");
    }
    
    return 0;
}
