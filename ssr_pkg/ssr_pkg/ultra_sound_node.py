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

def main(args=None):
    rclpy.init(args=None)
    node = rclpy.create_node('ultra_sound_node')

    print("Hello World, I am the ultra sound node! [Python]\n")

    pub = node.create_publisher(String, 'ultra_sound_topic', 10)

    rate = node.create_rate(1)  # 1 Hz

    while rclpy.ok():
        msg = String()
        msg.data = "Hello, I send ultra sound data!"
        pub.publish(msg)

        rate.sleep()

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()

