import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node


def generate_launch_description():

    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory("wpr_simulation2"),
                "launch",
                "robocup_home.launch.py"
            )
        )
    )

    # launch nav2
    nav2_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory("nav2_bringup"),
                "launch",
                "bringup_launch.py"
            )
        ),
        launch_arguments={
            "map": os.path.join(
                get_package_share_directory("nav_pkg"),
                "maps",
                "map.yaml"
            ),
            "params_file": os.path.join(
                get_package_share_directory("nav_pkg"),
                "params",
                "nav2_params.yaml"
            ),
            "use_sim_time": "true"
        }.items()
    )

    # rviz_launch
    rviz_launch = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
    )

    return LaunchDescription([
        gazebo_launch,
        nav2_launch,
        rviz_launch,
    ])
