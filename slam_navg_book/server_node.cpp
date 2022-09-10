#include "ros/ros.h"
#include "slam_book_navg/AddTwoInts.h"

bool add_execute(slam_book_navg::AddTwoInts::Request &req,slam_book_navg::AddTwoInts::Response &res) {
    res.sum =req.a+req.b;
    ROS_INFO("recieve request: a=%ld,b=%ld",(long int)req.a,(long int)req.b);
    ROS_INFO("send response: sum=%ld",(long int)res.sum);
    return true;
}

int main(int argc,char **argv) {
    ros::init(argc,argv,"server_node");
    ros::NodeHandle nh;

    ros::ServiceServer service = nh.advertiseService("add_two_ints",add_execute);
    ROS_INFO("ready");
    ros::spin();
    return 0;
}