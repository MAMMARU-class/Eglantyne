// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from simple_sim:msg/Motion.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "simple_sim/msg/detail/motion__rosidl_typesupport_introspection_c.h"
#include "simple_sim/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "simple_sim/msg/detail/motion__functions.h"
#include "simple_sim/msg/detail/motion__struct.h"


// Include directives for member types
// Member `motion`
#include "trajectory_msgs/msg/joint_trajectory_point.h"
// Member `motion`
#include "trajectory_msgs/msg/detail/joint_trajectory_point__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void simple_sim__msg__Motion__rosidl_typesupport_introspection_c__Motion_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  simple_sim__msg__Motion__init(message_memory);
}

void simple_sim__msg__Motion__rosidl_typesupport_introspection_c__Motion_fini_function(void * message_memory)
{
  simple_sim__msg__Motion__fini(message_memory);
}

size_t simple_sim__msg__Motion__rosidl_typesupport_introspection_c__size_function__Motion__motion(
  const void * untyped_member)
{
  const trajectory_msgs__msg__JointTrajectoryPoint__Sequence * member =
    (const trajectory_msgs__msg__JointTrajectoryPoint__Sequence *)(untyped_member);
  return member->size;
}

const void * simple_sim__msg__Motion__rosidl_typesupport_introspection_c__get_const_function__Motion__motion(
  const void * untyped_member, size_t index)
{
  const trajectory_msgs__msg__JointTrajectoryPoint__Sequence * member =
    (const trajectory_msgs__msg__JointTrajectoryPoint__Sequence *)(untyped_member);
  return &member->data[index];
}

void * simple_sim__msg__Motion__rosidl_typesupport_introspection_c__get_function__Motion__motion(
  void * untyped_member, size_t index)
{
  trajectory_msgs__msg__JointTrajectoryPoint__Sequence * member =
    (trajectory_msgs__msg__JointTrajectoryPoint__Sequence *)(untyped_member);
  return &member->data[index];
}

void simple_sim__msg__Motion__rosidl_typesupport_introspection_c__fetch_function__Motion__motion(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const trajectory_msgs__msg__JointTrajectoryPoint * item =
    ((const trajectory_msgs__msg__JointTrajectoryPoint *)
    simple_sim__msg__Motion__rosidl_typesupport_introspection_c__get_const_function__Motion__motion(untyped_member, index));
  trajectory_msgs__msg__JointTrajectoryPoint * value =
    (trajectory_msgs__msg__JointTrajectoryPoint *)(untyped_value);
  *value = *item;
}

void simple_sim__msg__Motion__rosidl_typesupport_introspection_c__assign_function__Motion__motion(
  void * untyped_member, size_t index, const void * untyped_value)
{
  trajectory_msgs__msg__JointTrajectoryPoint * item =
    ((trajectory_msgs__msg__JointTrajectoryPoint *)
    simple_sim__msg__Motion__rosidl_typesupport_introspection_c__get_function__Motion__motion(untyped_member, index));
  const trajectory_msgs__msg__JointTrajectoryPoint * value =
    (const trajectory_msgs__msg__JointTrajectoryPoint *)(untyped_value);
  *item = *value;
}

bool simple_sim__msg__Motion__rosidl_typesupport_introspection_c__resize_function__Motion__motion(
  void * untyped_member, size_t size)
{
  trajectory_msgs__msg__JointTrajectoryPoint__Sequence * member =
    (trajectory_msgs__msg__JointTrajectoryPoint__Sequence *)(untyped_member);
  trajectory_msgs__msg__JointTrajectoryPoint__Sequence__fini(member);
  return trajectory_msgs__msg__JointTrajectoryPoint__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember simple_sim__msg__Motion__rosidl_typesupport_introspection_c__Motion_message_member_array[1] = {
  {
    "motion",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(simple_sim__msg__Motion, motion),  // bytes offset in struct
    NULL,  // default value
    simple_sim__msg__Motion__rosidl_typesupport_introspection_c__size_function__Motion__motion,  // size() function pointer
    simple_sim__msg__Motion__rosidl_typesupport_introspection_c__get_const_function__Motion__motion,  // get_const(index) function pointer
    simple_sim__msg__Motion__rosidl_typesupport_introspection_c__get_function__Motion__motion,  // get(index) function pointer
    simple_sim__msg__Motion__rosidl_typesupport_introspection_c__fetch_function__Motion__motion,  // fetch(index, &value) function pointer
    simple_sim__msg__Motion__rosidl_typesupport_introspection_c__assign_function__Motion__motion,  // assign(index, value) function pointer
    simple_sim__msg__Motion__rosidl_typesupport_introspection_c__resize_function__Motion__motion  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers simple_sim__msg__Motion__rosidl_typesupport_introspection_c__Motion_message_members = {
  "simple_sim__msg",  // message namespace
  "Motion",  // message name
  1,  // number of fields
  sizeof(simple_sim__msg__Motion),
  simple_sim__msg__Motion__rosidl_typesupport_introspection_c__Motion_message_member_array,  // message members
  simple_sim__msg__Motion__rosidl_typesupport_introspection_c__Motion_init_function,  // function to initialize message memory (memory has to be allocated)
  simple_sim__msg__Motion__rosidl_typesupport_introspection_c__Motion_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t simple_sim__msg__Motion__rosidl_typesupport_introspection_c__Motion_message_type_support_handle = {
  0,
  &simple_sim__msg__Motion__rosidl_typesupport_introspection_c__Motion_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_simple_sim
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, simple_sim, msg, Motion)() {
  simple_sim__msg__Motion__rosidl_typesupport_introspection_c__Motion_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, trajectory_msgs, msg, JointTrajectoryPoint)();
  if (!simple_sim__msg__Motion__rosidl_typesupport_introspection_c__Motion_message_type_support_handle.typesupport_identifier) {
    simple_sim__msg__Motion__rosidl_typesupport_introspection_c__Motion_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &simple_sim__msg__Motion__rosidl_typesupport_introspection_c__Motion_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
