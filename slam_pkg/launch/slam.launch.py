import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node

def generate_launch_description():

    # get the launch directory
    launch_file_dir = os.path.join(
        get_package_share_directory(
            "wpr_simulation2"),
            "launch"
    )

    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                launch_file_dir,
                "robocup_home.launch.py"
            )
        )
    )

    # slam_launch
    slam_launch = Node(
        package = "slam_toolbox",
        executable="async_slam_toolbox_node",
        parameters=[{
            "use_sim_time": True,
            "base_frame": "base_footprint",
            "odom_frame": "odom",
            "map_frame": "map",
            "map_update_interval": 0.05,
        }]
    )

    # rviz_launch
    rviz_launch = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        arguments=['-d', [os.path.join(get_package_share_directory('wpr_simulation2'), 'rviz', 'slam.rviz')]]
    )

    # rqt steering launch
    rqt_steering_launch = Node(
        package="rqt_robot_steering",
        executable="rqt_robot_steering",
        name="rqt_robot_steering",
    )

    return LaunchDescription([
        gazebo_launch,
        slam_launch,
        rviz_launch,
        rqt_steering_launch
    ])
