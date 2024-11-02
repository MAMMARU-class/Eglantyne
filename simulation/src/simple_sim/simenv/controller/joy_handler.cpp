#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"
#include "sensor_msgs/msg/joy.hpp"

#include "../../src/motion/motion_trig.h"

using namespace std::chrono_literals;
using std::placeholders::_1;
using Int32 = std_msgs::msg::Int32;

class JoyHandler : public rclcpp::Node
{
public:
    JoyHandler()
    : Node("joy_handler")
    {
        sub_joy_ = this->create_subscription<sensor_msgs::msg::Joy>(
            "/joy", 10, std::bind(&JoyHandler::update_joy_order, this, _1));
        sub_trig_ = this->create_subscription<Int32>(
            "/motion_trigger", 10, std::bind(&JoyHandler::check_state, this, _1));
        pub_trig_ = this->create_publisher<Int32>("/motion_trigger", 1);
    }
private:
    void update_joy_order(sensor_msgs::msg::Joy joy_input){
        Int32 trig;
        if(state == STAY){
            if(joy_input.buttons[4] == 1){
                trig.data = WAKE_FRONT;
                pub_trig_->publish(trig);
            }else if(joy_input.buttons[5] == 1){
                trig.data = WAKE_BACK;
                pub_trig_->publish(trig);
            }else if(!(joy_input.axes[0] == 0 && 
                       joy_input.axes[1] == 0 && 
                       joy_input.axes[2] == 0 && 
                       joy_input.axes[3] == 0 && 
                       joy_input.axes[4] == 0 && 
                       joy_input.axes[5] == 0)){
                trig.data = WALK;
                pub_trig_->publish(trig);
            }
        }
    }

    void check_state(Int32 trig){
        this->state = trig.data;
    }
    
    int state = STAY;

    rclcpp::Subscription<Int32>::SharedPtr sub_trig_;
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_joy_;
    rclcpp::Publisher<Int32>::SharedPtr pub_trig_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<JoyHandler>());
    rclcpp::shutdown();
    return 0;
}
