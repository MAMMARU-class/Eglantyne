#include "Robot.h"
#include "rosidl_runtime_c/string_functions.h"

Robot::Robot(){}

void Robot::setSerial(IcsHardSerialClass* serial1, IcsHardSerialClass* serial2){
    this->serial1 = serial1;
    this->serial2 = serial2;
}

array<float, 18> Robot::init_home(float t){
    // vector<float> current;
    // vector<float> home;
    array<float, 18> current;
    array<float, 18> home;

    int link_num = 0;
    for(auto* link : link_set){
        // current.push_back(link->getq_current());
        // home.push_back(link->getq_home());
        current[link_num] = link->getq_current();
        home[link_num] = link->getq_home();
        link_num++;
    }
    
    int step_end = t*1000 / CONTROL_CYCLE;
    for(int step=0; step<step_end; step++){
        // vector<float> motion(current.size());
        array<float, 18> motion;
        for(int id=0; id<current.size(); id++){
            motion[id] = ( current[id]*( (float)(step_end-step) ) + home[id]*( (float)(step) ) ) / (float)step_end;
        }
        move_all(motion);
        delay(CONTROL_CYCLE);
    }
    return home;
}

// void Robot::move_all( vector<float> motion ){
//     for(int id=0; id<link_set.size(); id++){
//         link_set[id]->move(motion[id]);
//     }
// }
void Robot::move_all(array<float, 18> motion){
    for(int id=0; id<link_set.size(); id++){
        link_set[id]->move(motion[id]);
    }
}
