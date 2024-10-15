#ifndef Kinematics_h
#define Kinematics_h

#include <iostream>
#include <Eigen/Dense>
#include "Link.h"

using namespace Eigen;

namespace Kinematics{
    void showFromBody(Link* link);
    void forward(Link* link);
};

#endif
