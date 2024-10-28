#include "../MotionController.h"

using Eigen::Vector2d; using Eigen::Vector3d;

// fixed variables
#define Z 237.4 // grand to COM
#define H 15.0 // foot height while walking 
#define G (9.81 * 1000) // mm/s^2

#define Tsup 0.8 // s
#define Tc std::sqrt(Z / G)
#define C std::cosh(Tsup / Tc)
#define S std::sinh(Tsup / Tc)
#define COEF_A 10
#define COEF_B 1
#define D ( COEF_A * (C-1)*(C-1) + COEF_B * (S/Tc)*(S/Tc) )

static double sx=100;
static double sy=60;

// dirction of step n
// swing leg == left if step_dir == 1
static int step_dir;
static Vector2d m2_step; // foot pos at n-2 step (origin: n-1 step)
static Vector2d aim_step; // foot pos aim of on going walk (n step. origin: n-1 step)
static Vector2d p1_step; // foot pos at n+1 step (origin: n-1 step)

static Vector2d COM_p_start;
static Vector2d COM_v_start;
static Vector2d COM_p_aim;
static Vector2d COM_v_aim;

static std::vector< Vector3d > COM_traj;
static std::vector< Vector3d > COM_traj_next;
static std::vector< Vector3d > swing_foot_traj;
static std::vector< Vector3d > body_to_fixed_foot_traj;
static std::vector< Vector3d > body_to_swing_foot_traj;

static std::vector< std::vector<double> > foot_motion;

static void calc_COM_traj_zero();
void MotionController::InitWalkMotion(){
    step_dir   = -1;
    m2_step   << 0, 60;
    aim_step  << 0, 60;
    p1_step   << 0, 0;
    COM_p_aim << 0, 30;
    COM_v_aim << 0, -190;
    COM_traj_next = {};
    calc_COM_traj_zero();
}

// foot pos at n+1 step
static void calc_foot_pos();
// COM traj at n step
static void calc_COM_traj_next();
// swing leg traj at n step
static void calc_swing_foot_traj();
// integrate COM traj with foot traj
static void integrate_traj();
// convert trajectory to joint angle for each
static void traj_to_motion();

void MotionController::oneWalkMotion(double joy_sx, double joy_sy){
    setTrig(STAY);
    sx = joy_sx; sy = joy_sy;

    std::vector<double> arm_angle;
    for(int id=0; id<6; id++){
        double angle = this->link_set[id]->getq_home();
        arm_angle.push_back(angle);
    }

    if (COM_traj_next.empty()){
        InitWalkMotion();
    }

    COM_traj = COM_traj_next;
    COM_traj_next = {};
    calc_foot_pos();
    calc_COM_traj_next();
    calc_swing_foot_traj();
    init_zero(3);
    // void integrate_traj();
    // void traj_to_motion();

    // bool is_flag=true;
    // while (!foot_motion.empty()){
    //     std::vector<double> foot_positions = foot_motion.front();
    //     foot_motion.erase(foot_motion.begin());
        
    //     std::vector<double> positions = arm_angle;
    //     positions.insert(positions.end(), foot_positions.begin(), foot_positions.end());
    //     if(is_flag){
    //         positions.push_back(WALK);
    //         is_flag = false;
    //     }
    //     add_motion(positions);
    // }
}


