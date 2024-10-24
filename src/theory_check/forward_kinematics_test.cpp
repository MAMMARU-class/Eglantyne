#include "Link.h"
#include "Kinematics.h"
using namespace Kinematics;

Link body;
Link hip_yaw;
Link hip_roll;
Link hip_pitch;
Link nee_pitch;
Link toe_roll;
Link toe_pitch;

Link main_link;
Link next_link;
Link final_link;
    
void setup(){
    body.setname("body");
    hip_yaw.setDefault("hip_yaw", 0,-30,0, 0,0,1, 0, &body);
    hip_roll.setDefault("hip_roll", 0,0,-22.2, 1,0,0, 0, &hip_yaw);
    hip_pitch.setDefault("hip_pitch", 0,0,-26.01, 0,1,0, 0, &hip_roll);
    nee_pitch.setDefault("nee_pitch", 0,0,-78.02, 0,1,0, 0, &hip_pitch);
    toe_pitch.setDefault("toe_roll", 0,0,-78.02, 0,1,0, 0, &nee_pitch);
    toe_roll.setDefault("toe_pitch", 0,0,0, 1,0,0, 0, &toe_pitch);

    main_link.setname("main");
    next_link.setDefault("next", 0,20,0, 1,0,0, 0, &main_link);
    final_link.setDefault("final", 0,0,-30, 1,0,0, 0, &next_link);
}

int main(void){
    using std::cout;
    using std::endl;

    setup();
    // hip_yaw.setq(45);
    hip_pitch.setq(-17.47);
    nee_pitch.setq(56.78);
    toe_pitch.setq(-39.31);
    Kinematics::forward(&toe_pitch);
    
    cout << toe_pitch.getP_w() << endl;
    cout << toe_pitch.getR_w() << endl;

    std::vector<Link*> link_list;
    link_list = Kinematics::showFromBody(&toe_pitch);

    for (const auto& link : link_list){
        cout << link->name() << endl;
    }
}
