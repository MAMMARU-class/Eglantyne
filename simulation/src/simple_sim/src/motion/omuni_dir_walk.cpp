#include "omuni_dir_walk.hpp"
using std::cosh; using std::sinh;
using std::cos;  using std::sin;
using Eigen::Vector2d; using Eigen::Vector3d; using Eigen::Vector4d;
using Eigen::Matrix2d;

void OmuniDirWalk::update_joy_order(const sensor_msgs::msg::Joy msg){
    this->dx = X_MAX*msg.axes[1];
    this->dy = Y_MAX*msg.axes[0];
    this->dtheta = THETA_MAX*msg.axes[2];
}

void OmuniDirWalk::calc_foot_pos()
{
    RCLCPP_INFO(this->get_logger(), "n-2     step before: x: %f, y: %f", m2_step(0), m2_step(1));
    RCLCPP_INFO(this->get_logger(), "n (aim) step before: x: %f, y: %f", aim_step(0), aim_step(1));
    RCLCPP_INFO(this->get_logger(), "n+1     step before: x: %f, y: %f", p1_step(0), p1_step(1));
    // shift theta
    if( dtheta/abs(dtheta) == step_dir/abs(step_dir) ){ dtheta=0; }
    m2_theta = -aim_theta;
    aim_theta = p1_theta - aim_theta;
    p1_theta = aim_theta + dtheta;

    m_aim_XYRot << cos(-aim_theta), -sin(-aim_theta),
                   sin(-aim_theta),  cos(-aim_theta);
    p1_XYRot << cos(p1_theta), -sin(p1_theta),
                sin(p1_theta),  cos(p1_theta);

    // shift COM
    COM_p_start = COM_p_aim - aim_step;
    COM_p_start = m_aim_XYRot*COM_p_start;
    
    COM_v_start = COM_v_aim;
    COM_v_start = m_aim_XYRot*COM_v_start;

    // shift step
    m2_step = -aim_step;
    aim_step = p1_step - aim_step;

    sy = DEFALUT_Y+(step_dir*dy);
    if(sy < DEFALUT_Y){ sy=DEFALUT_Y; }
    Vector2d ofs{dx, -step_dir*sy};
    p1_step =aim_step + ofs;

    // model aim (origin: n-1 step)
    COM_p_aim << dx / 2,
                 -step_dir * sy / 2;
    COM_p_aim = p1_XYRot*COM_p_aim;

    COM_v_aim << (C + 1) / (Tc*S) * COM_p_aim(0),
                 (C - 1) / (Tc*S) * COM_p_aim(1); // reverse v_y as n-1 step is on the other side of n step
    COM_v_aim = p1_XYRot*COM_v_aim;

    // add aim_step after calculated velocity
    COM_p_aim += aim_step;


    aim_step(0) = - COEF_A*(C-1) / D * (COM_p_aim(0) - C*COM_p_start(0) - Tc*S*COM_v_start(0))
                  - COEF_B*S/( Tc*D ) * (COM_v_aim(0) - S/Tc * COM_p_start(0) - C*COM_v_start(0));
    aim_step(1) = - COEF_A*(C-1) / D * (COM_p_aim(1) - C*COM_p_start(1) - Tc*S*COM_v_start(1))
                  - COEF_B*S/( Tc*D ) * (COM_v_aim(1) - S/Tc * COM_p_start(1) - C*COM_v_start(1));
    
    COM_p_aim(0) = C    * COM_p_start(0) + Tc*S*COM_v_start(0) + (1-C)*aim_step(0);
    COM_v_aim(0) = S/Tc * COM_p_start(0) +    C*COM_v_start(0) - S/Tc *aim_step(0);

    COM_p_aim(1) = C    * COM_p_start(1) + Tc*S*COM_v_start(1) + (1-C)*aim_step(1);
    COM_v_aim(1) = S/Tc * COM_p_start(1) +    C*COM_v_start(1) - S/Tc *aim_step(1);

    RCLCPP_INFO(this->get_logger(), "dx : %f, dy : %f, dtheta : %f", dx, dy, dtheta);
    RCLCPP_INFO(this->get_logger(), "n-2 theta : %f, n theta : %f, n+1 theta : %f", m2_theta, aim_theta, p1_theta);
    RCLCPP_INFO(this->get_logger(), "n-2     step: x: %f, y: %f", m2_step(0),  m2_step(1));
    RCLCPP_INFO(this->get_logger(), "n (aim) step: x: %f, y: %f", aim_step(0), aim_step(1));
    RCLCPP_INFO(this->get_logger(), "n+1     step: x: %f, y: %f", p1_step(0),  p1_step(1));
}

void OmuniDirWalk::COM_traj_zero()
{
    double trajy;
    double yp_0 = -sy/2;
    double yv_0 = -1 * yp_0*(C-1)/(Tc*S);
    RCLCPP_INFO(this->get_logger(), "COM start: y: %f, dy/dt: %f ", yp_0, yv_0);
    for(double t=0; t<=Tsup+0.01; t+=INTERVAL*0.001){
        trajy = yp_0 * cosh(t/Tc) + Tc*yv_0*sinh(t/Tc);
        COM_traj_next.push_back({0,trajy, Z, 0});
        
        // RCLCPP_INFO(this->get_logger(), "COM trajectory: x: %d, y: %f ", 0, trajy);
    }
    COM_p_aim << 0, -trajy; COM_v_aim << 0, -yv_0;
}

