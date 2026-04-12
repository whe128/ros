#!/usr/bin/env python3
#conding: utf-8

import rclpy
from sensor_msgs.msg import Imu
from tf_transformations import euler_from_quaternion
from geometry_msgs.msg import Twist
import math

node = None
vel_pub = None

def imu_callback(msg):
    global node
    global vel_pub

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

    # course control
    target_yaw = 90.0
    yaw_error = target_yaw - yaw
    kp = 0.01

    cmd_vel = Twist()

    cmd_vel.linear.x = 0.1
    cmd_vel.angular.z = kp * yaw_error
    vel_pub.publish(cmd_vel)



def main(args=None):
    global node
    global vel_pub
    rclpy.init(args=None)
    node = rclpy.create_node('imu_node')

    print("Hello World, I am the imu node! [Python]\n")

    sub = node.create_subscription(Imu, '/imu/data', imu_callback, 10)
    vel_pub = node.create_publisher(Twist, "/cmd_vel", 10)

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
