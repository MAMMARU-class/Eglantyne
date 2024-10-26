#ifndef Kinematics_h
#define Kinematics_h

#include <iostream>
#include <cmath>
#include <vector>
#include <Eigen/Dense>
#include "Link.h"

namespace Kinematics{
    std::vector<Link*> showFromBody(Link* link);
    void forward(Link* link);
    void inverse(Link* link, Vector3d P_ref, Matrix3d R_ref);

    // sub calculer
    MatrixXd calcJacobi(std::vector<Link*> link_list);
    VectorXd calcerr(Link* link, Vector3d P_ref, Matrix3d R_ref);
    Vector3d rot2omega(Link* link, Matrix3d R_ref);
    void setQ(VectorXd q_vec, std::vector<Link*> link_list);
};

#endif