static void calc_foot_pos(){
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

static void calc_COM_traj_zero()
{
    double trajy;
    double yp_0 = -sy/2;
    double yv_0 = -1 * yp_0*(C-1)/(Tc*S);
    for(double t=0; t<=Tsup+0.01; t+=CONTROL_CYCLE*0.001){
        trajy = yp_0 * cosh(t/Tc) + Tc*yv_0*sinh(t/Tc);
        COM_traj_next.push_back({0,trajy, Z});
    }
    COM_p_aim << 0, -trajy; COM_v_aim << 0, -yv_0;
}

static void calc_COM_traj_next(){
    double trajx, trajy;
    for(double t=0; t<=Tsup+0.01; t+=CONTROL_CYCLE*0.001){
        trajx = ( COM_p_start(0) - aim_step(0) )*cosh(t/Tc) + Tc*COM_v_start(0)*sinh(t/Tc);
        trajy = ( COM_p_start(1) - aim_step(1) )*cosh(t/Tc) + Tc*COM_v_start(1)*sinh(t/Tc);
        COM_traj_next.push_back({trajx, trajy, Z});
    }
}

static void calc_swing_foot_traj(){
    double trajx, trajy, trajz;
    double lx = aim_step(0) - m2_step(0);
    double ly = aim_step(1) - m2_step(1);

    for(double t=0; t<=Tsup+0.01; t+=CONTROL_CYCLE*0.001){
        double theta = PI * t/Tsup;
        trajx = lx/2 * (1-cos(theta)) + m2_step(0);
        trajy = ly/2 * (1-cos(theta)) + m2_step(1);
        trajz = H * sin(theta);
        swing_foot_traj.push_back({trajx, trajy, trajz});
    }
}

static void integrate_traj(double BaseToCOM, double EndToFoot, double FlucToFootJoint){
    Vector3d body_to_fixed_foot_point;
    Vector3d body_to_swing_foot_point;

    while(!COM_traj.empty() && !swing_foot_traj.empty()){
        body_to_fixed_foot_point = -1 * (*COM_traj.begin());
        body_to_fixed_foot_point(0) += FlucToFootJoint;
        body_to_fixed_foot_point(2) += BaseToCOM + EndToFoot;
        // discard value under 0.1
        body_to_fixed_foot_point(0) = std::floor(body_to_fixed_foot_point(0) * 10) / 10.0 ;
        body_to_fixed_foot_point(1) = std::floor(body_to_fixed_foot_point(1) * 10) / 10.0 ;
        body_to_fixed_foot_point(2) = std::floor(body_to_fixed_foot_point(2) * 10) / 10.0 ;

        body_to_fixed_foot_traj.push_back(body_to_fixed_foot_point);

        body_to_swing_foot_point = -1 * (*COM_traj.begin()) + (*swing_foot_traj.begin());
        body_to_swing_foot_point(0) += FlucToFootJoint;
        body_to_swing_foot_point(2) += BaseToCOM + EndToFoot;
        // discard value under 0.1
        body_to_swing_foot_point(0) = std::floor(body_to_swing_foot_point(0) * 10) / 10.0 ;
        body_to_swing_foot_point(1) = std::floor(body_to_swing_foot_point(1) * 10) / 10.0 ;
        body_to_swing_foot_point(2) = std::floor(body_to_swing_foot_point(2) * 10) / 10.0 ;

        body_to_swing_foot_traj.push_back(body_to_swing_foot_point);

        COM_traj.erase(COM_traj.begin());
        swing_foot_traj.erase(swing_foot_traj.begin());
    }

    COM_traj = {};
    swing_foot_traj = {};
}

static void traj_to_motion(RobotLink* endeffector_right, RobotLink* endeffector_left, std::vector<RobotLink*> link_list){
    Eigen::Matrix3d R_ref;
    R_ref << 1, 0, 0,
             0, 1, 0,
             0, 0, 1;
    
    while(!body_to_fixed_foot_traj.empty() && !body_to_swing_foot_traj.empty()){
        if (step_dir == 1){
            // fixed... right, swing... left
            Kinematics::inverse(endeffector_right, (*body_to_fixed_foot_traj.begin()), R_ref);
            Kinematics::inverse(endeffector_left,  (*body_to_swing_foot_traj.begin()), R_ref);
        }else{
            // fixed... left, swing... right
            Kinematics::inverse(endeffector_left,  (*body_to_fixed_foot_traj.begin()), R_ref);
            Kinematics::inverse(endeffector_right, (*body_to_swing_foot_traj.begin()), R_ref);
        }
        body_to_fixed_foot_traj.erase(body_to_fixed_foot_traj.begin());
        body_to_swing_foot_traj.erase(body_to_swing_foot_traj.begin());
        std::vector<double> foot_pos;
        for (auto* link : link_list){
            foot_pos.push_back(link->getq());
        }
        foot_motion.push_back(foot_pos);

    }
    body_to_fixed_foot_traj = {};
    body_to_swing_foot_traj = {};
}
