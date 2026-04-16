#!/usr/bin/env python3
#conding: utf-8
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2

bridge = None
node = None
face_cascade = None

def cam_rgb_callback(msg):
    global bridge, node, face_cascade

    try:
        cv_image = bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")
    except Exception as e:
        node.get_logger().error(f"cv_bridge exception: {e}")
        return

    # face detection and drawing

    gray = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)

    faces = face_cascade.detectMultiScale(
        gray,
        scaleFactor = 1.1,
        minNeighbors = 3,
        minSize = (30, 30)
    )

    for (x, y, w, h) in faces:
        cv2.rectangle(
            cv_image,
            (x, y),
            (x + w, y + h),
            (0, 0, 255),
            2
        )

    # Display the image using OpenCV
    cv2.imshow("Face_Detect", cv_image)
    cv2.waitKey(1)


def main(args=None):
    global bridge, node, face_cascade

    rclpy.init(args=args)

    print("Hello World, I am the cv face detect node![Python]\n")

    node = Node("cv_face_detect_node")

    face_cascade = cv2.CascadeClassifier()

    if not  face_cascade.load("/root/ros2_ws/src/cv_pkg/models/haarcascade_frontalface_default.xml"):
        node.get_logger().error("Could not load face cascade model.")
        return 0


    bridge = CvBridge()

    # create subscriber for RGB image
    node.create_subscription(
        Image,
        "/kinect2/qhd/image_raw",
        cam_rgb_callback,
        1
    )



    # creat a window
    cv2.namedWindow("Face_Detect")
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()



if __name__ == "__main__":
    main()
