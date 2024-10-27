#include "Link_rad.h"
#include "Kinematics_devide_y.h"
using namespace Kinematics;

Link body;
Link leg_yaw;
Link leg_roll;
Link leg_upper;
Link leg_under;
Link foot_pitch;
Link foot_roll;

Link main_link;
Link next_link;
Link final_link;

void setup(){
    body.setname("body");
    leg_yaw.setDefault("leg_yaw", 0,-30,0, 0,0,1, 0, &body);
    leg_roll.setDefault("leg_roll", 0,0,-22.2, 1,0,0, 0, &leg_yaw);
    leg_upper.setDefault("leg_upper", 0,0,-26.01, 0,1,0, 0, &leg_roll);
    leg_under.setDefault("leg_under", 0,0,-78.02, 0,1,0, 0, &leg_upper);
    foot_pitch.setDefault("foot_pitch", 0,0,-78.02, 0,1,0, 0, &leg_under);
    foot_roll.setDefault("foot_roll", 0,0,0, 1,0,0, 0, &foot_pitch);
}

int main(void){
    using std::cout; using std::endl;

    setup();
    leg_upper.setq(-18.31 * M_PI / 180.0);
    leg_under.setq(65.8 * M_PI / 180.0);
    foot_pitch.setq(-45.88 * M_PI / 180.0);

    // Vector3d P_ref;
    // P_ref << -33,
    //          -30,
    //          -160;
    // P_ref << 30,
    //          -45,
    //          -100;
    Matrix3d R_ref;
    R_ref << 1, 0, 0,
             0, 1, 0,
             0, 0, 1;
    // Kinematics::inverse(&toe_roll, P_ref, R_ref);
    std::vector<Link*> link_list = Kinematics::showFromBody(&foot_roll);
    // cout << "**********************************" << endl;
    // for (const auto& link : link_list){
    //     cout << link->name() << " : " << link->getq_deg() << endl;
    // }

    // cout << "endeffector P_w : \n" << toe_roll.getP_w() << endl;
    // cout << "endeffector R_w : \n" << toe_roll.getR_w() << endl;
    // cout << "**********************************" << endl;

    // P_ref << -33,
    //          -30,
    //          -156;
    // Kinematics::inverse(&toe_roll, P_ref, R_ref);
    // cout << "**********************************" << endl;
    // for (const auto& link : link_list){
    //     cout << link->name() << " : " << link->getq() << endl;
    // }
    // cout << "endeffector P_w : \n" << toe_roll.getP_w() << endl;
    // cout << "endeffector R_w : \n" << toe_roll.getR_w() << endl;
    // cout << "**********************************" << endl;

    // P_ref << -33,
    //          -30,
    //          -156;
    // Kinematics::inverse(&toe_roll, P_ref, R_ref);
    // cout << "**********************************" << endl;
    // for (const auto& link : link_list){
    //     cout << link->name() << " : " << link->getq() << endl;
    // }
    // cout << "endeffector P_w : \n" << toe_roll.getP_w() << endl;
    // cout << "endeffector R_w : \n" << toe_roll.getR_w() << endl;
    // cout << "**********************************" << endl;

    // P_ref << -33,
    //          -30,
    //          -154;
    // Kinematics::inverse(&toe_roll, P_ref, R_ref);
    // cout << "**********************************" << endl;
    // for (const auto& link : link_list){
    //     cout << link->name() << " : " << link->getq() << endl;
    // }
    // cout << "endeffector P_w : \n" << toe_roll.getP_w() << endl;
    // cout << "endeffector R_w : \n" << toe_roll.getR_w() << endl;
    // cout << "**********************************" << endl;

    Vector3d P_ref;
    double x = -33;
    double y = -30;
    double z = -160;
    for (int i=0; i<5000; i++){
        x+=0.00003; y+=0.0008; z+=0.0005;
        P_ref << x,
                 y,
                 z;
        Kinematics::inverse(&foot_roll, P_ref, R_ref);
        cout << "**********************************\n" << i+1 << " times " << endl;
        for (const auto& link : link_list){
            cout << link->name() << " : " << link->getq_deg() << endl;
        }
        cout << "endeffector P_w : \n" << foot_roll.getP_w().transpose() << endl;
        cout << "endeffector R_w : \n" << foot_roll.getR_w() << endl;
        cout << "**********************************" << endl;
    }

    return 0;
}
