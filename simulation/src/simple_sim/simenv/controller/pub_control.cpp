#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class PubControl : public rclcpp::Node
{
public:
    PubControl()
    : Node("pub_control")
    {
        sub_motion_list_ = this->create_subscription<trajectory_msgs::msg::JointTrajectory>(
            "/motion_list_command", 10, std::bind(&PubControl::update_motions, this, _1));
        pub_positions_ = this->create_publisher<trajectory_msgs::msg::JointTrajectory>("/joint_trajectory_controller/joint_trajectory", 10);
        timer_ = this->create_wall_timer(
            20ms, std::bind(&PubControl::timer_callback, this));

        pub_state_ = this->create_publisher<std_msgs::msg::Int32>("/motion_trigger", 10);
    }
private:
    void timer_callback()
    {
        if(order.empty()){
            // auto msg = std_msgs::msg::Int32();
            // msg.data = 1;
            // pub_state_ -> publish(msg);
            return;
        }
        trajectory_msgs::msg::JointTrajectory positions;
        positions.joint_names = this->joint_names;

        trajectory_msgs::msg::JointTrajectoryPoint pos;
        pos = order.front();
        order.erase(order.begin());

        // publish motion state message
        if (sizeof(pos.positions) / sizeof(pos.positions[0]) > 18){
            double motion_trig = pos.positions.back();
            RCLCPP_INFO(this->get_logger(), "exception received : %f", motion_trig);
            pos.positions.erase(pos.positions.end());

            auto msg = std_msgs::msg::Int32();
            msg.data = (int)motion_trig;
            pub_state_->publish(msg);
        }

        // publishe position data
        pos.time_from_start.sec = 0.02;
        positions.points.push_back(pos);
        pub_positions_ -> publish(positions);
    }

    void update_motions(const trajectory_msgs::msg::JointTrajectory::SharedPtr motion) 
    {
        std::vector< trajectory_msgs::msg::JointTrajectoryPoint > motion_points = motion->points;
        order.insert(order.end(), motion_points.begin(), motion_points.end());
    }

    std::vector<std::string> joint_names =  {"arm_pitch_right", "arm_roll_right", "hand_right", 
                                                "arm_pitch_left", "arm_roll_left", "hand_left", 
                                                "leg_yaw_right", "leg_roll_right", "leg_upper_right", "leg_under_right", "foot_pitch_right", "foot_roll_right", 
                                                "leg_yaw_left", "leg_roll_left", "leg_upper_left", "leg_under_left", "foot_pitch_left", "foot_roll_left"};
    std::vector< trajectory_msgs::msg::JointTrajectoryPoint > order;
    
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr pub_positions_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_state_;
    rclcpp::Subscription<trajectory_msgs::msg::JointTrajectory>::SharedPtr sub_motion_list_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PubControl>());
    rclcpp::shutdown();
    return 0;
}
