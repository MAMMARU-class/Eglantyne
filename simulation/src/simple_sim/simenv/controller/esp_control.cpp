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
        pub_for_esp_ = this->create_publisher<trajectory_msgs::msg::JointTrajectory>("/motion_list_command_for_esp", 10);
        timer_ = this->create_wall_timer(
            100ms, std::bind(&PubControl::timer_callback, this));
    }
private:
    void timer_callback()
    {
        if(order.empty()){ return; }

        trajectory_msgs::msg::JointTrajectory motion_list;
        // max 10 motion
        for(int i=0; i<10; i++){
            if(!order.empty()){
                trajectory_msgs::msg::JointTrajectoryPoint pos;
                pos = order.front();
                order.erase(order.begin());
                motion_list.points.push_back(pos);
            }
        }
        // publishe position data
        pub_for_esp_ -> publish(motion_list);
    }

    void update_motions(const trajectory_msgs::msg::JointTrajectory::SharedPtr motion) 
    {
        std::vector< trajectory_msgs::msg::JointTrajectoryPoint > motion_points = motion->points;
        order.insert(order.end(), motion_points.begin(), motion_points.end());
    }    
    std::vector< trajectory_msgs::msg::JointTrajectoryPoint > order;
    
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr pub_for_esp_;
    rclcpp::Subscription<trajectory_msgs::msg::JointTrajectory>::SharedPtr sub_motion_list_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PubControl>());
    rclcpp::shutdown();
    return 0;
}
