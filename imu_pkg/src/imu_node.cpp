#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <geometry_msgs/msg/twist.hpp>

std::shared_ptr<rclcpp::Node> node;
std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::Twist>> vel_pub;

void imu_callback(const sensor_msgs::msg::Imu::SharedPtr msg)
{
    // print the orientation of the robot

    // invalid data, do nothing
    if (msg->orientation_covariance[0] < 0)
    {
        return;
    }

    tf2::Quaternion tf2_quaternion;
    tf2_quaternion.setX(msg->orientation.x);
    tf2_quaternion.setY(msg->orientation.y);
    tf2_quaternion.setZ(msg->orientation.z);
    tf2_quaternion.setW(msg->orientation.w);

    double roll, pitch, yaw;

    // get the rad of roll, pitch and yaw
    // rotate order,(Extrinsic)
    // first roll around world x, roll
    // then pitch around world y, pitch
    // finally yaw around world z, yaw

    // rotate order,(Intrinsic)
    // first roll around body z, yaw
    // then pitch around body y, pitch
    // finally yaw around body x, roll
    tf2::Matrix3x3(tf2_quaternion).getRPY(roll, pitch, yaw);

    // change rad to degree
    roll = roll * 180 / M_PI;
    pitch = pitch * 180 / M_PI;
    yaw = yaw * 180 / M_PI;


    RCLCPP_INFO(node->get_logger(),
                "The orientation of the robot is: roll: %.2f, pitch: %.2f, yaw: %.2f",
                roll, pitch, yaw);


    // course control
    double target_yaw = 90.0;
    double yaw_error = target_yaw - yaw;
    double kp = 0.01;

    geometry_msgs::msg::Twist cmd_vel;
    cmd_vel.angular.z = kp * yaw_error;
    vel_pub->publish(cmd_vel);
}


int main(int argc, char *argv[])
{
    // init ROS2
    rclcpp::init(argc, argv);

    // print hello and introduction
    printf("Hello World, I am the IMU node!\n");

    // create node
    node = std::make_shared<rclcpp::Node>("imu_node");

    // create subscriber
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr sub = node->create_subscription<sensor_msgs::msg::Imu>(
        "/imu/data",
        10,
        imu_callback
    );

    // create publisher
    vel_pub = node->create_publisher<geometry_msgs::msg::Twist>(
        "/cmd_vel",
        10
    );

    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}
