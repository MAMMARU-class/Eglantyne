#ifndef Kinematics_h
#define Kinematics_h

#include <iostream>
#include <cmath>
#include <vector>
#include <ArduinoEigenDense.h>
#include "RobotLink.h"

namespace Kinematics{
    std::vector<RobotLink*> showFromBody(RobotLink* link);
    void forward(RobotLink* link);
    void inverse(RobotLink* link, Vector3d P_ref, Matrix3d R_ref);

    // sub calculer
    MatrixXd calcJacobi(std::vector<RobotLink*> link_list);
    VectorXd calcerr(RobotLink* link, Vector3d P_ref, Matrix3d R_ref);
    Vector3d rot2omega(Matrix3d R_ref);
    void setQ(VectorXd q_vec, std::vector<RobotLink*> link_list);
};

#endif
