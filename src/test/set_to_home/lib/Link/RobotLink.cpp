#include "RobotLink.h"
#include <cmath>

RobotLink::RobotLink(){
    this->P_w = Vector3d::Zero();
    this->R_w = Matrix3d::Identity();
    Vector3d A_init;
    A_init << 1,0,0;
    this->A_l = A_init;
}

void RobotLink::setname(string link_name){ this->link_name = link_name; }
string RobotLink::name(){ return this->link_name; }
void RobotLink::setParent(RobotLink* parent_link){ this->parent_link = parent_link; }
RobotLink* RobotLink::parent(){ return this->parent_link; }

void RobotLink::setDefaultP(double x, double y, double z){
    P_l << x,y,z;
}
void RobotLink::setDefaultA(double x, double y, double z){
    A_l << x,y,z;
    A_l = A_l.normalized();
}
void RobotLink::setq(double q){
    // ignore under 0.2 deg
    if (abs(q) < 3e-3) { q = 0; }
    q = std::floor(q*100) / 100.0;
    this->q = q;
}

void RobotLink::setDefault(string name, double px, double py, double pz, double ax, double ay, double az, RobotLink* parent){
    setname(name);
    setDefaultP(px, py, pz);
    setDefaultA(ax, ay, az);
    setParent(parent);
}

// new
void RobotLink::set_pos_ini(int pos_ini){ this->pos_ini = pos_ini;}
void RobotLink::set_q_home(double q_home){this->q_home = q_home; setq(this->q_home);}
void RobotLink::set_limit(double q_min, double q_max){this->q_min = q_min; this->q_max = q_max;}
void RobotLink::setInitialPositions(int pos_ini, double q_home, double q_min, double q_max){
    set_pos_ini(pos_ini);    // position
    set_q_home(q_home);      // radius
    set_limit(q_min, q_max); // radius
}
void RobotLink::setInitialPositionsDeg(int pos_ini, double q_home, double q_min, double q_max){
    set_pos_ini(pos_ini);                      // position
    set_q_home(deg2rad(q_home));               // degree
    set_limit(deg2rad(q_min), deg2rad(q_max)); // degree
}
double RobotLink::deg2rad(double deg){ return deg * 3.1415 / 180.0; }

// new
void RobotLink::setMotor(IcsHardSerialClass* motor, int motor_id, int dir){
    this->motor = motor;
    this->motor_id = motor_id;
    this->dir = dir;
}

Vector3d RobotLink::getP_l(){ return P_l; }
Matrix3d RobotLink::getR_l(){ calcR_l(); return R_l; }
Vector3d RobotLink::getA_l(){ return A_l; }
double RobotLink::getq(){ return q; }
double RobotLink::getq_deg(){ return q * 180 / M_PI; }
double RobotLink::getq_home(){return this->q_home;}

void RobotLink::setP_w(Vector3d P_w){ this->P_w = P_w; }
void RobotLink::setR_w(Matrix3d R_w){ this->R_w = R_w; }
Vector3d RobotLink::getP_w(){ return this->P_w; }
Matrix3d RobotLink::getR_w(){ return this->R_w; }
Vector3d RobotLink::getA_w(){
    this->A_w = this->getR_w() * this->getA_l();
    return A_w;
}

Matrix4d RobotLink::getHTMat(){
    Matrix4d HTMat;
    /*  R_w,   P_w
        0,0,0, 1     */
    HTMat.block<3,3>(0,0) = this->getR_w();
    HTMat.block<1,3>(3,0) = MatrixXd::Zero(1,3);
    HTMat.block<3,1>(0,3) = this->getP_w();
    HTMat(3,3) = 1;
    return HTMat;
}
void RobotLink::setState_w(Matrix4d State){
    /*  P_w, R_w
        1,   0,0,0  */
    setP_w(State.block<3,1>(0,0));
    setR_w(State.block<3,3>(0,1));
}
Matrix4d RobotLink::getState_l(){
    /*  P_l, R_l
        1,   0,0,0  */
    Matrix4d State_l;
    State_l.block<3,1>(0,0) = this->getP_l();
    State_l.block<3,3>(0,1) = this->getR_l();
    State_l.block<1,3>(3,1) = MatrixXd::Zero(1,3);
    State_l(3,0) = 1;
    return State_l;
}

void RobotLink::calcR_l(){
    // make rotation matrix using axis matrix
    Matrix3d K;
    K << 0, -A_l.z(), A_l.y(),
         A_l.z(), 0, -A_l.x(),
         -A_l.y(), A_l.x(), 0;

    R_l = Matrix3d::Identity() + sin(q) * K + (1- cos(q)) * K * K;
}


// motor_control
void RobotLink::move(){ motor->setPos(motor_id, rad2pos(q)); }
void RobotLink::move_home(double t){}
void RobotLink::move_init(double t){}
double RobotLink::getq_current(){
    return pos2rad(motor->getPos(motor_id));
}

int RobotLink::rad2pos(double rad){ 
    return (int) ( ( (double)dir * rad * 180.0 / M_PI ) * ( 8000.0 / 270 ) + (double)this->pos_ini ); 
}
double RobotLink::pos2rad(int pos){
    return (double)dir * (double)(pos - pos_ini) * 270.0/8000.0 * M_PI/180.0;
}
