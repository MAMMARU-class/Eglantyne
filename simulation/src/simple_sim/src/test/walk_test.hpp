#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <numbers>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"
// #include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"
#include "../module/Link.h"
#include "../module/Kinematics.h"
#include "../motion/motion_trig.h"

// step firsts with right foot and x defference = 0

using namespace std::chrono_literals;
using std::placeholders::_1;
using Eigen::Vector2d; using Eigen::Vector3d;

#define INTERVAL 160 // ms

// fixed variables
#define Z 237.4 // grand to COM
#define BASE_TO_COM 40.0
#define END_TO_FOOT 37.4
#define FULC_TO_FOOT_JOINT -28
#define H 25.0 // foot height while walking 
#define G (9.81 * 1000) // mm/s^2
#define PI 3.1415

#define Tsup 1.2 // s
#define Tc std::sqrt(Z / G)
#define C std::cosh(Tsup / Tc)
#define S std::sinh(Tsup / Tc)
#define COEF_A 10
#define COEF_B 1
#define D ( COEF_A * (C-1)*(C-1) + COEF_B * (S/Tc)*(S/Tc) )

#define WALK_STEP Tsup / INTERVAL * 1000

class WalkTest : public rclcpp::Node
{
public:
    WalkTest()
    : Node("walk_test")
    {
        setup_link();

        pub_motion_list_ = this->create_publisher<trajectory_msgs::msg::JointTrajectory>("/motion_list_command", 10);
        sub_trig_ = this->create_subscription<std_msgs::msg::Int32>(
            "/motion_trigger", 10, std::bind(&WalkTest::pub_walk_trajectory, this, _1));

        RCLCPP_INFO(this->get_logger(), "done with node setup");
    }
private:
    // foot pos at n+1 step
    void calc_foot_pos();
    // COM traj at n step
    void calc_COM_traj_next();
    // swing leg traj at n step
    void calc_swing_foot_traj();
    // integrate COM traj with foot traj
    void integrate_traj();
    // convert trajectory to joint angle for each
    void traj_to_motion();

    // publish motion set
    void pub_walk_trajectory(const std_msgs::msg::Int32 msg);

    Link body;
    Link leg_yaw_right;
    Link leg_roll_right;
    Link leg_upper_right;
    Link leg_under_right;
    Link foot_pitch_right;
    Link foot_roll_right;

    Link leg_yaw_left;
    Link leg_roll_left;
    Link leg_upper_left;
    Link leg_under_left;
    Link foot_pitch_left;
    Link foot_roll_left;

    void setup_link();
    std::vector<Link*> link_vec;

    // changeble variables
    double sx=50;
    double sy=80;

    // dirction of step n
    // swing leg == left if step_dir == 1
    int step_dir = -1;
    Vector2d m2_step{0, 60}; // foot pos at n-2 step (origin: n-1 step)
    Vector2d aim_step{0, 60}; // foot pos aim of on going walk (n step. origin: n-1 step)
    Vector2d p1_step{0, 0}; // foot pos at n+1 step (origin: n-1 step)

    Vector2d COM_p_start;
    Vector2d COM_v_start;
    Vector2d COM_p_aim{0, 30};
    Vector2d COM_v_aim{0, -190};

    std::vector< Vector3d > COM_traj;
    std::vector< Vector3d > COM_traj_next;
    std::vector< Vector3d > swing_foot_traj;
    std::vector< Vector3d > body_to_fixed_foot_traj;
    std::vector< Vector3d > body_to_swing_foot_traj;

    std::vector< std::vector<double> > foot_motion;

    std::vector<double> arm_angle = {0.523, -0.349, -0.785, 0.523, 0.349, -0.785};
    
    void COM_traj_zero();


    rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr pub_motion_list_;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_trig_;
};

