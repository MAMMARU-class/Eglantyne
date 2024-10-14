# Copyright 2021 Kenji Brameld
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument('namespace', default_value=''),
        nao_state_publisher()
    ])


def nao_state_publisher():

    urdf_file_name = 'nao.urdf'
    urdf = os.path.join(
        get_package_share_directory('nao_description'),
        'urdf',
        urdf_file_name)
    with open(urdf, 'r') as infp:
        robot_desc = infp.read()

    return Node(
            package='nao_state_publisher',
            executable='nao_state_publisher',
            namespace=LaunchConfiguration('namespace'),
            output='screen',
            parameters=[{'robot_description': robot_desc}],
            arguments=[urdf],
        )
