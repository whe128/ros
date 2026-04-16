#include <rclcpp/rclcpp.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/msg/image.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect.hpp>

rclcpp::Node::SharedPtr node;
cv::CascadeClassifier face_cascade;
cv::Mat gray;
std::vector<cv::Rect> faces;

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

    cv::cvtColor(cv_ptr->image, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);


    // detect face and draw rectangle
    face_cascade.detectMultiScale(
        gray,
        faces,
        1.1,          // scale factor
        3,            // min neighbors
        0,
        cv::Size(30, 30)
    );

    if (faces.size() > 0)
    {
        for (const auto& face : faces)
        {
            cv::rectangle(
                cv_ptr->image,
                face,
                cv::Scalar(0, 0, 255),
                2
            );

        }
    }

    cv::imshow("Face_Detect", cv_ptr->image);
    cv::waitKey(1);
}

int main(int argc, char const *argv[])
{
    rclcpp::init(argc, argv);

    printf("Hello World, I am the cv face detect node!\n");

    node = rclcpp::Node::make_shared("cv_face_detect_node");

    if(!face_cascade.load("/root/ros2_ws/src/cv_pkg/models/haarcascade_frontalface_default.xml"))
    {
        RCLCPP_ERROR(node->get_logger(), "Could not load face cascade model.");
        return 0;
    }

    std::shared_ptr<rclcpp::Subscription<sensor_msgs::msg::Image>> rgb_sub =
        node->create_subscription<sensor_msgs::msg::Image>(
            "/kinect2/qhd/image_raw",
            1,
            Cam_RGB_Callback
        );

    cv::namedWindow("Face_Detect");
    rclcpp::spin(node);
    cv::destroyAllWindows();
    rclcpp::shutdown();

    return 0;
}
