#include "Kinematics.h"
#include "Link.h"

#define WALK_TRIG 0 // walk_trig

using namespace std::chrono_literals;
using std::placeholders::_1;
using Eigen::Vector2d; using Eigen::Vector3d;

#define INTERVAL 20 // ms

// fixed variables
#define Z 237.4 // grand to COM
#define BASE_TO_COM 40.0
#define END_TO_FOOT 37.4
#define H 15.0 // foot height while walking 
#define G (9.81 * 1000) // mm/s^2

#define Tsup 0.8 // s
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
    void setup_link();
    
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

    void pub_walk_trajectory(const std_msgs::msg::String msg)
    {
        if (COM_traj_next.empty()) {COM_traj_zero()}
        step_dir *= -1;
    }

private:
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

    // changeble variables
    double sx=100;
    double sy=60;

    // dirction of step n
    // swing leg == left if step_dir == 1
    int step_dir = 1;
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

    std::vector< std::vector<double> > right_foot_motion;
    std::vector< std::vector<double> > left_foot_motion;

    void COM_traj_zero();

    rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr pub_motion_list_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_trig_;
};

