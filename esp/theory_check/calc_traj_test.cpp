#include <iostream>
#include <Eigen/Dense>
#include <cmath>

using Eigen::Vector2d; using Eigen::Matrix2d;
using std::cout; using std::endl;


// fixed variables
#define Z 237.4 // grand to COM
#define BASE_TO_COM 40.0
#define END_TO_FOOT 37.4
#define H 15.0 // foot height while walking 
#define G (9.81 * 1000) // mm/s^2

#define Tsup 0.8
#define Tc std::sqrt(Z / G)
#define C std::cosh(Tsup / Tc)
#define S std::sinh(Tsup / Tc)
#define COEF_A 10
#define COEF_B 1
#define D ( COEF_A * (C-1)*(C-1) + COEF_B * (S/Tc)*(S/Tc) )

double sx;
double sy;

int step_dir = -1;
Vector2d m2_step{0, 60}; // foot pos at n-2 step (origin: n-1 step)
Vector2d aim_step{0, 60}; // foot pos aim of on going walk (n step. origin: n-1 step)
Vector2d p1_step{0, 0}; // foot pos at n+1 step (origin: n-1 step)

Vector2d COM_p_aim{0, 30};
Vector2d COM_v_aim{0, -190};

void calc_foot_pos()
{
    // shift foot step and com info
    Vector2d COM_p_start = COM_p_aim - aim_step;
    Vector2d COM_v_start = COM_v_aim;

    m2_step = -aim_step;
    aim_step = p1_step - aim_step;
    Vector2d ofs{sx, -step_dir*sy};
    p1_step =aim_step + ofs;

    cout << "step n-2\n" << m2_step << "\n" << endl;
    cout << "step n  \n" << aim_step << "\n" << endl;
    cout << "step n+1\n" << p1_step << "\n" << endl;

    // model aim (origin: n step)
    COM_p_aim << sx / 2,
                 -step_dir * sy / 2;
    COM_v_aim << (C + 1) / (Tc*S) * COM_p_aim[0],
                 (C - 1) / (Tc*S) * COM_p_aim[1]; // reverse v_y as n-1 step is on the other side of n step
    
    COM_p_aim += aim_step;

    cout << "COM p start\n" << COM_p_start << "\n" << endl;
    cout << "COM v start\n" << COM_v_start << "\n" << endl;
    cout << "COM p aim\n" << COM_p_aim << "\n" << endl;
    cout << "COM v aim\n" << COM_v_aim << "\n" << endl;
    
    aim_step[0] = - COEF_A*(C-1) / D * (COM_p_aim[0] - C*COM_p_start[0] - Tc*S*COM_v_start[0])
                  - COEF_B*S/( Tc*D ) * (COM_v_aim[0] - S/Tc * COM_p_start[0] - C*COM_v_start[0]);
    aim_step[1] = - COEF_A*(C-1) / D * (COM_p_aim[1] - C*COM_p_start[1] - Tc*S*COM_v_start[1])
                  - COEF_B*S/( Tc*D ) * (COM_v_aim[1] - S/Tc * COM_p_start[1] - C*COM_v_start[1]);
    
    step_dir *= -1;
    
    COM_p_aim[0] = C    * COM_p_start[0] + Tc*S*COM_v_start[0] + (1-C)*aim_step[0];
    COM_v_aim[0] = S/Tc * COM_p_start[0] +    C*COM_v_start[0] - S/Tc *aim_step[0];

    COM_p_aim[1] = C    * COM_p_start[1] + Tc*S*COM_v_start[1] + (1-C)*aim_step[1];
    COM_v_aim[1] = S/Tc * COM_p_start[1] +    C*COM_v_start[1] - S/Tc *aim_step[1];
}

int main(void){
    for (int i=0; i<5; i++){
        sx = 60; sy = 60;
        calc_foot_pos();
        cout << "step\n" << aim_step << "\n" << "*****************************" << endl;
    }
    for (int i=0; i<2; i++){
        sx = 0; sy = 60;
        calc_foot_pos();
        cout << "step\n" << aim_step << "\n" << "*****************************" << endl;
    }
}
