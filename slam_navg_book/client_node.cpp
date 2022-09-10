#include "ros/ros.h"
#include "slam_book_navg/AddTwoInts.h"

#include "iostream"

using namespace std;

int main(int argc,char **argv) {
    ros::init(argc,argv,"client_code");
    ros::NodeHandle nh;
    
    ros::ServiceClient client= nh.serviceClient<slam_book_navg::AddTwoInts>("add_two_ints");
    slam_book_navg::AddTwoInts srv;

    while (ros::ok())
    {
        long int a_in,b_in;
        cout<<"input a and b:";
        cin>>a_in>>b_in;

        srv.request.a=a_in;
        srv.request.b=b_in;
        if(client.call(srv)) {
            ROS_INFO("sum=%ld",(long int)srv.response.sum);
        
        } else {
            ROS_INFO("fail to call");
        }
    }
    return 0;
}