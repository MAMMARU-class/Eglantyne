#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"

class Squad : public rclcpp::Node
{
public:
    Squad()
    : Node("squad"), count_(0)
    {
        publisher_ = this->create_publisher<trajectory_msgs::msg::JointTrajectory>("/joint_trajectory_controller/joint_trajectory", 10);
        timer_ = this->create_wall_timer(
            1000ms, std::bind(&Squad::timer_callback, this)
        );
    }
private:
    void timer_callback()
    {
        auto positions = trajectory_msgs::msg::JointTrajectory();
        positions.joint_names = ['arm_pitch_right', 'arm_roll_right', 'hand_right', 
                                                'arm_pitch_left', 'arm_roll_left', 'hand_left', 
                                                'leg_yaw_right', 'leg_roll_right', 'leg_upper_right', 'leg_under_right', 'foot_pitch_right', 'foot_roll_right', 
                                                'leg_yaw_left', 'leg_roll_left', 'leg_upper_left', 'leg_under_left', 'foot_pitch_left', 'foot_roll_left'];

        std::vector<double> pos1 = [0.0, 0.0, -0.8,
                                    0.0, 0.0, -0.8,
                                    0.0, 0.0, -0.5, 0.25, -0.5, 0.0,
                                    0.0, 0.0, -0.5, 0.25, -0.5, 0.0]
        std::vector<double> pos2 = [0.0, 0.0, 0.0,
                                    0.0, 0.0, 0.0,
                                    0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                    0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
                                     
        if(count_++ % 2 == 0){ positions.points.positions = pos1;
        }else{positions.points.positions = pos2; }
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Squad>());
    rclcpp::shutdown();
    return 0;
}
