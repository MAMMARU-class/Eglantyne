#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"
#include "std_msgs/msg/int32.hpp"
#include "../motion/motion_trig.h"

using namespace std::chrono_literals;
using std::placeholders::_1;

class SquwadTest : public rclcpp::Node
{
public:
    SquwadTest()
    : Node("squwad_test"), count_(0)
    {
        pub_motion_list_ = this->create_publisher<trajectory_msgs::msg::JointTrajectory>("/motion_list_command", 10);
        sub_trig_ = this->create_subscription<std_msgs::msg::Int32>(
            "/motion_trigger", 10, std::bind(&SquwadTest::pub_squwad_trajectory, this, _1));
    }
private:
    void pub_squwad_trajectory(const std_msgs::msg::Int32 msg){
        if (msg.data != SQUWAD){ return; }

        trajectory_msgs::msg::JointTrajectory motion;

        std::vector<double> initial = {0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0, 0., 0.0,
                            0.0, 0.0, 0.0, 0.0, 0., 0.0};
        std::vector<double> goal = {0.2, -0.3, -0.8,
                            0.2, 0.3, -0.8,
                            0.0, 0.0, -1.8, 2.455, -1.314, 0.0,
                            0.0, 0.0, -1.8, 2.455, -1.314, 0.0};
        
        int max_count = 20;
        int link_count = 18;

        for (int count=0; count<max_count; count++){
            trajectory_msgs::msg::JointTrajectoryPoint pos;
            std::vector<double> positions(link_count+1);
            for(int j=0; j<link_count; j++){
                positions[j] = (initial[j] * (max_count-count) + goal[j] * count) / max_count;
            }
            positions[link_count] = 0;
            pos.positions = positions;
            motion.points.push_back(pos);
        }
        for (int count=0; count<max_count; count++){
            trajectory_msgs::msg::JointTrajectoryPoint pos;
            std::vector<double> positions(link_count+1);
            for(int j=0; j<link_count; j++){
                positions[j] = (goal[j] * (max_count-count) + initial[j] * count) / max_count;
            }
            positions[link_count] = 0;
            pos.positions = positions;
            motion.points.push_back(pos);
        }
        
        pub_motion_list_->publish(motion);
    }

    rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr pub_motion_list_;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_trig_;
    size_t count_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SquwadTest>());
    rclcpp::shutdown();
    return 0;
}
