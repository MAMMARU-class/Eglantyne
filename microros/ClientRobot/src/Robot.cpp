#include "Robot.h"
#include "rosidl_runtime_c/string_functions.h"

// using JointTrajectoryPoint = trajectory_msgs__msg__JointTrajectoryPoint;

Robot::Robot(){}

void Robot::setSerial(IcsHardSerialClass* serial1, IcsHardSerialClass* serial2){
    this->serial1 = serial1;
    this->serial2 = serial2;
}
void Robot::init_home(float t){
    vector<double> current;
    vector<double> home;
    for(auto* link : link_set){
        current.push_back(link->getq_current());
        home.push_back(link->getq_home());
    }
    
    int step_end = t*1000 / CONTROL_CYCLE;
    for(int step=0; step<step_end; step++){
        vector<double> motion(current.size());
        for(int id=0; id<current.size(); id++){
            motion[id] = ( current[id]*( (float)(step_end-step) ) + home[id]*( (float)(step) ) ) / (float)step_end;
        }
        // add_motion(motion);
        move_all(motion);
        delay(20);
    }
}
// void Robot::init_zero(float t){
//     vector<double> current;
//     vector<double> zero;
//     for(auto* link : link_set){
//         current.push_back(link->getq_current());
//         zero.push_back(0.0);
//     }
    
//     int step_end = t*1000 / CONTROL_CYCLE;
//     for(int step=0; step<step_end; step++){
//         vector<double> motion(current.size());
//         for(int id=0; id<current.size(); id++){
//             motion[id] = ( current[id]*( (float)(step_end-step) ) + zero[id]*( (float)(step) ) ) / (float)step_end;
//         }
//         add_motion(motion);
//     }
//     // vector<double> init_end_home = zero;
//     // init_end_home.push_back(INIT_HOME);
//     // add_motion(init_end_home);
// }

// void Robot::add_motion(vector<double> motion){
//     trajectorypoint_list.push_back(motion);
// }
// void Robot::add_trajectorypoint_list(vector< JointTrajectoryPoint > new_trajectorypoint_list){
//     trajectorypoint_list.insert(trajectorypoint_list.end(), new_trajectorypoint_list.begin(), new_trajectorypoint_list.end());
// }

// int Robot::next_motion(){
//     if(!trajectorypoint_list.empty()){
//         JointTrajectoryPoint next_motion = trajectorypoint_list.front();
//         trajectorypoint_list.erase(trajectorypoint_list.begin());

//         if( next_motion.data.positions.size() > this->link_set.size() ){
//             int trig = next_motion.back();
//             next_motion.erase(next_motion.end()-1);
//             move_all(next_motion.data.positions);
//             return trig;
//         }else{
//             move_all(next_motion.data.positions);
//             return NO_TRIG;
//         }
//     }else{ return NO_TRIG; }
// }
void Robot::move_all( vector<double> ){
    for(int id=0; id<link_set.size(); id++){
        link_set[id]->move(motion[id]);
    }
}
