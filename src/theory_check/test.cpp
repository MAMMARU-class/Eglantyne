#include <iostream>
#include <vector>
// #include <Eigen/Dense>

using namespace std;
// using namespace Eigen;

class Test{
public:
    void set_name(string name){this->name = name;}
    string get_name(){return name;}
private:
    string name;
};



int main(void){
    Test test1; test1.set_name("test1");
    Test test2; test2.set_name("test2");
    Test test3; test3.set_name("test3");

    std::vector<Test*> list;
    list.push_back(&test1);
    list.push_back(&test2);
    list.push_back(&test2);

    for (const auto& elem : list){
        cout << elem->get_name() << endl;
    }
}
