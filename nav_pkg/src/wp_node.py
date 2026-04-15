#!/usr/bin/env python3
#conding: utf-8
import rclpy
from std_msgs.msg import String
import time
import sys

waypoints = ["1", "2", "3", "4"]
index = 0
waiting = False
node = None
waypoint_pub = None


def nav_result_callback(msg):
    global node, waiting, index
    node.get_logger().info("Received Result: " + msg.data)
    index += 1
    waiting = False

    time.sleep(1.0)  # Sleep for a short time before sending the next waypoint

    if index < len(waypoints):
        msg = String()
        msg.data = waypoints[index]
        waypoint_pub.publish(msg)
        node.get_logger().info(f"Send waypoint name : {msg.data}")
    else:
        node.get_logger().info("All waypoints have been sent.")



def main(args=None):
    global node, waypoint_pub
    rclpy.init(args=None)

    print("Hello World, I am the wp node![Python]\n");

    node = rclpy.create_node('wp_node')
    waypoint_pub = node.create_publisher(String, "waterplus/navi_waypoint", 10)
    result_sub = node.create_subscription(String, "waterplus/navi_result", nav_result_callback, 10)

    time.sleep(1.0)  # Sleep for a short time to ensure subscriptions are set up

    msg = String()
    msg.data = waypoints[index]

    waypoint_pub.publish(msg)
    node.get_logger().info(f"Send waypoint name : {msg.data}")

    rclpy.spin(node)

if __name__ == '__main__':
    main()

