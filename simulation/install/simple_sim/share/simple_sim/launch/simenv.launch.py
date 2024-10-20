from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
import xacro
import os

def generate_launch_description():
    # specify the name of the package and path to eacro file
    pkg_name = "simple_sim"
    pkg_path = get_package_share_directory(pkg_name)
    gazebo_ros_pkg = get_package_share_directory('gazebo_ros')

    xacro_file_name = "eglantyne.xacro"

    robot_description_content = Command(
        [
            PathJoinSubstitution([FindExecutable(name="xacro")]),
            " ",
            PathJoinSubstitution(
                [FindPackageShare(pkg_name), "urdf", xacro_file_name]
            ),
        ]
    )

    robot_state_publisher = Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            output='screen',
            parameters=[{"robot_description": robot_description_content,
                         'use_sim_time': True}]
    )
    
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [os.path.join(gazebo_ros_pkg, 'launch'), '/gazebo.launch.py'] )
    )

    spawn_entity = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=['-topic', 'robot_description',
                   '-entity', 'my_robot'],
        output='screen'
    )

    return LaunchDescription([
        robot_state_publisher,
        gazebo,
        spawn_entity,
    ])
