#!/usr/bin/env python3
#conding: utf-8

import rclpy
from sensor_msgs.msg import Imu
from tf_transformations import euler_from_quaternion
import math

node = None

def imu_callback(msg):
    global node

    if msg.orientation_covariance[0] == -1:
        return

    quaternion = msg.orientation
    orientation_list = [
        quaternion.x,
        quaternion.y,
        quaternion.z,
        quaternion.w
    ]

    (roll, pitch, yaw) = euler_from_quaternion(orientation_list)

    roll = math.degrees(roll)
    pitch = math.degrees(pitch)
    yaw = math.degrees(yaw)

    node.get_logger().info("The orientation of the robot is: roll: %.2f, pitch: %.2f, yaw: %.2f" % (roll, pitch, yaw))

def main(args=None):
    global node
    rclpy.init(args=None)
    node = rclpy.create_node('imu_node')

    print("Hello World, I am the imu node! [Python]\n")

    sub = node.create_subscription(Imu, '/imu/data', imu_callback, 10)

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
