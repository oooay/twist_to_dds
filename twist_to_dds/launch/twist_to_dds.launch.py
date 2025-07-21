from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument(
            'twist_topic',
            default_value='/twist_cmd',
            description='Name of the twist topic to subscribe to'
        ),
        Node(
            package='twist_to_dds',
            executable='twist_to_dds',
            name='twist_to_dds',
            output='screen',
            parameters=[{'twist_topic': LaunchConfiguration('twist_topic')}]
        )
    ])