# CMake generated Testfile for 
# Source directory: /home/hibiki/Robomech_HR/Eglantyne/simulation/src/ros2_control_demos/example_7
# Build directory: /home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(example_7_urdf_xacro "/home/hibiki/.espressif/python_env/idf5.4_py3.10_env/bin/python3" "-u" "/opt/ros/humble/share/ament_cmake_test/cmake/run_test.py" "/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7/test_results/ros2_control_demo_example_7/example_7_urdf_xacro.xunit.xml" "--package-name" "ros2_control_demo_example_7" "--output-file" "/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7/ament_cmake_pytest/example_7_urdf_xacro.txt" "--command" "/home/hibiki/.espressif/python_env/idf5.4_py3.10_env/bin/python3" "-u" "-m" "pytest" "/home/hibiki/Robomech_HR/Eglantyne/simulation/src/ros2_control_demos/example_7/test/test_urdf_xacro.py" "-o" "cache_dir=/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7/ament_cmake_pytest/example_7_urdf_xacro/.cache" "--junit-xml=/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7/test_results/ros2_control_demo_example_7/example_7_urdf_xacro.xunit.xml" "--junit-prefix=ros2_control_demo_example_7")
set_tests_properties(example_7_urdf_xacro PROPERTIES  LABELS "pytest" TIMEOUT "60" WORKING_DIRECTORY "/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7" _BACKTRACE_TRIPLES "/opt/ros/humble/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/opt/ros/humble/share/ament_cmake_pytest/cmake/ament_add_pytest_test.cmake;169;ament_add_test;/home/hibiki/Robomech_HR/Eglantyne/simulation/src/ros2_control_demos/example_7/CMakeLists.txt;103;ament_add_pytest_test;/home/hibiki/Robomech_HR/Eglantyne/simulation/src/ros2_control_demos/example_7/CMakeLists.txt;0;")
add_test(view_example_7_launch "/home/hibiki/.espressif/python_env/idf5.4_py3.10_env/bin/python3" "-u" "/opt/ros/humble/share/ament_cmake_test/cmake/run_test.py" "/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7/test_results/ros2_control_demo_example_7/view_example_7_launch.xunit.xml" "--package-name" "ros2_control_demo_example_7" "--output-file" "/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7/ament_cmake_pytest/view_example_7_launch.txt" "--command" "/home/hibiki/.espressif/python_env/idf5.4_py3.10_env/bin/python3" "-u" "-m" "pytest" "/home/hibiki/Robomech_HR/Eglantyne/simulation/src/ros2_control_demos/example_7/test/test_view_robot_launch.py" "-o" "cache_dir=/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7/ament_cmake_pytest/view_example_7_launch/.cache" "--junit-xml=/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7/test_results/ros2_control_demo_example_7/view_example_7_launch.xunit.xml" "--junit-prefix=ros2_control_demo_example_7")
set_tests_properties(view_example_7_launch PROPERTIES  LABELS "pytest" TIMEOUT "60" WORKING_DIRECTORY "/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7" _BACKTRACE_TRIPLES "/opt/ros/humble/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/opt/ros/humble/share/ament_cmake_pytest/cmake/ament_add_pytest_test.cmake;169;ament_add_test;/home/hibiki/Robomech_HR/Eglantyne/simulation/src/ros2_control_demos/example_7/CMakeLists.txt;104;ament_add_pytest_test;/home/hibiki/Robomech_HR/Eglantyne/simulation/src/ros2_control_demos/example_7/CMakeLists.txt;0;")
add_test(run_example_7_launch "/home/hibiki/.espressif/python_env/idf5.4_py3.10_env/bin/python3" "-u" "/opt/ros/humble/share/ament_cmake_test/cmake/run_test.py" "/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7/test_results/ros2_control_demo_example_7/run_example_7_launch.xunit.xml" "--package-name" "ros2_control_demo_example_7" "--output-file" "/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7/ament_cmake_pytest/run_example_7_launch.txt" "--command" "/home/hibiki/.espressif/python_env/idf5.4_py3.10_env/bin/python3" "-u" "-m" "pytest" "/home/hibiki/Robomech_HR/Eglantyne/simulation/src/ros2_control_demos/example_7/test/test_r6bot_controller_launch.py" "-o" "cache_dir=/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7/ament_cmake_pytest/run_example_7_launch/.cache" "--junit-xml=/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7/test_results/ros2_control_demo_example_7/run_example_7_launch.xunit.xml" "--junit-prefix=ros2_control_demo_example_7")
set_tests_properties(run_example_7_launch PROPERTIES  LABELS "pytest" TIMEOUT "60" WORKING_DIRECTORY "/home/hibiki/Robomech_HR/Eglantyne/simulation/build/ros2_control_demo_example_7" _BACKTRACE_TRIPLES "/opt/ros/humble/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/opt/ros/humble/share/ament_cmake_pytest/cmake/ament_add_pytest_test.cmake;169;ament_add_test;/home/hibiki/Robomech_HR/Eglantyne/simulation/src/ros2_control_demos/example_7/CMakeLists.txt;105;ament_add_pytest_test;/home/hibiki/Robomech_HR/Eglantyne/simulation/src/ros2_control_demos/example_7/CMakeLists.txt;0;")