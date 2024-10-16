#include "Link.h"
#include <cmath>

Link::Link(){
    this->P_w = Vector3d::Zero();
    this->R_w = Matrix3d::Identity();
    Vector3d A_init;
    A_init << 1,0,0;
    this->A_l = A_init;
}

void Link::setname(string link_name){ this->link_name = link_name; }
string Link::name(){ return this->link_name; }
void Link::setParent(Link* parent_link){ this->parent_link = parent_link; }
Link* Link::parent(){ return this->parent_link; }

void Link::setDefaultP(double x, double y, double z){
    P_l << x,y,z;
}
void Link::setDefaultA(double x, double y, double z){
    A_l << x,y,z;
    A_l = A_l.normalized();
}
void Link::setq(double q){
    this->q = q;
}
void Link::setDefault(string name, double px, double py, double pz, double ax, double ay, double az, double q, Link* parent){
    setname(name);
    setDefaultP(px, py, pz);
    setDefaultA(ax, ay, az);
    setq(q);
    setParent(parent);
}

Vector3d Link::getP_l(){ return P_l; }
Matrix3d Link::getR_l(){ calcR_l(); return R_l; }
Vector3d Link::getA_l(){ return A_l; }
double Link::getq(){ return q; }

void Link::setP_w(Vector3d P_w){ this->P_w = P_w; }
void Link::setR_w(Matrix3d R_w){ this->R_w = R_w; }
Vector3d Link::getP_w(){ return this->P_w; }
Matrix3d Link::getR_w(){ return this->R_w; }
Vector3d Link::getA_w(){
    this->A_w = this->getR_w() * this->getA_l();
    return A_w;
}

Matrix4d Link::getHTMat(){
    Matrix4d HTMat;
    /*  R_w,   P_w
        0,0,0, 1     */
    HTMat.block<3,3>(0,0) = this->getR_w();
    HTMat.block<1,3>(3,0) = MatrixXd::Zero(1,3);
    HTMat.block<3,1>(0,3) = this->getP_w();
    HTMat(3,3) = 1;
    return HTMat;
}
void Link::setState_w(Matrix4d State){
    /*  P_w, R_w
        1,   0,0,0  */
    setP_w(State.block<3,1>(0,0));
    setR_w(State.block<3,3>(0,1));
}
Matrix4d Link::getState_l(){
    /*  P_l, R_l
        1,   0,0,0  */
    Matrix4d State_l;
    State_l.block<3,1>(0,0) = this->getP_l();
    State_l.block<3,3>(0,1) = this->getR_l();
    State_l.block<1,3>(3,1) = MatrixXd::Zero(1,3);
    State_l(3,0) = 1;
    return State_l;
}

void Link::calcR_l(){
    // angle to radians
    double angleRadians = q * M_PI / 180.0;

    // make rotation matrix using axis matrix
    Matrix3d K;
    K << 0, -A_l.z(), A_l.y(),
         A_l.z(), 0, -A_l.x(),
         -A_l.y(), A_l.x(), 0;

    R_l = Matrix3d::Identity() + sin(angleRadians) * K + (1- cos(angleRadians)) * K * K;
}
