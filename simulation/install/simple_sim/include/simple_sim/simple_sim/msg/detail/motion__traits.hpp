// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from simple_sim:msg/Motion.idl
// generated code does not contain a copyright notice

#ifndef SIMPLE_SIM__MSG__DETAIL__MOTION__TRAITS_HPP_
#define SIMPLE_SIM__MSG__DETAIL__MOTION__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "simple_sim/msg/detail/motion__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'motion'
#include "trajectory_msgs/msg/detail/joint_trajectory_point__traits.hpp"

namespace simple_sim
{

namespace msg
{

inline void to_flow_style_yaml(
  const Motion & msg,
  std::ostream & out)
{
  out << "{";
  // member: motion
  {
    if (msg.motion.size() == 0) {
      out << "motion: []";
    } else {
      out << "motion: [";
      size_t pending_items = msg.motion.size();
      for (auto item : msg.motion) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Motion & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: motion
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.motion.size() == 0) {
      out << "motion: []\n";
    } else {
      out << "motion:\n";
      for (auto item : msg.motion) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Motion & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace simple_sim

namespace rosidl_generator_traits
{

[[deprecated("use simple_sim::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const simple_sim::msg::Motion & msg,
  std::ostream & out, size_t indentation = 0)
{
  simple_sim::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use simple_sim::msg::to_yaml() instead")]]
inline std::string to_yaml(const simple_sim::msg::Motion & msg)
{
  return simple_sim::msg::to_yaml(msg);
}

template<>
inline const char * data_type<simple_sim::msg::Motion>()
{
  return "simple_sim::msg::Motion";
}

template<>
inline const char * name<simple_sim::msg::Motion>()
{
  return "simple_sim/msg/Motion";
}

template<>
struct has_fixed_size<simple_sim::msg::Motion>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<simple_sim::msg::Motion>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<simple_sim::msg::Motion>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SIMPLE_SIM__MSG__DETAIL__MOTION__TRAITS_HPP_
