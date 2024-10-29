#ifndef robot_h
#define robot_h

#include <Arduino.h>
#include <vector>
#include <string>
#include "RobotLink.h"
#include "motion_trig.h"
#include <trajectory_msgs/msg/joint_trajectory.h>

#define CONTROL_CYCLE 20 // ms
using JointTrajectoryPoint = trajectory_msgs__msg__JointTrajectoryPoint;
using std::vector;

class Robot{
public:
    Robot();
    void setSerial(IcsHardSerialClass* serial1, IcsHardSerialClass* serial2);
    void setLink();

    void init_home(float t);
    void init_zero(float t);
    // void add_motion(vector<double> motion);
    void add_trajectorypoint_list(vector< vector<double> > new_trajectorypoint_list);

    int next_motion();
    void move_all(vector<double> motion);

private:
    // serial
    IcsHardSerialClass* serial1;
    IcsHardSerialClass* serial2;

    vector< JointTrajectoryPoint > trajectorypoint_list;

    // link object
    vector<RobotLink*> link_set;
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
};

#endif