// COM traj at n step
void OmuniDirWalk::calc_COM_traj_next()
{
    double trajx, trajy, trajtheta;
    for(double t=0; t<=Tsup+0.01; t+=INTERVAL*0.001){
        trajx = ( COM_p_start(0) - aim_step(0) )*cosh(t/Tc) + Tc*COM_v_start(0)*sinh(t/Tc);
        trajy = ( COM_p_start(1) - aim_step(1) )*cosh(t/Tc) + Tc*COM_v_start(1)*sinh(t/Tc);
        trajtheta = (p1_theta-aim_theta) * (Tsup-t)/Tsup;
        COM_traj_next.push_back({trajx, trajy, Z, trajtheta});

        // RCLCPP_INFO(this->get_logger(), "next COM trajectory: x: %f, y: %f ", trajx, trajy);
    }
}

void OmuniDirWalk::calc_swing_foot_traj()
{
    double trajx, trajy, trajz, trajtheta;
    double lx = aim_step(0) - m2_step(0);
    double ly = aim_step(1) - m2_step(1);

    for(double t=0; t<=Tsup+0.01; t+=INTERVAL*0.001){
        double theta = PI * t/Tsup;
        trajx = lx/2 * (1-cos(theta)) + m2_step(0);
        trajy = ly/2 * (1-cos(theta)) + m2_step(1);
        trajz = H * sin(theta);
        trajtheta = ( m2_theta*(Tsup-t) + aim_theta*t ) / Tsup;

        swing_foot_traj.push_back({trajx, trajy, trajz, trajtheta});

        // RCLCPP_INFO(this->get_logger(), "swing foot trajectory: x: %f, y: %f, z:%f ", trajx, trajy, trajz);
    }
}

void OmuniDirWalk::integrate_traj()
{
    Vector4d body_to_fixed_foot_point;
    Vector4d body_to_swing_foot_point;

    while(!COM_traj.empty() && !swing_foot_traj.empty()){
        body_to_fixed_foot_point = -1 * COM_traj.front();
        body_to_fixed_foot_point(0) += FULC_TO_FOOT_JOINT;
        body_to_fixed_foot_point(2) += BASE_TO_COM + END_TO_FOOT;

        body_to_fixed_foot_traj.push_back(body_to_fixed_foot_point);

        body_to_swing_foot_point = -1 * COM_traj.front() + swing_foot_traj.front();
        body_to_swing_foot_point(0) += FULC_TO_FOOT_JOINT;
        body_to_swing_foot_point(2) += BASE_TO_COM + END_TO_FOOT;

        body_to_swing_foot_traj.push_back(body_to_swing_foot_point);

        COM_traj.erase(COM_traj.begin());
        swing_foot_traj.erase(swing_foot_traj.begin());
    }
    
    // for (size_t i=0; i<body_to_fixed_foot_traj.size(); ++i){
    //     RCLCPP_INFO(this->get_logger(), "body to fixed leg trajectory: x: %f, y: %f, z:%f ",
    //         body_to_fixed_foot_traj[i](0), body_to_fixed_foot_traj[i](1), body_to_fixed_foot_traj[i](2));
    // }
    // for (size_t i=0; i<body_to_swing_foot_traj.size(); ++i){
    //     RCLCPP_INFO(this->get_logger(), "body to swing leg trajectory: x: %f, y: %f, z:%f ",
    //         body_to_swing_foot_traj[i](0), body_to_swing_foot_traj[i](1), body_to_swing_foot_traj[i](2));
    // }    

    // initialize
    COM_traj = {};
    swing_foot_traj = {};
}

Eigen::Matrix3d theta2Rot(double theta){
    Eigen::Matrix3d rotMat;
    rotMat << cos(theta), -sin(theta), 0,
              sin(theta),  cos(theta), 0,
              0,           0,          1;
    return rotMat;
}

void OmuniDirWalk::traj_to_motion()
{
    Eigen::Matrix3d R_ref_r, R_ref_l;
    
    while(!body_to_fixed_foot_traj.empty() && !body_to_swing_foot_traj.empty()){
        if (step_dir == 1){
            // fixed... right, swing... left
            Kinematics::inverse(&foot_roll_right, body_to_fixed_foot_traj.front().segment(0,3), theta2Rot(body_to_fixed_foot_traj.front()(3)));
            Kinematics::inverse(&foot_roll_left,  body_to_swing_foot_traj.front().segment(0,3), theta2Rot(body_to_swing_foot_traj.front()(3)));
        }else{
            // fixed... left, swing... right
            Kinematics::inverse(&foot_roll_left,  body_to_fixed_foot_traj.front().segment(0,3), theta2Rot(body_to_fixed_foot_traj.front()(3)));
            Kinematics::inverse(&foot_roll_right, body_to_swing_foot_traj.front().segment(0,3), theta2Rot(body_to_swing_foot_traj.front()(3)));
        }
        body_to_fixed_foot_traj.erase(body_to_fixed_foot_traj.begin());
        body_to_swing_foot_traj.erase(body_to_swing_foot_traj.begin());
        std::vector<double> foot_pos;
        for (auto* link : link_vec){
            foot_pos.push_back(link->getq());
        }
        foot_motion.push_back(foot_pos);

    }
    body_to_fixed_foot_traj = {};
    body_to_swing_foot_traj = {};
}

