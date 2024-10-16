#ifndef Kinematics_h
#define Kinematics_h

#include <iostream>
#include <cmath>
#include <vector>
#include <Eigen/Dense>
#include "Link.h"

using namespace Eigen;

namespace Kinematics{
    std::vector<Link*> showFromBody(Link* link);
    void forward(Link* link);
    void inverse(Link* link, Vector3d P_ref, Matrix3d R_ref);

    // sub calculer
    VectorXd calcerr(Link* link, Vector3d P_ref, Matrix3d R_ref);
    Vector3d rot2omega(Link* link, Matrix3d R_ref);
};

#endif
