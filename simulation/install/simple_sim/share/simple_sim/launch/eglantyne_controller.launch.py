from launch import LaunchDescription
from launch.actions import RegisterEventHandler, DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.event_handlers import OnProcessExit
from launch.substitutions import Command, FindExecutable, PathJoinSubstitution, LaunchConfiguration

from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

from launch.actions import ExecuteProcess, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    # specify the name of the package and path to eacro file
    pkg_name = "simple_sim"
    pkg_path = get_package_share_directory(pkg_name)

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
        # remappings=[("~/robot_description", "/robot_description"),
        # ],
        output="both",
    )

    view_robot = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [os.path.join(pkg_path, "launch"), "/view_robot.launch.py"]
        )
    )

    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster", "--controller-manager", "/controller_manager"],
    )

    robot_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_trajectory_controller", "-c", "/controller_manager"],
    )

    # load_joint_state_broadcaster = ExecuteProcess(
    #     cmd=['ros2', 'control', 'load_controller', '--set-state', 'active',
    #          'joint_state_broadcaster'],
    #     output='screen'
    # )

    # load_joint_trajectory_controller = ExecuteProcess(
    #     cmd=['ros2', 'control', 'load_controller', '--set-state', 'active',
    #          'joint_trajectory_controller'],
    #     output='screen'
    # )

    return LaunchDescription([
        control_node,
        view_robot,
        # load_joint_state_broadcaster,
        # load_joint_trajectory_controller,
        joint_state_broadcaster_spawner,
        robot_controller_spawner,
    ])
