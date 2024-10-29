#include <Arduino.h>
#include <vector>
#include "RobotLink.h"
#include "motion_trigger.h"
#include "Kinematics.h"

#define CONTROL_CYCLE 20 // ms

class MotionController{
public:
    MotionController();
    void setSerial(IcsHardSerialClass* serial1, IcsHardSerialClass* serial2);
    void setLink();
    std::vector<RobotLink*> getLinkSet();

    void init_home(double t);
    void init_zero(double t);
    void add_motion(std::vector<double> motion);
    void add_motion_list(std::vector< std::vector<double> > new_motion_list);
    // get next motion from list, and move robot
    std::vector<double> next_motion();

    // motion trigger
    void setTrig(int trig);
    int getTrig();

    void move_all(std::vector<double> motion);
    void move_all_q();
    void move_legs();
    void move_arms();


    // motions
    // basic

    void InitWalkMotion();
    void oneWalkMotion(double joy_sx, double joy_sy);


private:
    // serial
    IcsHardSerialClass* serial1;
    IcsHardSerialClass* serial2;

    std::vector< std::vector<double> > motion_list;
    std::vector<double> prev_motion;
    int MotionTrig;

    // link object
    std::vector<RobotLink*> link_set;
    RobotLink body;

    RobotLink arm_pitch_right;
    RobotLink arm_roll_right;
    RobotLink hand_right;

    RobotLink arm_pitch_left;
    RobotLink arm_roll_left;
    RobotLink hand_left;

    RobotLink leg_yaw_right;
    RobotLink leg_roll_right;
    RobotLink leg_upper_right;
    RobotLink leg_under_right;
    RobotLink foot_pitch_right;
    RobotLink foot_roll_right;

    RobotLink leg_yaw_left;
    RobotLink leg_roll_left;
    RobotLink leg_upper_left;
    RobotLink leg_under_left;
    RobotLink foot_pitch_left;
    RobotLink foot_roll_left;

    double BaseToCOM;
    double EndToFoot;
    double FulcToFootJoint;

    // これ、いらないのでは？
    // convert 18 axis position info into one number
    // each position has max about 6 min 0.0001
    // prepare 6 bit for each pos, first bit is pos(1)/neg(0)
    // for example, 3.2543 rad -> 132543
    // long home_position;
};