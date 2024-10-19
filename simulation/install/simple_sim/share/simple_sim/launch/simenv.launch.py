from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    # パッケージディレクトリの取得
    simple_sim_pkg = get_package_share_directory('simple_sim')
    gazebo_ros_pkg = get_package_share_directory('gazebo_ros')

    # CougarBotのURDFファイルのパス
    urdf_file = os.path.join(simple_sim_pkg, 'urdf', 'eglantyne.urdf')

    # 空のワールドのlaunchファイルのパス
    empty_world_launch = os.path.join(gazebo_ros_pkg, 'launch', 'empty_world.launch.py')

    return LaunchDescription([
        # robot_descriptionパラメータをセットする
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            output='screen',
            parameters=[{'robot_description': open(urdf_file).read()}]
        ),

        # 空のワールドでGazeboを起動
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(empty_world_launch)
        ),

        # CougarBotをGazeboにスポーン
        Node(
            package='gazebo_ros',
            executable='spawn_entity.py',
            arguments=[
                '-param', 'robot_description',
                '-urdf',
                '-model', 'cougarbot'
            ],
            output='screen'
        ),
    ])
