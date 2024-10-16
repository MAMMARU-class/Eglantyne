#ifndef Link_h
#define Link_h

#include<iostream>
#include<Eigen/Dense>

using namespace std;
using namespace Eigen;

class Link{
public:
    Link();
    
    void setname(string link_name);
    string name();
    void setParent(Link* parent_link);
    Link* parent();

    void setDefaultP(double x, double y, double z);
    void setDefaultA(double x, double y, double z);
    void setq(double q);
    void setDefault(string name, double px, double py, double pz, double ax, double ay, double az, double q, Link* parent);

    // getter of local info
    Vector3d getP_l();
    Matrix3d getR_l();
    Vector3d getA_l();
    double getq();

    // setter and getter of world info
    void setP_w(Vector3d P_w);
    void setR_w(Matrix3d R_w);
    Vector3d getP_w();
    Matrix3d getR_w();
    Vector3d getA_w();

    // homogeneous transformation matrix
    Matrix4d getHTMat();
    // state info
    void setState_w(Matrix4d State);
    Matrix4d getState_w();
    Matrix4d getState_l();

private:
    string link_name;
    Link* parent_link;

    Vector3d P_l;
    Vector3d A_l;
    double q;
    Matrix3d R_l;

    Vector3d P_w;
    Vector3d A_w;
    Matrix3d R_w;

    void calcR_l();
};

#endif
