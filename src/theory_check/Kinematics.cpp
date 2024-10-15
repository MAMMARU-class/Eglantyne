#include "Kinematics.h"

void Kinematics::showFromBody(Link* link){
    if (link->parent() == nullptr){
        cout << link->name() << endl;
        return;
    }

    showFromBody(link->parent());
    cout << link->name() << endl;
    return;
}

void Kinematics::forward(Link* link){
    if (link->parent() == nullptr){
        Matrix4d RootMat;
        RootMat.block<3,1>(0,0) = MatrixXd::Zero(3,1);
        RootMat.block<3,3>(0,1) = Matrix3d::Identity();
        RootMat(3,0) = 1;
        RootMat.block<1,3>(3,1) = MatrixXd::Zero(1,3);
        link->setState_w(RootMat);
        cout << "state of " << link->name() << "\n" << RootMat << "\n\n" << endl;
        return;
    }

    forward(link->parent());
    // cout << "made state matrix" << endl;
    cout << "state of " << link->name() << endl;
    cout << "HTMat\n" << link->getHTMat() << endl;
    cout << "parent state \n" << link->parent()->getState_w() << endl;
    Matrix4d State_w;
    State_w = link->parent()->getHTMat() * link->getState_l();
    cout << "state on world\n" << State_w << "\n\n" << endl;
    link->setState_w(State_w);
    return;
}
