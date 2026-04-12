#!/usr/bin/env python3
#conding: utf-8

"""
Author: ssr
Date: 2024-06-01 17:00:00
LastEditTime: 2024-06-01 17:00:00
LastEditors: ssr
Description:


"""

import rclpy
from std_msgs.msg import String

pub = None

def timer_callback():
    global pub

    msg = String()
    msg.data = "Hello, I send aml data!"
    pub.publish(msg)

def main(args=None):
    rclpy.init(args=None)
    node = rclpy.create_node('aml_node')

    print("Hello World, I am the aml node! [Python]\n")

    pub = node.create_publisher(String, 'aml_topic', 10)

    rate = node.create_rate(4)  # 4 Hz

    node.create_timer(1.0, timer_callback)

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()

