#include "twist_to_dds.hpp"


TwistToDDS::TwistToDDS() : Node("twist_to_dds_node")
{
    // ROS2 twist
    std::string twist_topic_name = this->declare_parameter<std::string>("twist_topic", "/twist_vel");
    subscription_ = this->create_subscription<geometry_msgs::msg::TwistStamped>( twist_topic_name, 10, std::bind(&TwistToDDS::topic_callback, this, std::placeholders::_1));
        RCLCPP_INFO(this->get_logger(), "Subscribed to topic: %s", twist_topic_name.c_str());

    //ROS2 state and control
    // the state_suber is set to subscribe "sportmodestate" topic
    state_suber_ = this->create_subscription<unitree_go::msg::SportModeState>(
        "sportmodestate", 10, std::bind(&TwistToDDS::state_callback, this, std::placeholders::_1));
    // the req_puber is set to subscribe "/api/sport/request" topic with dt
    req_puber_ = this->create_publisher<unitree_api::msg::Request>("/api/sport/request", 10);
    timer_ = this->create_wall_timer(std::chrono::milliseconds(int(dt_ * 1000)), std::bind(&TwistToDDS::timer_callback, this));

    t_ = -1; // Runing time count
}

void TwistToDDS::topic_callback( const geometry_msgs::msg::TwistStamped::SharedPtr msg )
{
    RCLCPP_INFO(this->get_logger(), "Linear X: %.2f, Linear Y: %.2f, Angular Z: %.2f", msg->twist.linear.x, msg->twist.linear.y, msg->twist.angular.z);
    sport_req_.Move(req_, msg->twist.linear.x, msg->twist.linear.y, msg->twist.angular.z);
    req_puber_->publish(req_);
}

void TwistToDDS::timer_callback()
{

}

void TwistToDDS::state_callback(unitree_go::msg::SportModeState::SharedPtr data)
{
    if (t_ < 0)
    {
        // Get initial position
        px0_ = data->position[0];
        py0_ = data->position[1];
        yaw0_ = data->imu_state.rpy[2];
        // std::cout << px0_ << ", " << py0_ << ", " << yaw0_ << std::endl;
    }
}

int main(int argc, char * argv[]) 
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TwistToDDS>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}