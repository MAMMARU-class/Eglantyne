#include "walk_test_func.h"

WalkTest Robot;
int main(void)
{
    Robot.setup_link();
    Robot.calc_foot_pos();
}