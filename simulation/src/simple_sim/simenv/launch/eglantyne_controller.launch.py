from launch import LaunchDescription
from launch.actions import RegisterEventHandler, DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.event_handlers import OnProcessExit
from launch.substitutions import Command, FindExecutable, PathJoinSubstitution, LaunchConfiguration

from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
import xacro
import os

def generate_launch_description():
    # specify the name of the package and path to eacro file
    pkg_name = "simple_sim"

    robot_controllers = PathJoinSubstitution(
        [
            FindPackageShare(pkg_name),
            "config",
            "eglantyne_controller.yaml",
        ]
    )

    control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[robot_controllers],
        remappings=[("~/robot_description", "/robot_description"),
        ],
        output="both",
    )

    view_robot = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [os.path.join(FindPackageShare(pkg_name), "launch"), "/view_robot.lanuch.py"]
        )
    )

    return LaunchDescription([
        control_node,
        view_robot
    ])
