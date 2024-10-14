#ifndef link_def_h
#define link_def_h

#include<iostream>
#include<Eigen/Dense>

using namespace std;
using namespace Eigen;
// using Eigen::MatrixXd;

class Link{
public:
    Link();

    void setParent(Link parent);
    void setChild(Link child);

    void setDefaultP(int x, int y, int z);
    void setDefaultA(int x, int y, int z);
    void setq(int q);
    void setDefault(int px, int py, int pz, int ax, int ay, int az, int q);

    Vector3d getP_l();
    Vector3d getA_l();
    int getq();
    Matrix3d getR_l();

    // Vector3d calcP_w();
    // Matrix3d calcR_w();
    Vector3d calcA_w();
    void setP_w();
    void setR_w();

private:
    Link parent;
    Link child;

    Vector3d P_l;
    Vector3d A_l;
    int q;
    Matrix3d R_l;

    Vector3d P_w;
    Vector3d A_w;
    Matrix3d R_w;

    void calcR_l();
};

#endif