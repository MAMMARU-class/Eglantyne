import os
import launch
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess
from launch.substitutions import Command
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    # パッケージパスの取得
    pkg_share = FindPackageShare(package='simple_sim').find('simple_sim')

    # xacroでURDFファイルを生成
    model = os.path.join(pkg_share, 'urdf', 'model.urdf')

    return LaunchDescription([
        # Gazebo Classicを空のワールドで起動
        ExecuteProcess(
            cmd=['gazebo', '--verbose', '--pause', '-s', 'libgazebo_ros_factory.so', 'empty.world'],
            output='screen'
        ),
        
        # URDFを読み込んでロボットをスポーンするノード
        Node(
            package='gazebo_ros',
            executable='spawn_entity.py',
            arguments=['-topic', '/robot_description', '-entity', 'my_robot'],
            output='screen'
        ),

        # ロボットのURDFをパラメータとして設定
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            output='screen',
            parameters=[{'robot_description': model}]
        ),
    ])
