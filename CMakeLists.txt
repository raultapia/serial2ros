# @file CMakeLists.txt
# @brief Serial to ROS
# @author Raul Tapia (raultapia.com)
# @copyright The Unlicense
# @see https://github.com/raultapia/serial2ros

cmake_minimum_required(VERSION 3.1.0)
project(serial2ros)
find_package(catkin REQUIRED COMPONENTS roscpp std_msgs serial)

catkin_package()
include_directories(${catkin_INCLUDE_DIRS})
add_executable(serial2ros src/serial2ros_node.cpp)
target_link_libraries(serial2ros ${catkin_LIBRARIES})
