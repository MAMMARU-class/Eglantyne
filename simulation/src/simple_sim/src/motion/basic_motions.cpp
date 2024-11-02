#include "basic_motions.hpp"

void BasicMotion::update_joy_order(const sensor_msgs::msg::Joy msg){
}

void BasicMotion::motion_handler(const Int32 trig){
    switch(trig.data){
        case WAKE_FRONT:
            this->wake_front();
            break;
        case WAKE_BACK:
            this->wake_back();
            break;
        default:
            break;
    }
}

double deg2rad(double deg){
    return deg*3.1415 / 180;
}

void wake_front(){
    trajectory_msgs::msg::JointTrajectory motion;

    vectord key_motions[5];
    key_motions[0] = this->home;
    key_motions[1] = {-20, 0, -180,
                 -20, 0, -180,
                 0, 0, -150, 160, -70, 0,
                 0, 0, -150, 160, -70, 0}; 
    key_motions[2] = {-10, -90, -170,
                 -10,  90, -170,
                 0, -90, -150, 160, -70,  80,
                 0,  90, -150, 160, -70, -80};
    key_motions[3] = {-10, -90, 0,
                 -10,  90, 0,
                 0, -90, -150, 160, -70,  80,
                 0,  90, -150, 160, -70, -80};
    key_motions[4] = this->home;

    trajectory_msgs::msg::JointTrajectoryPoint pos;
    for(int i=0; i<50; i++){
        pos.positions = ( key_motions[0]*(50-i) + key_motions[1]*i ) / 50;
        pos.positions.push_back(0);
        motion.points.push_back(pos);
    }
    for(int i=0; i<50; i++){
        pos.positions = ( key_motions[1]*(50-i) + key_motions[2]*i ) / 50;
        pos.positions.push_back(0);
        motion.points.push_back(pos);
    }
    for(int i=0; i<10; i++){
        pos.positions = ( key_motions[2]*(50-i) + key_motions[3]*i ) / 50;
        pos.positions.push_back(0);
        motion.points.push_back(pos);
    }
    for(int i=0; i<5; i++){
        for(int j=0; j<10; j++){
            pos.positions = ( key_motions[3]*(50-i*10) + key_motions[4]*i*10 ) / 50;
            if(i==4 && j==9){pos.positions.push_back(STAY);
            }else{pos.positions.push_back(0);}
            motion.points.push_back(pos);
        }
    }
    
    pub_motion_list_->publish(motion);
}

void wake_back(){}
