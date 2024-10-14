#include "link_def.h"
#include <cmath>

Link::Link(){}

void Link::setParent(Link parent){
    this->parent = parent;
}
void Link::setChild(Link child){
    this->child = child;
}

void Link::setDefaultP(int x, int y, int z){
    P_l << x,y,z;
}
void Link::setDefaultA(int x, int y, int z){
    A_l << x,y,z;
    A_l = A_l.normalized();

}
void Link::setq(int q){
    this->q = q;
}
void Link::setDefault(int px, int py, int pz, int ax, int ay, int az, int q){
    setDefaultP(px, py, pz);
    setDefaultA(ax, ay, az);
    setq(q);
}

Vector3d Link::getP_l(){ return P_l; }
Vector3d Link::getA_l(){ return A_l; }
int Link::getq(){ return q; }
Matrix3d Link::getR_l(){
    calcR_l();
    return R_l;
}

// Vector3d Link::calcP_w(){
//     this->P_w = parent->P_w + parent->R_w * this->P_l;
//     return this->P_w;
// }
// Matrix3d Link::calcR_w(){
//     this->R_w = this->R_l * parent->R_w;
//     return this->R_w
// }
// void calcState_w(){
//     calcP_w(); calcR_w();
// }

Vector3d Link::calcA_w(){
    this->A_w = this->R_w * this->A_l;
    return A_w;
}
void setP_w(Vector3d P_w){ this->P_w = P_w; }
void setR_w(Matrix3d R_w){ this->R_w = R_w; }


void Link::calcR_l(){
    double angleRadians = q * M_PI / 180.0;
    Matrix3d K;
    K << 0, -A_l.z(), A_l.y(),
         A_l.z(), 0, -A_l.x(),
         -A_l.y(), A_l.x(), 0;

    R_l = Matrix3d::Identity() + sin(angleRadians) * K + (1- cos(angleRadians)) * K * K;
}
