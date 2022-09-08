
#include "Eigen/Core"
#include "pangolin/pangolin.h"
#include<Eigen/Geometry>
#include "unistd.h"

using namespace std;
using namespace Eigen;

//path to trajectory file
string trajectory_file="/home/zsm/CLionProjects/slam_learning/example/trajectory.txt";

void DrawTrajectory(vector<Isometry3d,Eigen::aligned_allocator<Isometry3d>>);

int main() {
    vector<Isometry3d,Eigen::aligned_allocator<Isometry3d>> poses;
    //vector的第二个参数是eigen的内存分配器，因为默认的分配器是std的，和eigen的不一样，所以不写的话虽然编译没错，但运行出错
    ifstream fin(trajectory_file);
    if (!fin) {
        cout<<"can't find trajectory file at "<<trajectory_file<<endl;
        return 2;
    }
    while(!fin.eof()) {
        double time,tx,ty,tz,qx,qy,qz,qw;//time 指该位姿的记录时间，t为平移，q为旋转四元数
        fin>>time>>tx>>ty>>tz>>qx>>qy>>qz>>qw;
        Isometry3d Twr(Quaterniond(qw,qx,qy,qz));
        Twr.pretranslate(Vector3d(tx,ty,tz));
        poses.push_back(Twr);
    }
    cout<<"read total: "<<poses.size()<<"pose entries"<<endl;
    //draw trajectory in pangolin
    DrawTrajectory(poses);
    return 0;
}

void DrawTrajectory(vector<Isometry3d,Eigen::aligned_allocator<Isometry3d>> poses) {
    // create pangolin window and plot the trajectory
    pangolin::CreateWindowAndBind("Trajectory Viewer",1024,768);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    pangolin::OpenGlRenderState s_cam (
            pangolin::ProjectionMatrix(1024,768,500,500,512,389,0.1,1000),
            pangolin::ModelViewLookAt(0,-0.1,-1.8,0,0,0,0.0,-1.0,0.0)
            );
    pangolin::View &d_cam= pangolin::CreateDisplay().SetBounds(0.0,1.0,0.0,1.0,-1024.0f/768.0f).SetHandler(new pangolin::Handler3D(s_cam));
    while (!pangolin::ShouldQuit()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        d_cam.Activate(s_cam);
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glLineWidth(2);
        for (size_t i = 0; i < poses.size(); ++i) {
            Vector3d Ow = poses[i].translation();
            Vector3d Xw = poses[i] * (0.1*Vector3d(1,0,0));
            Vector3d Yw = poses[i] *(0.1*Vector3d(0,1,0));
            Vector3d Zw = poses[i]*(0.1*Vector3d(0,0,1));
            glBegin(GL_LINE);
            glColor3f(1.0,0.0,0.0);
            glVertex3d(Ow[0],Ow[1],Ow[2]);
            glVertex3d(Xw[0],Xw[1],Xw[2]);
            glColor3f(0.0,1.0,0.0);
            glVertex3d(Ow[0],Ow[1],Ow[2]);
            glVertex3d(Yw[0],Yw[1],Yw[2]);
            glColor3f(0.0,0.0,1.0);
            glVertex3d(Ow[0],Ow[1],Ow[2]);
            glVertex3d(Zw[0],Zw[1],Zw[2]);
            glEnd();
        }
        //
        for (size_t i = 0; i < poses.size(); ++i) {
            glColor3f(0,0,0);
            glBegin(GL_LINES);
            auto p1=poses[i],p2=poses[i+1];
            glVertex3d(p1.translation()[0],p1.translation()[1],p1.translation()[2]);
            glVertex3d(p2.translation()[0],p2.translation()[1],p2.translation()[2]);
            glEnd();
        }
        pangolin::FinishFrame();
        usleep(5000);
    }
}