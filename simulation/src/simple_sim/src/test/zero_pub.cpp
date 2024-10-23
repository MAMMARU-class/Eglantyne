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
                          0.0, 0.0, -0.5, 1.05, -0.5, 0.0,
                          0.0, 0.0, -0.5, 1.05, -0.5, 0.0};
    std::vector<double> goal = {0.0, 0.0, 0.0,
                          0.0, 0.0, 0.0,
                          0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                          0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    
    int max_count = 150;
    for (int count=0; count<max_count; count++){
        trajectory_msgs::msg::JointTrajectoryPoint pos;
        std::vector<double> positions(18);
        for(int j=0; j<18; j++){
            positions[j] = (initial[j] * (max_count-count) + goal[j] * count) / max_count;
        }
        pos.positions = positions;
        motion.points.push_back(pos);
    }
    
    pub->publish(motion);
    while (rclcpp::ok()) 
    {
    }
    return 0;
}
