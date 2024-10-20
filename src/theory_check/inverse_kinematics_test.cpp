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
    hip_yaw.setDefault("hip_yaw", 0,-25,0, 0,0,1, 0, &body);
    hip_roll.setDefault("hip_roll", 0,0,-15, 1,0,0, 0, &hip_yaw);
    hip_pitch.setDefault("hip_pitch", 0,0,-15, 0,1,0, 0, &hip_roll);
    nee_pitch.setDefault("nee_pitch", 0,0,-70, 0,1,0, 0, &hip_pitch);
    toe_pitch.setDefault("toe_roll", 0,0,-70, 0,1,0, 0, &nee_pitch);
    toe_roll.setDefault("toe_pitch", 0,0,0, 1,0,0, 0, &toe_pitch);

    main_link.setname("main");
    next_link.setDefault("next", 0,20,0, 1,0,0, 0, &main_link);
    final_link.setDefault("final", 0,0,-30, 1,0,0, 0, &next_link);
}

int main(void){
    using std::cout;
    using std::endl;

    setup();
    hip_pitch.setq(-30);
    nee_pitch.setq(60);
    toe_pitch.setq(-30);
    // default state

    Vector3d P_ref;
    P_ref << 20,
             -45,
             -70;
    Matrix3d R_ref;
    R_ref << 1, 0, 0,
             0, 1, 0,
             0, 0, 1;
    Kinematics::inverse(&toe_roll, P_ref, R_ref);
    std::vector<Link*> link_list = Kinematics::showFromBody(&toe_roll);
    for (const auto& link : link_list){
        cout << link->name() << " : " << link->getq() << endl;
    }

    cout << "\n\nendeffector P_w : \n" << toe_roll.getP_w() << endl;
    cout <<   "\nendeffector R_w : \n" << toe_roll.getR_w() << endl;

    return 0;
}