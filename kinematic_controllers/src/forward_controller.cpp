#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Bool.h>

const double PUBLISH_FREQUENCY = 10.0;
const double DEFAULT_VELOCITY = 0.5;

double _velocity = DEFAULT_VELOCITY;
bool _active;

void callback_forward_velocity(const std_msgs::Float64ConstPtr& vel) {
    _velocity = vel->data;
}

void callback_activate(const std_msgs::BoolConstPtr& val) {
    _active = val->data;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "forward_controller");

    ros::NodeHandle n;
    ros::Subscriber sub_vel = n.subscribe("/controller/forward/velocity", 1, callback_forward_velocity);
    ros::Subscriber sub_act = n.subscribe("/controller/forward/active",   1, callback_activate);
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/controller/forward/twist", 1);

    ros::Rate loop_rate(PUBLISH_FREQUENCY);

    while (ros::ok()) {

        geometry_msgs::Twist twist;

        if (_active)
            twist.linear.x = _velocity;
        else
            twist.linear.x = 0;

        pub.publish(twist);

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
