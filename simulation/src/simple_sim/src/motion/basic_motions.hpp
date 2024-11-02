#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <numbers>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"
#include "sensor_msgs/msg/joy.hpp"
#include "std_msgs/msg/int32.hpp"
#include "../module/Link.h"
#include "../module/Kinematics.h"
#include "../motion/motion_trig.h"

using namespace std::chrono_literals;
using std::placeholders::_1;
using Eigen::Vector2d; using Eigen::Vector3d; using Eigen::Vector4d;
using Eigen::Matrix2d;

using Int32 = std_msgs::msg::Int32;

class BasicMotion : public rclcpp::Node
{
public:
    BasicMotion()
    : Node("basic motion")
    {
        sub_trig_ = this->create_subscription<std_msgs::msg::Int32>(
            "/motion_trigger", 10, std::bind(&BasicMotion::handle_motion, this, _1));
        pub_trig_ = this->create_publisher<std_msgs::msg::Int32>("/motion_trigger", 1);
        sub_joy_ = this->create_subscription<sensor_msgs::msg::Joy>(
            "/joy", 10, std::bind(&BasicMotion::update_joy_order, this, _1));
    }

private:
    void update_joy_order(const sensor_msgs::msg::Joy msg);
    void handle_motion(const Int32 trig);
}