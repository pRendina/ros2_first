#include <memory>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

using std::placeholders::_1;

class Derivative : public rclcpp::Node
{
    public:
        Derivative()
        : Node("derivative")
        {
            subscriber_ = this -> create_subscription<std_msgs::msg::Float64>(
                "signal", 10, std::bind(&Derivative::topic_callback, this, _1)
            );
        }

    private:

        void topic_callback(const std_msgs::msg::Float64::SharedPtr msg)
        {
            double y_new = msg->data;
            bool has_derivative = false;
            double derivative = 0.0;

            if (count_ == 1) {
                derivative = (y_new - y_) / dt_;
                has_derivative = true;
            } else if (count_ >= 2) {
                derivative = (y_new - y_prev_) / (2 * dt_);
                has_derivative = true;
            }

            if (has_derivative) 
                RCLCPP_INFO(this->get_logger(), "Derivata: '%f'", derivative);
            

            y_prev_ = y_;
            y_ = y_new;
            if (count_ < 2) count_++;
        }


        double dt_ = 0.01;
        double y_prev_ = 0.0;
        double y_ = 0.0;
        int count_ = 0;  

        rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr subscriber_;
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Derivative>());
    rclcpp::shutdown();
    return 0;
}