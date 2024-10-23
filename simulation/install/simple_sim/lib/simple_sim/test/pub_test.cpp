#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"

using namespace std::chrono_literals;

class PubTest : public rclcpp::Node
{
public:
    PubTest()
    : Node("pub_test"), count_(0)
    {
        publisher_ = this->create_publisher<trajectory_msgs::msg::JointTrajectory>("/joint_trajectory_controller/joint_trajectory", 10);
        timer_ = this->create_wall_timer(
            1000ms, std::bind(&PubTest::timer_callback, this)
        );
    }
private:
    void timer_callback()
    {
        std::vector<std::string> joint_names =  {"arm_pitch_right", "arm_roll_right", "hand_right", 
                                                "arm_pitch_left", "arm_roll_left", "hand_left", 
                                                "leg_yaw_right", "leg_roll_right", "leg_upper_right", "leg_under_right", "foot_pitch_right", "foot_roll_right", 
                                                "leg_yaw_left", "leg_roll_left", "leg_upper_left", "leg_under_left", "foot_pitch_left", "foot_roll_left"};

        trajectory_msgs::msg::JointTrajectoryPoint pos1;
        pos1.positions.resize(joint_names.size());
        pos1.positions = {0.0, 0.0, -0.8,
                          0.0, 0.0, -0.8,
                          0.0, 0.0, -0.5, 0.25, -0.5, 0.0,
                          0.0, 0.0, -0.5, 0.25, -0.5, 0.0};
        
        // std::vector<trajectory_msgs::msg::JointTrajectoryPoint> pos2;
        // pos2.resize(joint_names.size());
        // pos2 = {0.0, 0.0, 0.0,
        //                   0.0, 0.0, 0.0,
        //                   0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        //                   0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        
        trajectory_msgs::msg::JointTrajectory positions;
        positions.joint_names = joint_names;
        positions.points.push_back(pos1);
        // if(count_++ % 2 == 0){ positions.points.push_back(pos1);
        // }else{positions.points.push_back(pos2); }
        publisher_ -> publish(positions);
    }

    
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PubTest>());
    rclcpp::shutdown();
    return 0;
}
