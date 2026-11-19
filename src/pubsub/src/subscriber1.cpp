#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

using std::placeholders::_1;

class SignalProcessor : public rclcpp::Node
{
    public:
        SignalProcessor()
        : Node("signal_processor")
        {
            subscriber_ = this -> create_subscription<std_msgs::msg::Float64>(
                "signal", 10, std::bind(&SignalProcessor::topic_callback, this, _1)
            );

            derivative_publisher_ = this -> create_publisher<std_msgs::msg::Float64>("derivate", 10);
            integral_publisher_ = this -> create_publisher<std_msgs::msg::Float64>("integral", 10);
        }

    private:

        void topic_callback(const std_msgs::msg::Float64::SharedPtr msg)
        {
            double y_new = msg -> data;

            compute_derivative(y_new);
            compute_integral(y_new);

            y_prev_ = y_;
            y_ = y_new;
            if(count_ < 2) count_++;
        }

        void compute_derivative(double y_new)
        {
            if (count_ == 0) return;

            double derivative;
            if (count_ == 1) {
                derivative = (y_new - y_) / dt_;
            }
            else {
                derivative = (y_new - y_prev_) / (2.0 * dt_);
            }

            std_msgs::msg::Float64 output_derivative;
            output_derivative.data = derivative;
            derivative_publisher_ -> publish(output_derivative);
        }

        void compute_integral(double y_new)
        {
            if (count_ == 0) return;

            integral_ += (y_new - y_) / 2.0 * dt_;
            
            std_msgs::msg::Float64 output_integral;
            output_integral.data = integral_;
            integral_publisher_ -> publish(output_integral);
        }

        double dt_ = 0.01;
        double integral_ = 0.0;
        double y_ = 0.0;
        double y_prev_ = 0.0;
        int count_ = 0;

        rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr subscriber_;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr derivative_publisher_;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr integral_publisher_;
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SignalProcessor>());
    rclcpp::shutdown();
    return 0;
}