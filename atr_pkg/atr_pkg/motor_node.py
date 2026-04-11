#!/usr/bin/env python3
#conding: utf-8


import rclpy
from std_msgs.msg import String

node = None

def ultra_sound_callback(msg):
    global node
    node.get_logger().info("I am the motor node and received a message: " + msg.data)

def aml_callback(msg):
    global node
    node.get_logger().warn("I am the motor node and received a message: " + msg.data)


def main(args=None):
    global node
    rclpy.init(args=None)
    node = rclpy.create_node('motor_node')

    print("Hello World, I am the motor node!\n")

    sub_ultra_sound = node.create_subscription(String, 'ultra_sound_topic', ultra_sound_callback, 10)
    sub_aml = node.create_subscription(String, 'aml_topic', aml_callback, 10)

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

