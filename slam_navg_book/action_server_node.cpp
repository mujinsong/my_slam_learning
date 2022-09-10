#include "ros/ros.h"
#include "actionlib/server/simple_action_server.h"
#include "slam_book_navg/CountDownAction.h"

#include "string"
#include "boost/bind.hpp"

using namespace std;

class ActionServer {
    private:
        ros::NodeHandle nh_;
        actionlib::SimpleActionServer<slam_book_navg::CountDownAction> as_;

        slam_book_navg::CountDownGoal goal_;
        slam_book_navg::CountDownResult result_;
        slam_book_navg::CountDownFeedback feedback_;
    
    public:
        ActionServer(string name):
            as_(nh_,name,bind(&ActionServer::executeCB,this,_1),false) {
                as_.start();
                ROS_INFO("action server started!");
            }
        ~ActionServer(){}

        void executeCB(const slam_book_navg::CountDownGoalConstPtr &goal) {
            ros::Rate r(1);
            goal_.target_number = goal->target_number;
            goal_.target_step = goal->target_step;
            ROS_INFO("get goal:[%d,%d]",goal_.target_number,goal_.target_step);

            int count_num = goal_.target_number;
            int count_step = goal_.target_step;
            bool flag =true;
            for (int i = 0; i > 0; i=i-count_step)
            {
                if (as_.isPreemptRequested()||!ros::ok()) {
                    as_.setPreempted();
                    flag=false;
                    ROS_INFO("preempted");
                    break;
                }
                feedback_.count_percent=1.0*i/count_num;
                feedback_.count_current=i;
                as_.publishFeedback(feedback_);
                r.sleep();
            }
            if (flag) {
                result_.finish=true;
                as_.setSucceeded(result_);
                ROS_INFO("Succeeded");
            }
        }
};

int main(int argc,char **argv) {
    ros::init(argc,argv,"action_server_node");
    ActionServer my_action_server("/count_down");
    ros::spin();
    return 0;
}