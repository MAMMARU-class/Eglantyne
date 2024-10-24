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

    trajectory_msgs::msg::JointTrajectory motion;

    std::vector<double> initial = {0.0, 0.0, -0.8,
                          0.0, 0.0, -0.8,
                          0.0, 0.0, -0.32, 1.15, -0.8, 0.0,
                          0.0, 0.0, -0.32, 1.15, -0.8, 0.0};
    std::vector<double> goal = {0.0, 0.0, -0.8,
                          0.0, 0.0, -0.8,
                          0.0, 0.0, -0.44, 1.455, -1.014, 0.0,
                          0.0, 0.0, -0.44, 1.455, -1.014, 0.0};
    
    int max_count = 50;
    for (int loop=0; loop<50; loop++){
    for (int count=0; count<max_count; count++){
        trajectory_msgs::msg::JointTrajectoryPoint pos;
        std::vector<double> positions(18);
        for(int j=0; j<18; j++){
            positions[j] = (initial[j] * (max_count-count) + goal[j] * count) / max_count;
        }
        pos.positions = positions;
        motion.points.push_back(pos);
    }
    for (int count=0; count<max_count; count++){
        trajectory_msgs::msg::JointTrajectoryPoint pos;
        std::vector<double> positions(18);
        for(int j=0; j<18; j++){
            positions[j] = (goal[j] * (max_count-count) + initial[j] * count) / max_count;
        }
        pos.positions = positions;
        motion.points.push_back(pos);
    }
    }
    
    pub->publish(motion);
    while (rclcpp::ok()) 
    {
    }
    return 0;
}
