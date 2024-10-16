#include "Kinematics.h"

std::vector<Link*> Kinematics::showFromBody(Link* link){
    std::vector<Link*> link_v;
    if (link->parent() == nullptr){
        // cout << link->name() << endl;
        return link_v;
    }

    showFromBody(link->parent());
    // cout << link->name() << endl;
    for (const auto& elem : link_v){
        cout << elem->name() << endl;
    }
    cout << "\n" << endl;
    link_v.push_back(link);
    return link_v;
}

void Kinematics::forward(Link* link){
    if (link->parent() == nullptr){
        Matrix4d RootMat;
        RootMat.block<3,1>(0,0) = MatrixXd::Zero(3,1);
        RootMat.block<3,3>(0,1) = Matrix3d::Identity();
        RootMat(3,0) = 1;
        RootMat.block<1,3>(3,1) = MatrixXd::Zero(1,3);
        link->setState_w(RootMat);
        return;
    }

    forward(link->parent());
    // cout << "state of " << link->name() << endl;
    // cout << "parent's HTMat\n" << link->parent()->getHTMat() << endl;
    // cout << "local state \n" << link->getState_l() << endl;
    Matrix4d State_w;
    // multiple parent HT matrix with target local states
    State_w = link->parent()->getHTMat() * link->getState_l();
    // cout << "state on world\n" << State_w << "\n\n" << endl;
    link->setState_w(State_w);
    return;
}

void Kinematics::inverse(Link* link, Vector3d P_ref, Matrix3d R_ref){
}




VectorXd Kinematics::calcerr(Link* link, Vector3d P_ref, Matrix3d R_ref){
    VectorXd errVec(6);
    
    Vector3d P_err = P_ref - link->getP_w();
    Matrix3d R_now = link->getR_w();
    Matrix3d R_err = R_now.inverse() * R_ref;
    Vector3d w_err = R_now * rot2omega(link, R_err);

    errVec.segment(0,3) = P_err;
    errVec.segment(3,3) = w_err;
    return errVec;
}

Vector3d Kinematics::rot2omega(Link* link, Matrix3d R_ref){
    Vector3d w;
    
    Matrix3d R_now = link->getR_w();
    Vector3d el;
    el << R_now(2,1) - R_now(1,2),
          R_now(0,2) - R_now(2,0),
          R_now(1,0) - R_now(0,1);
    double norm_el = el.norm();

    if (norm_el > std::numeric_limits<double>::epsilon()){
        return link->getq() / norm_el * el;
    }else if (R_now(0,0) > 0 && R_now(1,1) > 0 && R_now(2,2) > 0){
        return Vector3d::Zero();
    }else{
        Vector3d v;
        v << R_now(0,0) + 1,
             R_now(1,1) + 1,
             R_now(2,2) + 1;
        return M_PI / 2 * v;
    }
}