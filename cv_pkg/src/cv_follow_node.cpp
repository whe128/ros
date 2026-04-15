#include <rclcpp/rclcpp.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/msg/image.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <geometry_msgs/msg/twist.hpp>

// hue
static int iLowH = 10;
static int iHighH = 40;
// saturation
static int iLowS = 90;
static int iHighS = 255;
// value
static int iLowV = 1;
static int iHighV = 255;

std::shared_ptr<rclcpp::Node> node;
geometry_msgs::msg::Twist vel_cmd;
std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::Twist>> vel_pub;

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

    // change RGB to HSV
    cv::Mat imgHSV;
    cv::cvtColor(cv_ptr->image, imgHSV, cv::COLOR_BGR2HSV);

    // make histogram equalization on HSV color space
    std::vector<cv::Mat> hsvSplit;
    cv::split(imgHSV, hsvSplit);
    cv::equalizeHist(hsvSplit[2], hsvSplit[2]);
    cv::merge(hsvSplit, imgHSV);

    // binarization
    cv::Mat imgThresholded;   // storage for the binary image
    cv::inRange(
        imgHSV,
        cv::Scalar(iLowH, iLowS, iLowV),
        cv::Scalar(iHighH, iHighS, iHighV),
        imgThresholded);


    // morphological opening (remove small noise/dots)
    // use the 5x5 kernel to scan the image
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    // open operation,
    morphologyEx(imgThresholded, imgThresholded, cv::MORPH_OPEN, kernel);
    // close operation
    morphologyEx(imgThresholded, imgThresholded, cv::MORPH_CLOSE, kernel);

    // get the center of the object
    int nTargetX = 0;
    int nTargetY = 0;
    int nPixCount = 0;

    int nImageWidth = imgThresholded.cols;
    int nImageHeight = imgThresholded.rows;

    for (int y = 0; y < nImageHeight; y++) {
        for (int x = 0; x < nImageWidth; x++) {
            // white pixel
            if(imgThresholded.data[y*nImageWidth + x] == 255)
            {
                // add value of coordinate
                nTargetX += x;
                nTargetY += y;
                // count the number of pixels
                nPixCount++;
            }
        }
    }

    // use the avage coordinate as the center of the object
    if (nPixCount > 0)
    {
        nTargetX /= nPixCount;
        nTargetY /= nPixCount;
        printf("Target center: (%d, %d)\n", nTargetX, nTargetY);

        // draw the coordinate
        cv::line(
            cv_ptr->image,
            cv::Point(nTargetX - 10, nTargetY),
            cv::Point(nTargetX + 10, nTargetY),
            cv::Scalar(255, 0, 0)
        );

        cv::line(
            cv_ptr->image,
            cv::Point(nTargetX, nTargetY - 10),
            cv::Point(nTargetX, nTargetY + 10),
            cv::Scalar(255, 0, 0)
        );

        // control the robot to follow the object
        int error_x = nImageWidth / 2 - nTargetX;
        int error_y = nImageHeight / 2 - nTargetY;

        float vel_rotate = error_x * 0.005;
        float vel_forward = error_y * 0.003;

        vel_cmd.linear.x = vel_forward;
        vel_cmd.linear.y = 0;
        vel_cmd.linear.z = 0;
        vel_cmd.angular.x = 0;
        vel_cmd.angular.y = 0;
        vel_cmd.angular.z = vel_rotate;
    }
    else
    {
        printf("No target detected!\n");

        vel_cmd.linear.x = 0;
        vel_cmd.linear.y = 0;
        vel_cmd.linear.z = 0;
        vel_cmd.angular.x = 0;
        vel_cmd.angular.y = 0;
        vel_cmd.angular.z = 0;
    }

    vel_pub->publish(vel_cmd);
    printf("robot velocity: linear.x = %.2f, angular.z = %.2f\n", vel_cmd.linear.x, vel_cmd.angular.z);


    cv::imshow("RGB", cv_ptr->image);
    cv::imshow("Result", imgThresholded);

    cv::waitKey(5);
}

int main(int argc, char const *argv[])
{
    rclcpp::init(argc, argv);

    printf("Hello World, I am the cv follow node!\n");

    node = rclcpp::Node::make_shared("cv_follow_node");

    vel_pub = node->create_publisher<geometry_msgs::msg::Twist>(
            "/cmd_vel",
            10
        );

    std::shared_ptr<rclcpp::Subscription<sensor_msgs::msg::Image>> rgb_sub =
        node->create_subscription<sensor_msgs::msg::Image>(
            "/kinect2/qhd/image_raw",
            1,
            Cam_RGB_Callback
        );

    // thresholdig window
    cv::namedWindow("Threshold", cv::WINDOW_AUTOSIZE);
    // create trackbars in "Threshold" window
    // Hue (0 - 179)
    cv::createTrackbar("LowH", "Threshold", &iLowH, 179);
    cv::createTrackbar("HighH", "Threshold", &iHighH, 179);

    // Saturation (0 - 255)
    cv::createTrackbar("LowS", "Threshold", &iLowS, 255);
    cv::createTrackbar("HighS", "Threshold", &iHighS, 255);

    // Value (0 - 255)
    cv::createTrackbar("LowV", "Threshold", &iLowV, 255);
    cv::createTrackbar("HighV", "Threshold", &iHighV, 255);

    cv::namedWindow("RGB");
    cv::namedWindow("Result");

    rclcpp::spin(node);

    cv::destroyAllWindows();
    rclcpp::shutdown();
    return 0;
}


