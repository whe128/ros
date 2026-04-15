#include <rclcpp/rclcpp.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/msg/image.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

rclcpp::Node::SharedPtr node;


void Cam_RGB_Callback(const sensor_msgs::msg::Image::SharedPtr msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
        RCLCPP_ERROR(node->get_logger(), "cv_bridge exception: %s", e.what());
        return;
    }

    cv::imshow("RGB", cv_ptr->image);
    cv::waitKey(1);
}

int main(int argc, char const *argv[])
{
    rclcpp::init(argc, argv);

    printf("Hello World, I am the cv image node!\n");

    node = rclcpp::Node::make_shared("cv_image_node");

    std::shared_ptr<rclcpp::Subscription<sensor_msgs::msg::Image>> rgb_sub =
        node->create_subscription<sensor_msgs::msg::Image>(
            "/kinect2/qhd/image_raw",
            1,
            Cam_RGB_Callback
        );

    cv::namedWindow("RGB");
    rclcpp::spin(node);
    cv::destroyAllWindows();
    rclcpp::shutdown();

    return 0;
}
