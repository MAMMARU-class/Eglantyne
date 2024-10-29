#include "walk_test_func.hpp"
using std::cosh; using std::sinh;
using std::cos;  using std::sin;
using Eigen::Vector2d; using Eigen::Vector3d;

void WalkTest::calc_foot_pos()
{
    // shift foot step and com info
    COM_p_start = COM_p_aim - aim_step;
    COM_v_start = COM_v_aim;

    m2_step = -aim_step;
    aim_step = p1_step - aim_step;
    Vector2d ofs{sx, -step_dir*sy};
    p1_step =aim_step + ofs;

    // model aim (origin: n step)
    COM_p_aim << sx / 2,
                 -step_dir * sy / 2;
    COM_v_aim << (C + 1) / (Tc*S) * COM_p_aim(0),
                 (C - 1) / (Tc*S) * COM_p_aim(1); // reverse v_y as n-1 step is on the other side of n step
    
    COM_p_aim += aim_step;

    aim_step(0) = - COEF_A*(C-1) / D * (COM_p_aim(0) - C*COM_p_start(0) - Tc*S*COM_v_start(0))
                  - COEF_B*S/( Tc*D ) * (COM_v_aim(0) - S/Tc * COM_p_start(0) - C*COM_v_start(0));
    aim_step(1) = - COEF_A*(C-1) / D * (COM_p_aim(1) - C*COM_p_start(1) - Tc*S*COM_v_start(1))
                  - COEF_B*S/( Tc*D ) * (COM_v_aim(1) - S/Tc * COM_p_start(1) - C*COM_v_start(1));
    
    COM_p_aim(0) = C    * COM_p_start(0) + Tc*S*COM_v_start(0) + (1-C)*aim_step(0);
    COM_v_aim(0) = S/Tc * COM_p_start(0) +    C*COM_v_start(0) - S/Tc *aim_step(0);

    COM_p_aim(1) = C    * COM_p_start(1) + Tc*S*COM_v_start(1) + (1-C)*aim_step(1);
    COM_v_aim(1) = S/Tc * COM_p_start(1) +    C*COM_v_start(1) - S/Tc *aim_step(1);
}

void WalkTest::COM_traj_zero()
{
    double trajy;
    double yp_0 = -sy/2;
    double yv_0 = yp_0*(C-1)/(Tc*S);
    for(double t=0; t<Tsup; t+=INTERVAL*0.001){
        trajy = yp_0 * cosh(t/Tc) + Tc*yv_0*sinh(t/Tc);
        COM_traj_next.push_back({0,trajy, Z});
    }
}

// COM traj at n step
void WalkTest::calc_COM_traj_next()
{
    double trajx, trajy;
    for(double t=0; t<Tsup; t+=INTERVAL*0.001){
        trajx = ( COM_p_start(0) - aim_step(0) )*cosh(t/Tc) + Tc*COM_v_start(0)*sinh(t/Tc);
        trajy = ( COM_p_start(1) - aim_step(1) )*cosh(t/Tc) + Tc*COM_v_start(1)*sinh(t/Tc);
        COM_traj_next.push_back({trajx, trajy, Z});
    }
}

void WalkTest::calc_swing_foot_traj()
{
    using std::numbers::pi;
    double trajx, trajy, trajz;
    double lx = aim_step(0) - m2_step(0);
    double ly = aim_step(1) - m2_step(1);

    for(double t=0; t<Tsup; t+=INTERVAL*0.001){
        double theta = pi * t/Tsup;
        trajx = lx/2 * (1-cos(theta)) + lx/2 + p2_step(0);
        trajy = ly/2 * (1-cos(theta)) + ly/2 + p2_step(1);
        trajz = H * sin(theta);
        swing_foot_traj.push_back({trajx, trajy, trajz});
    }
}

void WalkTest::integrate_traj()
{
    Vector3d body_to_fixed_foot_point;
    Vector3d body_to_swing_foot_point;
    while(!COM_traj.empty() && !swing_foot_traj.empty()){
        body_to_fixed_foot_point = -COM_traj.begin();
        body_to_fixed_foot_point(2) += BASE_TO_COM + END_TO_FOOT;
        body_to_fixed_foot_foot_traj.push_back(body_to_fixed_foot_point);

        body_to_swing_foot_point = -COM_traj.begin() + swing_foot_traj.begin();
        body_to_swing_foot_point(2) += BASE_TO_COM + END_TO_FOOT;
        body_to_swing_foot_foot_traj.push_back(body_to_swing_foot_point);

        COM_traj.erase(COM_traj.begin());
        swing_foot_traj.erase(swing_foot_traj.begin());
    }
    // initialize
    COM_traj = {};
    swing_foot_traj = {};
}

void WalkTest::traj_to_motion()
{
    std::vector<double> fixed_leg_motion;
    std::vector<double> swing_leg_motion;
    

}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<WalkTest>());
    rclcpp::shutdown();
    return 0;
}

void WalkTest::setup_link(){
    body.setname("body");
    leg_yaw_right.setDefault("leg_yaw_right", 0,-30,0, 0,0,1, 0, &body);
    leg_roll_right.setDefault("leg_roll_right", 0,0,-22.2, 1,0,0, 0, &hip_yaw);
    leg_upper_right.setDefault("leg_upper_right", 0,0,-26.01, 0,1,0, -25.22, &hip_roll);
    leg_under_right.setDefault("leg_under_right", 0,0,-78.02, 0,1,0, 83.34, &hip_pitch);
    foot_pitc_right.setDefault("foot_pitch_right", 0,0,-78.02, 0,1,0, -58.12, &nee_pitch);
    foot_roll_right.setDefault("foot_roll_right", 0,0,0, 1,0,0, 0, &toe_pitch);

    leg_yaw_right.setDefault("leg_yaw_right", 0,-30,0, 0,0,1, 0, &body);
    leg_roll_right.setDefault("leg_roll_right", 0,0,-22.2, 1,0,0, 0, &hip_yaw);
    leg_upper_right.setDefault("leg_upper_right", 0,0,-26.01, 0,1,0, -25.22, &hip_roll);
    leg_under_right.setDefault("leg_under_right", 0,0,-78.02, 0,1,0, 83.34, &hip_pitch);
    foot_pitc_right.setDefault("foot_pitch_right", 0,0,-78.02, 0,1,0, -58.12, &nee_pitch);
    foot_roll_right.setDefault("foot_roll_right", 0,0,0, 1,0,0, 0, &toe_pitch);
}
