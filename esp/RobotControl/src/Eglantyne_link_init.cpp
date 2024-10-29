#include "MotionController.h"

std::vector<RobotLink*> MotionController::getLinkSet(){ return link_set; }

void MotionController::setLink(){
    body.setname("body");

    arm_pitch_right.setDefault("arm_pitch_right", 0,0,64.98,  0,1,0, &body);
    arm_pitch_right.setInitialPositionsDeg(9400, 30.0 ,0,0);
    arm_pitch_right.setMotor(serial1, 1, FORWARD);

    arm_roll_right.setDefault("arm_roll_right",   0,-62.2,0,  1,0,0, &arm_pitch_right);
    arm_roll_right.setInitialPositionsDeg(10050, -20.0 ,0,0);
    arm_roll_right.setMotor(serial1, 2, FORWARD);

    hand_right.setDefault("hand_right",           0,0,-82.84, 0,1,0, &arm_roll_right);
    hand_right.setInitialPositionsDeg(5700, -90.0 ,0,0);
    hand_right.setMotor(serial1, 3, REVERSE);



    arm_pitch_left.setDefault("arm_pitch_left", 0,0,64.98,  0,1,0, &body);
    arm_pitch_left.setInitialPositionsDeg(5930, 30,0,0);
    arm_pitch_left.setMotor(serial1, 4, REVERSE);

    arm_roll_left.setDefault("arm_roll_left",   0, 62.2,0,  1,0,0, &arm_pitch_left);
    arm_roll_left.setInitialPositionsDeg(10300, 20,0,0);
    arm_roll_left.setMotor(serial1, 5, REVERSE);

    hand_left.setDefault("hand_left",           0,0,-82.84, 0,1,0, &arm_roll_left);
    hand_left.setInitialPositionsDeg(6350, -90.0 ,0,0);
    hand_left.setMotor(serial1, 6, REVERSE);



    leg_yaw_right.setDefault("leg_yaw_right",       0,-30,0,    0,0,1, &body);
    leg_yaw_right.setInitialPositionsDeg(7380, 0,0,0);
    leg_yaw_right.setMotor(serial1, 7, REVERSE);

    leg_roll_right.setDefault("leg_roll_right",     0,0,-22.2,  1,0,0, &leg_yaw_right);
    leg_roll_right.setInitialPositionsDeg(9000, 0,0,0);
    leg_roll_right.setMotor(serial2, 1, FORWARD);

    leg_upper_right.setDefault("leg_upper_right",   0,0,-26.01, 0,1,0, &leg_roll_right);
    leg_upper_right.setInitialPositionsDeg(9850, -18.31, 0,0);
    leg_upper_right.setMotor(serial2, 2, FORWARD);

    leg_under_right.setDefault("leg_under_right",   0,0,-78.02, 0,1,0, &leg_upper_right);
    leg_under_right.setInitialPositionsDeg(4200, 65.8, 0,0);
    leg_under_right.setMotor(serial2, 3, FORWARD);

    foot_pitch_right.setDefault("foot_pitch_right", 0,0,-78.02, 0,1,0,  &leg_under_right);
    foot_pitch_right.setInitialPositionsDeg(5890, -45.88, 0,0);
    foot_pitch_right.setMotor(serial2, 4, REVERSE);

    foot_roll_right.setDefault("foot_roll_right",   0,0,0,      1,0,0, &foot_pitch_right);
    foot_roll_right.setInitialPositionsDeg(7600, 0,0,0);
    foot_roll_right.setMotor(serial2, 5, REVERSE);



    leg_yaw_left.setDefault("leg_yaw_left",       0,30,0,     0,0,1, &body);
    leg_yaw_left.setInitialPositionsDeg(7500, 0,0,0);
    leg_yaw_left.setMotor(serial1, 8, REVERSE);

    leg_roll_left.setDefault("leg_roll_left",     0,0,-22.2,  1,0,0, &leg_yaw_left);
    leg_roll_left.setInitialPositionsDeg(8330, 0,0,0);
    leg_roll_left.setMotor(serial2, 6, REVERSE);

    leg_upper_left.setDefault("leg_upper_left",   0,0,-26.01, 0,1,0, &leg_roll_left);
    leg_upper_left.setInitialPositionsDeg(9750, -18.31, 0,0);
    leg_upper_left.setMotor(serial2, 7, FORWARD);

    leg_under_left.setDefault("leg_under_left",   0,0,-78.02, 0,1,0, &leg_upper_left);
    leg_under_left.setInitialPositionsDeg(4320, 65.8, 0,0);
    leg_under_left.setMotor(serial2, 8, FORWARD);

    foot_pitch_left.setDefault("foot_pitch_left", 0,0,-78.02, 0,1,0, &leg_under_left);
    foot_pitch_left.setInitialPositionsDeg(6090, -45.88, 0,0);
    foot_pitch_left.setMotor(serial2, 9, REVERSE);

    foot_roll_left.setDefault("foot_roll_left",   0,0,0,      1,0,0, &foot_pitch_left);
    foot_roll_left.setInitialPositionsDeg(7400, 0,0,0);
    foot_roll_left.setMotor(serial2, 10, REVERSE);

    BaseToCOM = 40.0;
    EndToFoot = 37.4;
    FulcToFootJoint = -33.0;

    this->link_set = {
        &arm_pitch_right, &arm_roll_right, &hand_right,
        &arm_pitch_left, &arm_roll_left, &hand_left,
        &leg_yaw_right, &leg_roll_right, &leg_upper_right, &leg_under_right, &foot_pitch_right, &foot_roll_right,
        &leg_yaw_left, &leg_roll_left, &leg_upper_left, &leg_under_left, &foot_pitch_left, &foot_roll_left };
    
    arm_pitch_right.move_q();
    // arm_roll_right.setq(-0.3);
    arm_roll_right.move_q();
    // hand_right.setq(0);
    hand_right.move_q();
  
    // arm_pitch_left.setq(0);
    arm_pitch_left.move_q();
    // arm_roll_left.setq(-0.3);
    arm_roll_left.move_q();
    // hand_left.setq(0);
    hand_left.move_q();
  
    // leg_yaw_right.setq(0);
    leg_yaw_right.move_q();
    // leg_roll_right.setq(0);
    leg_roll_right.move_q();
    // leg_upper_right.setq(-30.0 * 3.14 / 180.0);
    leg_upper_right.move_q();
    // leg_under_right.setq(-60.0 * 3.14 / 180.0);
    leg_under_right.move_q();
    // foot_pitch_right.setq(0);
    foot_pitch_right.move_q();
    // foot_roll_right.setq(0);
    foot_roll_right.move_q();
  
    // leg_yaw_left.setq(0);
    leg_yaw_left.move_q();
    // leg_roll_left.setq(0);
    leg_roll_left.move_q();
    // leg_upper_left.setq(-30.0 * 3.14 / 180.0);
    leg_upper_left.move_q();
    // leg_under_left.setq(-60.0 * 3.14 / 180.0);
    leg_under_left.move_q();
    // foot_pitch_left.setq(0);
    foot_pitch_left.move_q();
    // foot_roll_left.setq(0);
    foot_roll_left.move_q();

}

