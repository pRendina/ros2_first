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

                timer_ = this -> create_wall_timer(10ms, std::bind(&SinePublisher::timer_callback, this));
            }

        private:
            
            void timer_callback()
            {
                clean_sine_value_ = amplitude_ * std::sin(2*pi*frequency_*time_);
                signal_ = clean_sine_value + dist(generator);
                message.data = signal_;

                RCLCPP_INFO(this->get_logger(), "Pubblicato: '%f'", message.data.c_str());
                publisher_->publish(message);

                time_ += 0.01;
            }


            double time_ = 0.0;
            double amplitude_ = 1.0;
            double frequency_ = 5.0;
            double pi = std::numbers::pi;

            double signal_;
            double clean_sine_value_;

            std::default_random_engine generator;
            std::normal_distribution<double> dist{0.0, 0.1};

            std_msgs::msg::Float64 message;

            rclcpp::TimerBase::SharedPtr timer_;
            rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;

    }