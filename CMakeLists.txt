cmake_minimum_required(VERSION 2.8.3)
project(bag2kitti_wr)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -g")

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  nav_msgs
  sensor_msgs
  gps_common
  roscpp
  rospy
  rosbag
  pcl_ros
  std_msgs
  image_transport
  cv_bridge
  tf
  genmsg
)

find_package(PCL 1.7 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

message(STATUS "PCL_INCLUDE_DIRS:" ${PCL_INCLUDE_DIRS})
message(STATUS "PCL_LIBRARY_DIRS:" ${PCL_LIBRARY_DIRS})
message(STATUS "PCL_DEFINITIONS:" ${PCL_DEFINITIONS})


generate_messages(
   DEPENDENCIES
   std_msgs
   nav_msgs   
   geometry_msgs
   sensor_msgs
   tf
 )

include_directories(
  include
	${catkin_INCLUDE_DIRS} ${PCL_INCLUDE_DIRS}
)


catkin_package(
  CATKIN_DEPENDS geometry_msgs nav_msgs roscpp rospy rosbag std_msgs message_runtime
  DEPENDS EIGEN3 PCL 
  INCLUDE_DIRS include
)

add_executable(bag2kitti_wr src/bag2kitti_wr.cpp src/read_txt.cpp)
target_link_libraries(bag2kitti_wr ${catkin_LIBRARIES} ${PCL_LIBRARIES})




