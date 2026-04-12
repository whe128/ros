#!/usr/bin/env python3
#conding: utf-8

import rclpy
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist

node = None
vel_pub = None
rotate_cycle = 0

def lidar_callback(msg):
    global node
    global vel_pub
    global rotate_cycle

    front_distance = msg.ranges[180]
    node.get_logger().info(f"The front distance is: {front_distance} meters")

    vel_msg = Twist()

    if rotate_cycle > 0:
        rotate_cycle -= 1
        return

    if front_distance < 1.5:
        vel_msg.angular.z = 2.0
        rotate_cycle = 9
    else:
        vel_msg.linear.x = 2.0

    vel_pub.publish(vel_msg)

def main(args=None):
    global node
    global vel_pub

    rclpy.init(args=None)

    node = rclpy.create_node('lidar_node')

    print("Hello World, I am the lidar node! [Python]\n")

    vel_pub = node.create_publisher(Twist, '/cmd_vel', 10)
    sub = node.create_subscription(LaserScan, '/scan', lidar_callback, 10)

    rclpy.spin(node)

    node.destroy_node()

    rclpy.shutdown()

if __name__ == '__main__':
    main()
