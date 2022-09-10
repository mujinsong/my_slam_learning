#include "ros/ros.h"
#include "std_msgs/String.h"

using namespace ros;

#include "sstream"

using namespace std;

int main(int argc,char **argv) {
    init(argc,argv,"publish_node");
    NodeHandle nh;

    Publisher chatter_pub = nh.advertise<std_msgs::String>("chatter",1000);
    Rate loop_rate(10);
    int count =0;
    while (ok())
    {
        std_msgs::String msg;
        stringstream ss;
        ss << "hello " << count;
        msg.data = ss.str();
        ROS_INFO("%s",msg.data.c_str());

        chatter_pub.publish(msg);
        spinOnce();
        loop_rate.sleep();
        ++count;
    }
    return 0;
}