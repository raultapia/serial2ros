/**
 * @file serial2ros_node.hpp
 * @brief Serial to ROS
 * @author Raul Tapia (raultapia.com)
 * @copyright The Unlicense
 * @see https://github.com/raultapia/serial2ros
 */
#include <cctype>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <ros/console.h>
#include <ros/init.h>
#include <ros/node_handle.h>
#include <ros/publisher.h>
#include <serial/serial.h>
#include <sstream>
#include <std_msgs/Int32MultiArray.h>
#include <string>
#include <thread>
#include <vector>

class Serial {
public:
  explicit Serial(ros::NodeHandle &nh) {
    const std::string port = "/dev/ttyACM0";
    const int baudrate = 9600;
    serial::Timeout to = serial::Timeout::simpleTimeout(1000);
    nh.param<std::string>("port", port);
    nh.param<int>("baudrate", baudrate);
    pub = nh.advertise<std_msgs::Int32MultiArray>("serial2ros_topic", 10);
    msg.layout.dim.resize(1);
    msg.layout.dim[0].label = "length";
    msg.layout.dim[0].stride = 1;

    try {
      s.setPort(port);
      s.setBaudrate(baudrate);
      s.setTimeout(to);
      s.open();
      ROS_INFO_STREAM("Connected to port " << port << " @ " << baudrate);
    } catch(serial::IOException &e) {
      ROS_ERROR_STREAM("Unable to open port " << port);
    }

    reset();
  }

  void read() {
    if(!static_cast<bool>(s.available())) {
      return;
    }
    buffer += s.read(s.available());
    if(!isValid(buffer)) {
      ROS_WARN("Error in serial buffer. Resetting...");
      reset();
    }

    while(true) {
      const size_t pos = buffer.find('\n');
      if(pos != std::string::npos) {
        const std::string next = buffer.substr(0, pos);
        buffer.erase(0, pos + 1);
        msg.data = parse(next);
        msg.layout.dim[0].size = msg.data.size();
        pub.publish(msg);
      } else {
        break;
      }
    }
  }

private:
  serial::Serial s;
  std::string buffer;
  std_msgs::Int32MultiArray msg;
  ros::Publisher pub;

  static std::vector<int> parse(const std::string &str) {
    std::vector<int> result;
    std::stringstream ss(str);
    std::string item;
    while(std::getline(ss, item, ';')) {
      result.push_back(std::stoi(item));
    }
    return result;
  }

  static bool isValid(const std::string &str) {
    if(str.size() > 50000) {
      return false;
    }
    for(const char c : str) {
      if(!isdigit(c) && c != ';' && c != 10 && c != 13) {
        return false;
      }
    }
    return true;
  }

  void reset() {
    uint8_t c;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    buffer.clear();
    while(s.read(&c, 1) != 1 || c != '\n') {
      std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
  }
};

int main(int argc, char **argv) {
  ros::init(argc, argv, "serial2ros");
  ros::NodeHandle nh("~");
  Serial serial(nh);

  while(ros::ok()) {
    serial.read();
  }

  return 0;
}
