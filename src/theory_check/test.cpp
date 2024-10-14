#include <iostream>
#include <Eigen/Dense>

using namespace std;
using Eigen::MatrixXd;

int main(void){
    MatrixXd m(2,2);
    m(0,0) = 1;
    m(1,0) = 2.5;
    m(0,1) = -3;
    m(1,1) = m(0,0) + m(0,1);

    cout << m << endl;
}