// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from simple_sim:msg/Motion.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "simple_sim/msg/detail/motion__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace simple_sim
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void Motion_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) simple_sim::msg::Motion(_init);
}

void Motion_fini_function(void * message_memory)
{
  auto typed_message = static_cast<simple_sim::msg::Motion *>(message_memory);
  typed_message->~Motion();
}

size_t size_function__Motion__motion(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<trajectory_msgs::msg::JointTrajectoryPoint> *>(untyped_member);
  return member->size();
}

const void * get_const_function__Motion__motion(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<trajectory_msgs::msg::JointTrajectoryPoint> *>(untyped_member);
  return &member[index];
}

void * get_function__Motion__motion(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<trajectory_msgs::msg::JointTrajectoryPoint> *>(untyped_member);
  return &member[index];
}

void fetch_function__Motion__motion(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const trajectory_msgs::msg::JointTrajectoryPoint *>(
    get_const_function__Motion__motion(untyped_member, index));
  auto & value = *reinterpret_cast<trajectory_msgs::msg::JointTrajectoryPoint *>(untyped_value);
  value = item;
}

void assign_function__Motion__motion(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<trajectory_msgs::msg::JointTrajectoryPoint *>(
    get_function__Motion__motion(untyped_member, index));
  const auto & value = *reinterpret_cast<const trajectory_msgs::msg::JointTrajectoryPoint *>(untyped_value);
  item = value;
}

void resize_function__Motion__motion(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<trajectory_msgs::msg::JointTrajectoryPoint> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember Motion_message_member_array[1] = {
  {
    "motion",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<trajectory_msgs::msg::JointTrajectoryPoint>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(simple_sim::msg::Motion, motion),  // bytes offset in struct
    nullptr,  // default value
    size_function__Motion__motion,  // size() function pointer
    get_const_function__Motion__motion,  // get_const(index) function pointer
    get_function__Motion__motion,  // get(index) function pointer
    fetch_function__Motion__motion,  // fetch(index, &value) function pointer
    assign_function__Motion__motion,  // assign(index, value) function pointer
    resize_function__Motion__motion  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers Motion_message_members = {
  "simple_sim::msg",  // message namespace
  "Motion",  // message name
  1,  // number of fields
  sizeof(simple_sim::msg::Motion),
  Motion_message_member_array,  // message members
  Motion_init_function,  // function to initialize message memory (memory has to be allocated)
  Motion_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t Motion_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &Motion_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace simple_sim


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<simple_sim::msg::Motion>()
{
  return &::simple_sim::msg::rosidl_typesupport_introspection_cpp::Motion_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, simple_sim, msg, Motion)() {
  return &::simple_sim::msg::rosidl_typesupport_introspection_cpp::Motion_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
