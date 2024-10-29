#include"link_def.h"
using namespace std;

Link test_link;

int main(void){
    test_link.setDefault(0,1,1,0,0,2,30);
    cout << "local P : \n" << test_link.getP_l() << endl;
    cout << "local A : \n" << test_link.getA_l() << endl;
    cout << "q : " << test_link.getq() << endl;
    cout << "local R : \n" << test_link.getR_l() << endl;
    
    return 0;
}
