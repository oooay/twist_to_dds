#include <memory>
#include "rclcpp/rclcpp.hpp"
#include <geometry_msgs/msg/twist_stamped.hpp>
#include "unitree_go/msg/sport_mode_state.hpp"
#include <unitree_go/msg/sport_mode_cmd.hpp>
#include "unitree_api/msg/request.hpp"
#include "common/ros2_sport_client.h"
// #include <unitree/robot/go2/sport/sport_client.hpp>
// #include <unitree/robot/client/client.hpp>

#include <unistd.h>

class TwistToDDS : public rclcpp::Node
{
    public:
    TwistToDDS(); 

    private:
    void topic_callback(const geometry_msgs::msg::TwistStamped::SharedPtr msg);

    private:
    // ros1
    rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr subscription_;
    void timer_callback();
    void state_callback(unitree_go::msg::SportModeState::SharedPtr data);

    // ros2
    rclcpp::Subscription<unitree_go::msg::SportModeState>::SharedPtr state_suber_;
    rclcpp::TimerBase::SharedPtr timer_; // ROS2 timer
    rclcpp::Publisher<unitree_api::msg::Request>::SharedPtr req_puber_;
    unitree_api::msg::Request req_; // Unitree Go2 ROS2 request message
    SportClient sport_req_;
    double dt_ = 0.002; //control time step
    double t_;
    double px0_ = 0;  // initial x position
    double py0_ = 0;  // initial y position
    double yaw0_ = 0; // initial yaw angle
};