void OmuniDirWalk::pub_walk_trajectory(const std_msgs::msg::Int32 msg)
{
    if(msg.data != WALK){ return; }
    if (COM_traj_next.empty()){
        dx = 0; dy = 0; dtheta = 0;
        sy = DEFALUT_Y;
        step_dir = -1;
        m2_step << 0, DEFALUT_Y;  m2_theta = 0;
        aim_step << 0, DEFALUT_Y; aim_theta = 0;
        p1_step << 0, 0;          p1_theta = 0;
        COM_traj_zero();
    }

    COM_traj = COM_traj_next;
    COM_traj_next = {};
    RCLCPP_INFO(this->get_logger(), "calculate foot pos");
    calc_foot_pos();
    RCLCPP_INFO(this->get_logger(), "calculate swing foot trajectory");
    calc_swing_foot_traj();
    RCLCPP_INFO(this->get_logger(), "integrate COM and foot trajectory");
    integrate_traj();
    RCLCPP_INFO(this->get_logger(), "solve inverse kinematics");
    traj_to_motion();

    // motion includes { right arm (3), left arm (3), right foot (3), left foot(3) }
    trajectory_msgs::msg::JointTrajectory motion;
    int count = 0;
    while (!foot_motion.empty()){
        trajectory_msgs::msg::JointTrajectoryPoint pos;

        std::vector<double> foot_positions = (*foot_motion.begin());
        foot_motion.erase(foot_motion.begin());
        std::vector<double> positions = arm_angle;
        
        positions.insert(positions.end(), foot_positions.begin(), foot_positions.end());
        if(count == 1){positions.push_back(WALK);
        }else{positions.push_back(0);}
        pos.positions = positions;

        // print position data
        // std::stringstream ss;
        // for (size_t i=0; i<positions.size(); ++i){
        //     ss << positions[i];
        //     if (i < positions.size() - 1){ ss << ", "; }}
        // RCLCPP_INFO(this->get_logger(), "positions: [%s]", ss.str().c_str());
        
        motion.points.push_back(pos);
        count++;
    }
    
    // motion.points[5].positions[19] = WALK;
    pub_motion_list_ -> publish(motion);

    // initialize
    step_dir *= -1;
    calc_COM_traj_next();
}


void OmuniDirWalk::setup_link(){
    body.setname("body");
    leg_yaw_right.setDefault("leg_yaw_right", 0,-30,0, 0,0,1, 0, &body);
    leg_roll_right.setDefault("leg_roll_right", 0,0,-22.2, 1,0,0, 0, &leg_yaw_right);
    leg_upper_right.setDefault("leg_upper_right", 0,0,-26.01, 0,1,0, -18.31 * (PI / 180), &leg_roll_right);
    leg_under_right.setDefault("leg_under_right", 0,0,-78.02, 0,1,0, 65.8 * (PI / 180), &leg_upper_right);
    foot_pitch_right.setDefault("foot_pitch_right", 0,0,-78.02, 0,1,0, -45.88 * (PI / 180), &leg_under_right);
    foot_roll_right.setDefault("foot_roll_right", 0,0,0, 1,0,0, 0, &foot_pitch_right);

    leg_yaw_left.setDefault("leg_yaw_left", 0,30,0, 0,0,1, 0, &body);
    leg_roll_left.setDefault("leg_roll_left", 0,0,-22.2, 1,0,0, 0, &leg_yaw_left);
    leg_upper_left.setDefault("leg_upper_left", 0,0,-26.01, 0,1,0, -18.31 * (PI / 180), &leg_roll_left);
    leg_under_left.setDefault("leg_under_left", 0,0,-78.02, 0,1,0, 65.8 * (PI / 180), &leg_upper_left);
    foot_pitch_left.setDefault("foot_pitch_left", 0,0,-78.02, 0,1,0, -45.88 * (PI / 180), &leg_under_left);
    foot_roll_left.setDefault("foot_roll_left", 0,0,0, 1,0,0, 0, &foot_pitch_left);

    link_vec = {&leg_yaw_right, &leg_roll_right, &leg_upper_right, &leg_under_right, &foot_pitch_right, &foot_roll_right,
                &leg_yaw_left,  &leg_roll_left,  &leg_upper_left,  &leg_under_left,  &foot_pitch_left,  &foot_roll_left  };
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OmuniDirWalk>());
    rclcpp::shutdown();
    return 0;
}
