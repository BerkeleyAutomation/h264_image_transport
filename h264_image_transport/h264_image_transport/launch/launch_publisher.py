from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='opencv_cam',
            executable='opencv_cam_main',
            name='image_publisher',
            parameters=[
                {'file': True},
                {'filename': 'src/test.mov'}
            ]
        )
    ])