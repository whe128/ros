from launch import LaunchDescription
import launch_ros.actions

def generate_launch_description():
    return LaunchDescription([
        launch_ros.actions.Node(
            package='ssr_pkg', executable='ultra_sound_node'),
        launch_ros.actions.Node(
            package='ssr_pkg', executable='aml_node'),
    ])
