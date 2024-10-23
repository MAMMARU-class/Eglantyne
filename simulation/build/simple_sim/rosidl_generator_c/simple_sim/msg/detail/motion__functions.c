// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from simple_sim:msg/Motion.idl
// generated code does not contain a copyright notice
#include "simple_sim/msg/detail/motion__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `motion`
#include "trajectory_msgs/msg/detail/joint_trajectory_point__functions.h"

bool
simple_sim__msg__Motion__init(simple_sim__msg__Motion * msg)
{
  if (!msg) {
    return false;
  }
  // motion
  if (!trajectory_msgs__msg__JointTrajectoryPoint__Sequence__init(&msg->motion, 0)) {
    simple_sim__msg__Motion__fini(msg);
    return false;
  }
  return true;
}

void
simple_sim__msg__Motion__fini(simple_sim__msg__Motion * msg)
{
  if (!msg) {
    return;
  }
  // motion
  trajectory_msgs__msg__JointTrajectoryPoint__Sequence__fini(&msg->motion);
}

bool
simple_sim__msg__Motion__are_equal(const simple_sim__msg__Motion * lhs, const simple_sim__msg__Motion * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // motion
  if (!trajectory_msgs__msg__JointTrajectoryPoint__Sequence__are_equal(
      &(lhs->motion), &(rhs->motion)))
  {
    return false;
  }
  return true;
}

bool
simple_sim__msg__Motion__copy(
  const simple_sim__msg__Motion * input,
  simple_sim__msg__Motion * output)
{
  if (!input || !output) {
    return false;
  }
  // motion
  if (!trajectory_msgs__msg__JointTrajectoryPoint__Sequence__copy(
      &(input->motion), &(output->motion)))
  {
    return false;
  }
  return true;
}

simple_sim__msg__Motion *
simple_sim__msg__Motion__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  simple_sim__msg__Motion * msg = (simple_sim__msg__Motion *)allocator.allocate(sizeof(simple_sim__msg__Motion), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(simple_sim__msg__Motion));
  bool success = simple_sim__msg__Motion__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
simple_sim__msg__Motion__destroy(simple_sim__msg__Motion * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    simple_sim__msg__Motion__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
simple_sim__msg__Motion__Sequence__init(simple_sim__msg__Motion__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  simple_sim__msg__Motion * data = NULL;

  if (size) {
    data = (simple_sim__msg__Motion *)allocator.zero_allocate(size, sizeof(simple_sim__msg__Motion), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = simple_sim__msg__Motion__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        simple_sim__msg__Motion__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
simple_sim__msg__Motion__Sequence__fini(simple_sim__msg__Motion__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      simple_sim__msg__Motion__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

simple_sim__msg__Motion__Sequence *
simple_sim__msg__Motion__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  simple_sim__msg__Motion__Sequence * array = (simple_sim__msg__Motion__Sequence *)allocator.allocate(sizeof(simple_sim__msg__Motion__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = simple_sim__msg__Motion__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
simple_sim__msg__Motion__Sequence__destroy(simple_sim__msg__Motion__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    simple_sim__msg__Motion__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
simple_sim__msg__Motion__Sequence__are_equal(const simple_sim__msg__Motion__Sequence * lhs, const simple_sim__msg__Motion__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!simple_sim__msg__Motion__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
simple_sim__msg__Motion__Sequence__copy(
  const simple_sim__msg__Motion__Sequence * input,
  simple_sim__msg__Motion__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(simple_sim__msg__Motion);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    simple_sim__msg__Motion * data =
      (simple_sim__msg__Motion *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!simple_sim__msg__Motion__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          simple_sim__msg__Motion__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!simple_sim__msg__Motion__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
