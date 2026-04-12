#!/usr/bin/env python3
#conding: utf-8


import rclpy
from custom_msgs.msg import CustomMsg

node = None

def custom_msg_callback(msg):
    global node
    node.get_logger().info("I am the custom msg sub node and received a message: ")
    node.get_logger().warn(f"grade: {msg.grade}")
    node.get_logger().warn(f"star: {msg.star}")
    node.get_logger().info(f"data: {msg.data}")


def main(args=None):
    global node
    rclpy.init(args=None)
    node = rclpy.create_node('custom_msg_sub_node')

    print("Hello World, I am the custom msg sub node! [Python]\n")

    sub_ultra_sound = node.create_subscription(CustomMsg, 'custom_msg_topic', custom_msg_callback, 10)

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

