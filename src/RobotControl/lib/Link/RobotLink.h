#ifndef Link_h
#define Link_h

#include <Arduino.h>
#include <ArduinoEigenDense.h>
#include "IcsHardSerialClass.h"

using namespace std;
using namespace Eigen;

#define FORWARD 1
#define REVERSE -1

class RobotLink{
public:
    RobotLink();

    void setname(string link_name);
    string name();
    void setParent(RobotLink* parent_link);
    RobotLink* parent();
    
    void setDefaultP(double x, double y, double z);
    void setDefaultA(double x, double y, double z);
    void setq(double q); // radian
    
    // delete q_setter
    void setDefault(string name, double px, double py, double pz, double ax, double ay, double az, RobotLink* parent);

    // new
    void set_pos_ini(int pos_ini); // position 3500-11500
    void set_q_home(double q_home); // radian
    void set_limit(double q_min, double q_max); // radian
    void setInitialPositions(int pos_ini, double q_home, double q_min, double q_max);
    void setInitialPositionsDeg(int pos_ini, double q_home, double q_min, double q_max);
    double deg2rad(double deg);

    // new
    void setMotor(IcsHardSerialClass* motor, int motor_id, int dir);

    // getter of local info
    Vector3d getP_l();
    Matrix3d getR_l();
    Vector3d getA_l();
    double getq(); // radian
    double getq_deg(); // degree
    double getq_home();
    double getq_min();
    double getq_max();

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
    
    // new
    // controlling motor
    void move(double q_order);
    void move_q();
    void move_home(double t);
    void move_init(double t);
    double getq_current(); // relax the motor and get pos(rad)
    int rad2pos(double rad);
    double pos2rad(int pos);

private:
    string link_name;
    RobotLink* parent_link;

    // new
    IcsHardSerialClass* motor;
    int motor_id;
    int dir;

    Vector3d P_l;
    Vector3d A_l;
    double q;
    Matrix3d R_l;
    
    // new
    int pos_ini;
    double q_home;
    double q_max;
    double q_min;

    Vector3d P_w;
    Vector3d A_w;
    Matrix3d R_w;

    void calcR_l();
};

#endif
