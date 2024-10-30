#include "Robot.h"
#include "rosidl_runtime_c/string_functions.h"

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
        move_all(motion);
        delay(20);
    }
}

void Robot::move_all( vector<double> motion ){
    for(int id=0; id<link_set.size(); id++){
        link_set[id]->move(motion[id]);
    }
}
