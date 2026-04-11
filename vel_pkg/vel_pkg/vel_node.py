#!/usr/bin/env python3
#conding: utf-8

import rclpy
from geometry_msgs.msg import Twist

def main(args=None):
    rclpy.init(args=None)

    node = rclpy.create_node('vel_node')

    print("Hello World, I am the aml node! [Python]\n")

    pub = node.create_publisher(Twist, '/cmd_vel', 10)

    rate = node.create_rate(30)  # 30 Hz

    while rclpy.ok():
        rclpy.spin_once(node)
        msg = Twist()

        msg.linear.x = 0.5
        msg.linear.y = 0.0
        msg.linear.z = 0.0

        msg.angular.x = 0.0
        msg.angular.y = 0.0
        msg.angular.z = 0.0

        pub.publish(msg)

        rate.sleep()

    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
