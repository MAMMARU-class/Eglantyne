// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from simple_sim:msg/Motion.idl
// generated code does not contain a copyright notice

#ifndef SIMPLE_SIM__MSG__DETAIL__MOTION__BUILDER_HPP_
#define SIMPLE_SIM__MSG__DETAIL__MOTION__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "simple_sim/msg/detail/motion__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace simple_sim
{

namespace msg
{

namespace builder
{

class Init_Motion_motion
{
public:
  Init_Motion_motion()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::simple_sim::msg::Motion motion(::simple_sim::msg::Motion::_motion_type arg)
  {
    msg_.motion = std::move(arg);
    return std::move(msg_);
  }

private:
  ::simple_sim::msg::Motion msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::simple_sim::msg::Motion>()
{
  return simple_sim::msg::builder::Init_Motion_motion();
}

}  // namespace simple_sim

#endif  // SIMPLE_SIM__MSG__DETAIL__MOTION__BUILDER_HPP_
