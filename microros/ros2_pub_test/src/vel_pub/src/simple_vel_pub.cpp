#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("zero_pub");
    auto pub = node->create_publisher<trajectory_msgs::msg::JointTrajectory>(
        "/motion_list_command", 10);

    trajectory_msgs::msg::JointTrajectory motion_list;

    std::vector<double> start = {3500, 3500};
    std::vector<double> end = {5500, 7500};

    trajectory_msgs::msg::JointTrajectoryPoint pos;
    pos.positions = start;
    int max = 20;
    for(int i=0; i<max; i++){
        pos.positions[0] += 2000/max;
        pos.positions[1] += 4000/max;
        motion_list.points.push_back(pos);
    }
    pos.positions = end;
    for(int i=0; i<max; i++){
        pos.positions[0] -= 2000/max;
        pos.positions[1] -= 4000/max;
        motion_list.points.push_back(pos);
    }
    // for(int i=0; i<22; i++){
    //     pos.positions[0] += 30;
    //     pos.positions[1] -= 60;
    //     motion_list.points.push_back(pos);
    // }
    
    pub->publish(motion_list);
    while (rclcpp::ok())
    {}
    return 0;
}
