// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from simple_sim:msg/Motion.idl
// generated code does not contain a copyright notice

#ifndef SIMPLE_SIM__MSG__DETAIL__MOTION__STRUCT_H_
#define SIMPLE_SIM__MSG__DETAIL__MOTION__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'motion'
#include "trajectory_msgs/msg/detail/joint_trajectory_point__struct.h"

/// Struct defined in msg/Motion in the package simple_sim.
typedef struct simple_sim__msg__Motion
{
  trajectory_msgs__msg__JointTrajectoryPoint__Sequence motion;
} simple_sim__msg__Motion;

// Struct for a sequence of simple_sim__msg__Motion.
typedef struct simple_sim__msg__Motion__Sequence
{
  simple_sim__msg__Motion * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} simple_sim__msg__Motion__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SIMPLE_SIM__MSG__DETAIL__MOTION__STRUCT_H_
