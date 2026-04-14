#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <nav2_msgs/action/navigate_to_pose.hpp>

typedef nav2_msgs::action::NavigateToPose NavigateToPose;


int main (int argc, char ** argv)
{
    rclcpp::init(argc, argv);

    printf("Hello World, I am the nav client node!\n");

    std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("nav_client");
    std::shared_ptr<rclcpp_action::Client<NavigateToPose>> client =
        rclcpp_action::create_client<NavigateToPose>(node, "navigate_to_pose");

    // while loop to wait for the action server to be available, with a timeout of 5 seconds
    while(!client->wait_for_action_server(std::chrono::seconds(5))) {
        RCLCPP_INFO(node->get_logger(), "Waiting for the action server to be available...");
    }

    // create a goal message
    NavigateToPose::Goal goal_msg;
    goal_msg.pose.header.stamp = node->get_clock()->now();
    goal_msg.pose.header.frame_id = "map";
    goal_msg.pose.pose.position.x = 2.0;
    goal_msg.pose.pose.position.y = 0.5;
    goal_msg.pose.pose.orientation.w = 1.0;

    RCLCPP_INFO(node->get_logger(), "Sending goal to the action server...");

    rclcpp_action::Client<NavigateToPose>::SendGoalOptions options =
        rclcpp_action::Client<NavigateToPose>::SendGoalOptions();

    // send
    auto goal_handle_future = client->async_send_goal(goal_msg, options);

    // wait accept for handle
    if (rclcpp::spin_until_future_complete(node, goal_handle_future)
        != rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_ERROR(node->get_logger(), "send_goal failed");
        return 1;
    };

    auto goal_handle = goal_handle_future.get();
    if (!goal_handle) {
        RCLCPP_ERROR(node->get_logger(), "Goal was rejected by the action server");
        return 1;
    }

    // wait for the result
    auto result_future = client->async_get_result(goal_handle);

    if (rclcpp::spin_until_future_complete(node, result_future)
        != rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_ERROR(node->get_logger(), "get_result failed");
        return 1;
    }

    auto result = result_future.get();

    // output the ressult
    switch (result.code)
    {
    case rclcpp_action::ResultCode::SUCCEEDED:
        RCLCPP_INFO(node->get_logger(), "Goal succeeded");
        break;

    case rclcpp_action::ResultCode::ABORTED:
        RCLCPP_ERROR(node->get_logger(), "Goal aborted");
        break;

    case rclcpp_action::ResultCode::CANCELED:
        RCLCPP_WARN(node->get_logger(), "Goal canceled");
        break;

    default:
        RCLCPP_ERROR(node->get_logger(), "Unknown result code");
        break;
    }

    rclcpp::shutdown();
    return 0;
}



