#!/usr/bin/env python3
#conding: utf-8
import rclpy
from rclpy.node import Node
from rclpy.action import ActionClient
from nav2_msgs.action import NavigateToPose

def main(args=None):
    rclpy.init(args=args)

    print("Hello World, I am the nav client node![Python]\n")

    node = Node("nav_client_node")

    client = ActionClient(node, NavigateToPose, "navigate_to_pose")

    # wait for server
    while not client.wait_for_server(timeout_sec=5.0):
        node.get_logger().info("Waiting for the action server to be available...")

    # create goal message
    goal_msg = NavigateToPose.Goal()
    goal_msg.pose.header.stamp = node.get_clock().now().to_msg()
    goal_msg.pose.header.frame_id = "map"

    goal_msg.pose.pose.position.x = 2.0
    goal_msg.pose.pose.position.y = 0.5
    goal_msg.pose.pose.orientation.w = 1.0

    node.get_logger().info("Sending goal to the action server...")

    # callback storage
    result_future = None
    goal_handle_holder = {}

    # feedback callback
    # def feedback_callback(feedback_msg):
    #     node.get_logger().info(f"Received feedback: {feedback_msg.feedback.current_pose}")

    # result callback
    def result_callback(future):
        result = future.result()

        status = result.status
        if status == 4:  # SUCCEEDED
            node.get_logger().info("Goal succeeded!")
        elif status == 5:  # ABORTED
            node.get_logger().info("Goal was aborted.")
        elif status == 6:  # CANCELED
            node.get_logger().info("Goal was canceled.")
        else:
            node.get_logger().info(f"Goal finished with status: {status}")

        rclpy.shutdown()

    # goal response callback
    def goal_response_callback(future):
        goal_handle = future.result()

        if not goal_handle.accepted:
            node.get_logger().info("Goal was rejected by the action server")
            rclpy.shutdown()
            return

        # request result
        future_result = goal_handle.get_result_async()
        future_result.add_done_callback(result_callback)

        # avoid garbage collection of goal_handle
        goal_handle_holder["goal_handle"] = goal_handle

    # send goal
    send_goal_future = client.send_goal_async(
        goal_msg,
        feedback_callback=None
    )

    send_goal_future.add_done_callback(goal_response_callback)

    rclpy.spin(node)

if __name__ == "__main__":
    main()
