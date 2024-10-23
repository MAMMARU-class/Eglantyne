#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"
// #include "simple_sim/msg/motion.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class PubControl : public rclcpp::Node
{
public:
    PubControl()
    : Node("pub_control")
    {
        publisher_ = this->create_publisher<trajectory_msgs::msg::JointTrajectory>("/joint_trajectory_controller/joint_trajectory", 10);
        timer_ = this->create_wall_timer(
            20ms, std::bind(&PubControl::timer_callback, this));

        subscription_ = this->create_subscription<trajectory_msgs::msg::JointTrajectory>(
            "/motion_list_command", 10, std::bind(&PubControl::update_motions, this, _1));
    }
private:
    void timer_callback()
    {
        if(!order.empty()){
            trajectory_msgs::msg::JointTrajectory positions;
            positions.joint_names = this->joint_names;

            trajectory_msgs::msg::JointTrajectoryPoint pos;
            pos = order.front();
            pos.time_from_start.sec = 0.02;
            order.erase(order.begin());

            positions.points.push_back(pos);
            publisher_ -> publish(positions);
        }
    }

    void update_motions(const trajectory_msgs::msg::JointTrajectory::SharedPtr motion) 
    {
        std::vector< trajectory_msgs::msg::JointTrajectoryPoint > motion_points = motion->points;
        // for(const auto& point : motion_points){
        //     trajectory_msgs::msg::JointTrajectoryPoint pos;
        //     std::vector<double> positions;
        //     for(const auto& angle : point.positions){
        //         positions.push_back(angle);
        //     }
        //     pos.positions = positions;
        //     order.push_back(pos);
        // }
        // order.insert(order.begin(), motion_points.begin(), motion_points.end());
        order.insert(order.end(), motion_points.begin(), motion_points.end());

    }

    std::vector<std::string> joint_names =  {"arm_pitch_right", "arm_roll_right", "hand_right", 
                                                "arm_pitch_left", "arm_roll_left", "hand_left", 
                                                "leg_yaw_right", "leg_roll_right", "leg_upper_right", "leg_under_right", "foot_pitch_right", "foot_roll_right", 
                                                "leg_yaw_left", "leg_roll_left", "leg_upper_left", "leg_under_left", "foot_pitch_left", "foot_roll_left"};
    std::vector< trajectory_msgs::msg::JointTrajectoryPoint > order;
    
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr publisher_;
    rclcpp::Subscription<trajectory_msgs::msg::JointTrajectory>::SharedPtr subscription_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PubControl>());
    rclcpp::shutdown();
    return 0;
}
