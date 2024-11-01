cmake_minimum_required(VERSION 3.8)
project(simple_sim)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(control_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(std_msgs REQUIRED)
find_package(trajectory_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(Eigen3 REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

include_directories(
  PUBLIC $<BUILD_INTERFACE:${EIGEN3_INCLUDE_DIR}>)

set(MODULE
  src/module/Kinematics.cpp
  src/module/Link.cpp
)

add_executable(pub_control simenv/controller/pub_control.cpp)
ament_target_dependencies(pub_control rclcpp trajectory_msgs std_msgs)

add_executable(esp_control simenv/controller/esp_control.cpp)
ament_target_dependencies(esp_control rclcpp trajectory_msgs)

#motions
add_executable(omuni_dir_walk src/motion/omuni_dir_walk.cpp ${MODULE})
ament_target_dependencies(omuni_dir_walk rclcpp trajectory_msgs std_msgs sensor_msgs)

# test codes
add_executable(pub_test src/test/pub_test.cpp)
ament_target_dependencies(pub_test rclcpp trajectory_msgs)

add_executable(zero_pub src/test/zero_pub.cpp)
ament_target_dependencies(zero_pub rclcpp trajectory_msgs)

add_executable(squwad_esp src/test/squwad_esp.cpp)
ament_target_dependencies(squwad_esp rclcpp trajectory_msgs std_msgs)

add_executable(walk_test src/test/walk_test.cpp ${MODULE})
ament_target_dependencies(walk_test rclcpp trajectory_msgs std_msgs)

install(TARGETS
  pub_control
  esp_control

  omuni_dir_walk

  pub_test

  zero_pub
  squwad_esp
  walk_test
  DESTINATION lib/${PROJECT_NAME}/)

install(
  DIRECTORY description/launch description/urdf
  DESTINATION share/${PROJECT_NAME}/)

install(
  DIRECTORY simenv/launch simenv/config
  DESTINATION share/${PROJECT_NAME}/)

ament_package()
