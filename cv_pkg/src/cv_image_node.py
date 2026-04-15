#!/usr/bin/env python3
#conding: utf-8
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2

bridge = None
node = None

def cam_rgb_callback(msg):
    global bridge, node

    try:
        cv_image = bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")
    except Exception as e:
        node.get_logger().error(f"cv_bridge exception: {e}")
        return

    # Display the image using OpenCV
    cv2.imshow("RGB", cv_image)
    cv2.waitKey(1)


def main(args=None):
    global bridge, node

    rclpy.init(args=args)

    print("Hello World, I am the cv image node![Python]\n")

    node = Node("cv_image_node")
    bridge = CvBridge()

    # create subscriber for RGB image
    node.create_subscription(
        Image,
        "/kinect2/qhd/image_raw",
        cam_rgb_callback,
        1
    )

    # creat a window
    cv2.namedWindow("RGB")
    cv2.namedWindow("test")

    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()



if __name__ == "__main__":
    main()
