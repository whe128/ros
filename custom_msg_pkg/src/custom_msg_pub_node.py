#!/usr/bin/env python3
#conding: utf-8
import rclpy
from custom_msgs.msg import CustomMsg
import time

pub = None

def timer_callback():
    global pub

    msg = CustomMsg()

    msg.grade = "A+"
    msg.star = 50
    msg.data = "Hello, I send custom msg data!"
    pub.publish(msg)

def main(args=None):
    global pub

    rclpy.init(args=None)
    node = rclpy.create_node('custom_msg_pub_node')

    print("Hello World, I am the custom msg pub node! [Python]\n")

    pub = node.create_publisher(CustomMsg, 'custom_msg_topic', 10)

    node.create_timer(1.0, timer_callback)

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
