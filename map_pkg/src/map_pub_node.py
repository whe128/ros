#!/usr/bin/env python3
#conding: utf-8
import rclpy
from nav_msgs.msg import OccupancyGrid

pub = None

def timer_callback():
    global pub

    msg = OccupancyGrid()

    msg.header.frame_id = "map"
    msg.header.stamp = rclpy.time.Time().to_msg()
    msg.info.resolution = 1.0
    msg.info.width = 4
    msg.info.height = 2
    msg.info.origin.position.x = 0.0
    msg.info.origin.position.y = 0.0

    msg.data = [0] * (msg.info.width * msg.info.height)
    msg.data[0] = 100
    msg.data[1] = 100
    msg.data[2] = 0
    msg.data[3] = -1

    pub.publish(msg)

def main(args=None):
    global pub

    rclpy.init(args=None)
    node = rclpy.create_node('map_pub_node')

    print("Hello World, I am the map pub node! [Python]\n")

    pub = node.create_publisher(OccupancyGrid, '/map', 10)

    node.create_timer(1.0, timer_callback)

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
