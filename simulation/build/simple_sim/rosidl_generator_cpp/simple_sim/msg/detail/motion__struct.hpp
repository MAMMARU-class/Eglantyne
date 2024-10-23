// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from simple_sim:msg/Motion.idl
// generated code does not contain a copyright notice

#ifndef SIMPLE_SIM__MSG__DETAIL__MOTION__STRUCT_HPP_
#define SIMPLE_SIM__MSG__DETAIL__MOTION__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'motion'
#include "trajectory_msgs/msg/detail/joint_trajectory_point__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__simple_sim__msg__Motion __attribute__((deprecated))
#else
# define DEPRECATED__simple_sim__msg__Motion __declspec(deprecated)
#endif

namespace simple_sim
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Motion_
{
  using Type = Motion_<ContainerAllocator>;

  explicit Motion_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
  }

  explicit Motion_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
    (void)_alloc;
  }

  // field types and members
  using _motion_type =
    std::vector<trajectory_msgs::msg::JointTrajectoryPoint_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<trajectory_msgs::msg::JointTrajectoryPoint_<ContainerAllocator>>>;
  _motion_type motion;

  // setters for named parameter idiom
  Type & set__motion(
    const std::vector<trajectory_msgs::msg::JointTrajectoryPoint_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<trajectory_msgs::msg::JointTrajectoryPoint_<ContainerAllocator>>> & _arg)
  {
    this->motion = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    simple_sim::msg::Motion_<ContainerAllocator> *;
  using ConstRawPtr =
    const simple_sim::msg::Motion_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<simple_sim::msg::Motion_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<simple_sim::msg::Motion_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      simple_sim::msg::Motion_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<simple_sim::msg::Motion_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      simple_sim::msg::Motion_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<simple_sim::msg::Motion_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<simple_sim::msg::Motion_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<simple_sim::msg::Motion_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__simple_sim__msg__Motion
    std::shared_ptr<simple_sim::msg::Motion_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__simple_sim__msg__Motion
    std::shared_ptr<simple_sim::msg::Motion_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Motion_ & other) const
  {
    if (this->motion != other.motion) {
      return false;
    }
    return true;
  }
  bool operator!=(const Motion_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Motion_

// alias to use template instance with default allocator
using Motion =
  simple_sim::msg::Motion_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace simple_sim

#endif  // SIMPLE_SIM__MSG__DETAIL__MOTION__STRUCT_HPP_
