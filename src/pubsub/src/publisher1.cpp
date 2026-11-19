    #include <chrono>
    #include <functional>
    #include <memory>
    #include <string>
    #include <cmath>
    #include <numbers>
    #include <random>

    #include "rclcpp/rclcpp.hpp"
    #include "std_msgs/msg/float64.hpp"

    using namespace std::chrono_literals;


    class SinePublisher : public rclcpp::Node
    {
        public:
            SinePublisher()
            : Node("sine_publisher")
            {
                publisher_ = this -> create_publisher<std_msgs::msg::Float64>("signal", 10);

                this -> declare_parameter("frequency", 1.0);
                this -> declare_parameter("amplitude", 1.0);

                frequency_ = this -> get_parameter("frequency").as_double();
                amplitude_ = this -> get_parameter("amplitude").as_double();

                timer_ = this -> create_wall_timer(10ms, std::bind(&SinePublisher::timer_callback, this));
            }

        private:
            
            void timer_callback()
            {
                clean_sine_value_ = amplitude_ * std::sin(2*pi_*frequency_*time_);
                signal_ = clean_sine_value_ + dist(generator_);
                message_.data = signal_;

                //RCLCPP_INFO(this->get_logger(), "Pubblicato: '%f'", message_.data);
                //RCLCPP_INFO(this->get_logger(), "Frequency: %f", frequency_);

                publisher_->publish(message_);

                time_ += dt_;
            }

            double dt_ = 0.01;
            double time_ = 0.0;
            double amplitude_;
            double frequency_;
            double pi_ = std::numbers::pi;

            double signal_;
            double clean_sine_value_;

            std::default_random_engine generator_{std::random_device{}()};
            std::normal_distribution<double> dist{0.0, 0.1};

            std_msgs::msg::Float64 message_;

            rclcpp::TimerBase::SharedPtr timer_;
            rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;

    };

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SinePublisher>());
    rclcpp::shutdown();
    return 0;
}