#!/usr/bin/env python3
#conding: utf-8
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
import numpy as np

bridge = None
node = None

iLowH, iHighH = 10, 40;
iLowS, iHighS = 90, 255;
iLowV, iHighV = 1, 255;

def cam_rgb_callback(msg):
    global node, bridge, iLowH, iHighH, iLowS, iHighS, iLowV, iHighV

    try:
        cv_image = bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")
    except Exception as e:
        node.get_logger().error(f"cv_bridge exception: {e}")
        return

    # Convert the image to HSV color space
    hsv_img = cv2.cvtColor(cv_image, cv2.COLOR_BGR2HSV)

    # equalize V channel
    imgThresholded = cv2.inRange(
        hsv_img,
        (iLowH, iLowS, iLowV),
        (iHighH, iHighS, iHighV)
    )

    # morphology
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
    imgThresholded = cv2.morphologyEx(imgThresholded, cv2.MORPH_OPEN, kernel)
    imgThresholded = cv2.morphologyEx(imgThresholded, cv2.MORPH_CLOSE, kernel)

    # centroid
    y_whole, x_whole = np.where(imgThresholded == 255)

    if len(x_whole) > 0 and len(y_whole) > 0:
        x_center = int(np.mean(x_whole))
        y_center = int(np.mean(y_whole))
        print(f"Target center: ({x_center}, {y_center})")

        # draw cross lines
        cv2.line(
            cv_image,
            (x_center - 10, y_center),
            (x_center + 10, y_center),
            (255, 0, 0)
        )

        cv2.line(
            cv_image,
            (x_center, y_center - 10),
            (x_center, y_center + 10),
            (255, 0, 0)
        )
    else:
        print("No target detected")

    # Display the image using OpenCV
    cv2.imshow("RGB", cv_image)
    cv2.imshow("HSV", hsv_img)
    cv2.imshow("Result", imgThresholded)
    cv2.waitKey(5)


def main(args=None):
    global node, bridge, iLowH, iHighH, iLowS, iHighS, iLowV, iHighV

    rclpy.init(args=args)

    print("Hello World, I am the cv hsv node![Python]\n")

    node = Node("cv_hsv_node")
    bridge = CvBridge()

    # create subscriber for RGB image
    node.create_subscription(
        Image,
        "/kinect2/qhd/image_raw",
        cam_rgb_callback,
        1
    )

    # threshold values for HSV
    cv2.namedWindow("Threshold")

    cv2.createTrackbar("LowH", "Threshold", iLowH, 179, lambda x: globals().update(iLowH=x))
    cv2.createTrackbar("HighH", "Threshold", iHighH, 179, lambda x: globals().update(iHighH=x))

    cv2.createTrackbar("LowS", "Threshold", iLowS, 255, lambda x: globals().update(iLowS=x))
    cv2.createTrackbar("HighS", "Threshold", iHighS, 255, lambda x: globals().update(iHighS=x))

    cv2.createTrackbar("LowV", "Threshold", iLowV, 255, lambda x: globals().update(iLowV=x))
    cv2.createTrackbar("HighV", "Threshold", iHighV, 255, lambda x: globals().update(iHighV=x))


    # creat windows
    cv2.namedWindow("RGB")
    cv2.namedWindow("HSV")
    cv2.namedWindow("Result")


    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()



if __name__ == "__main__":
    main()
