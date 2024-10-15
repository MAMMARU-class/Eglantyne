#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

int main(void){
    Matrix2d m_mini;
    m_mini << 1, 2,
              3, 4;

    MatrixXd m(3,3);
    m.block<2,2>(0,0) = m_mini;
    m.block<1,3>(2,0) = MatrixXd::Zero(1,3);
    m(0,2) = 3;
    m(1,2) = 5;

    cout << m << endl;
}