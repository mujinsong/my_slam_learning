#include "ros/ros.h"
#include "actionlib/client/simple_action_client.h"
#include "actionlib/client/terminal_state.h"
#include "slam_book_navg/CountDownAction.h"

#include "iostream"
using namespace std;
void doneCB(const actionlib::SimpleClientGoalState& state,const slam_book_navg::CountDownResultConstPtr& result) {
    ROS_INFO("done");
    ros::shutdown();
}

void activeCB() {
    ROS_INFO("active");
}

void feedbackCB(const slam_book_navg::CountDownFeedbackConstPtr& feedback) {
    ROS_INFO("feedback:[%f,%d]",feedback->count_percent,feedback->count_current);
}

int main(int argc,char **argv) {
    ros::init(argc,argv,"action_client_node");
    ros::NodeHandle nh;

    actionlib::SimpleActionClient<slam_book_navg::CountDownAction> ac("/count_down",true);
    ROS_INFO("wait for action server to start");
    ac.waitForServer();
    slam_book_navg::CountDownGoal goal;

    cout<<"input number and step"<<endl;
    cin>>goal.target_number>>goal.target_step;
    ac.sendGoal(goal,&doneCB,&activeCB,&feedbackCB);
    ros::spin();
    return 0;
}