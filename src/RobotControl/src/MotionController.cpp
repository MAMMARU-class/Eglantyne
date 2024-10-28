#include "MotionController.h"

MotionController::MotionController(){}

void MotionController::setSerial(IcsHardSerialClass* serial1, IcsHardSerialClass* serial2){
    this->serial1 = serial1;
    this->serial2 = serial2;
    // serial1->begin(); serial2->begin();
}
void MotionController::init_home(double t){
    std::vector<double> current;
    std::vector<double> home;
    for(auto* link : link_set){
        current.push_back(link->getq_current());
        home.push_back(link->getq_home());
    }
    
    int step_end = t*1000 / CONTROL_CYCLE;
    for(int step=0; step<step_end; step++){
        std::vector<double> motion(current.size());
        for(int id=0; id<current.size(); id++){
            motion[id] = ( current[id]*( (double)(step_end-step) ) + home[id]*( (double)(step) ) ) / (double)step_end;
        }
        add_motion(motion);
    }
}

void MotionController::add_motion(std::vector<double> motion){
    motion_list.push_back(motion);
}
void MotionController::add_motion_list(std::vector< std::vector<double> > new_motion_list){
    motion_list.insert(motion_list.end(), new_motion_list.begin(), new_motion_list.end());
}
std::vector<double> MotionController::next_motion(){
    if(!motion_list.empty()){
        std::vector<double> next_motion = motion_list.front();
        motion_list.erase(motion_list.begin());

        move_all(next_motion);
        prev_motion = next_motion;
        return next_motion;
    }else{
        // move_all(prev_motion);
        return {};
    }
}
void MotionController::setTrig(int trig){ this->MotionTrig = trig; }
int  MotionController::getTrig(){ return this->MotionTrig; }

void MotionController::move_all(std::vector<double> motion){
    if( motion.size() > this->link_set.size() ){
        setTrig((int)motion.back());
        motion.erase(motion.end()-1);
    }
    for(int id=0; id<link_set.size(); id++){
        link_set[id]->move(motion[id]);
    }
    // move_all_q();
}
void MotionController::move_all_q(){
    for(auto* link : link_set){
        link->move_q();
    }
}
// void MotionController::move_legs(){
//     for(int i=6; i<18; i++){ link_set[i]->move(); }
// }
// void MotionController::move_arms(){
//     for(int i=0; i<6; i++){ link_set[i]->move(); }
// }
