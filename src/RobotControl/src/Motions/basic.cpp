#include "../MotionController.h"

std::vector< std::vector<double> > MotionController::home(double t){
    std::vector< std::vector<double> > home_motion_list;
    std::vector<double> current;
    std::vector<double> home;
    for(auto* link : link_set){
        current.push_back(link->getq_current());
        home.push_back(link->getq_home());
    }
    
    int step_end = t*1000 / CONTROL_CYCLE;
    for(int step=0; step<step_end; step++){
        std::vector<double> motion;
        for(int id=0; id<current.size(); id++){
            motion[id] = ( current[id]*( (double)(step_end-step) ) + home[id]*( (double)(step) ) ) / (double)step_end;
        }
        home_motion_list.push_back(motion);
    }
    home_motion_list.push_back(home);
    return home_motion_list;
}
