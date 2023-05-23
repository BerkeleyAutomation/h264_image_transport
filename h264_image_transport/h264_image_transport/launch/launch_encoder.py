from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='image_transport',
            executable='republish',
            name='h264_encoder',
            arguments=['raw', 'h264'],
            remappings=[
                ('in', 'image_raw'),
                ('out/h264', 'image/h264')
            ]
        )
    ])
