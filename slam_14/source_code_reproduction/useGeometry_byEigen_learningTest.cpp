#include "iostream"
#include "vector"
#include "cmath"
using namespace std;

#include "Eigen/Core"
#include "Eigen/Geometry"
using namespace Eigen;

int coordinateTransform() {
    /* 世界坐标系为w，萝卜坐标为r1,r2。萝卜一号的位姿为q1，t1，二号位姿为q2，t2
       萝卜一号某点在自身坐标系下坐标为pr1，求在二号坐标系下坐标
     */
    Quaterniond q1(0.35,0.2,0.3,0.1),q2(-0.5,0.4,-0.1,0.2);
    q1.normalize();//归一化
    q2.normalize();
    Vector3d pr1(0.5,0,0.2),t1(0.3,0.1,0.1),t2(-0.1,0.5,0.3);
    Isometry3d T1w(q1),T2w(q2);
    T1w.pretranslate(t1);
    T2w.pretranslate(t2);
    Vector3d pr2=T2w*T1w.inverse()*pr1;
    cout<<pr2.transpose()<<endl;
    return 1;
}

int main() {
    //3D旋转矩阵用Matrix3d or 3f
    Matrix3d rotation_matrix = Matrix3d::Identity();//Identity()初始化
    //旋转向量用AngleAxis，底层不是Matrix，运算可以当矩阵（重载运算符了）
    AngleAxisd rotation_vec(M_PI/4,Vector3d(0,0,1));//沿Z轴转45度
    cout.precision(3);//精确到小数点后三位
    cout<<"rotation_matrix:\n"<<rotation_vec.matrix()<<endl;//转换为矩阵输出
    //矩阵也可以直接赋值
    rotation_matrix = rotation_vec.toRotationMatrix();
    //用AngleAxis进行坐标变换
    Vector3d v(1,0,0);
    Vector3d v_rotation = rotation_vec * v;
    cout<<"(1,0,0) after rotation by angle axis:\n"<<v_rotation.transpose()<<endl;
    //旋转矩阵
    v_rotation = rotation_matrix*v;
    cout<<"(1,0,0) after rotation by matrix:\n"<<v_rotation.transpose()<<endl;

    //欧拉角
    Vector3d euler_angles = rotation_matrix.eulerAngles(2,1,0);//ZYX顺序，即roll pitch yaw，括号里的数就是顺序
    cout<<"yaw pitch roll: "<<euler_angles.transpose()<<endl;

    //欧氏变换矩阵Eigen::Isometry
    Isometry3d T = Isometry3d::Identity();//虽然是3d，但是是4*4的矩阵
    T.rotate(rotation_vec);//旋转
    T.pretranslate(Vector3d(1,3,4));//平移
    cout<<"Transform mat:\n"<<T.matrix()<<endl;

    //用变换矩阵进行坐标变换
    Vector3d v_transformed =T*v;//R*v+t
    cout<<"v transformed:\n"<<v_transformed.transpose()<<endl;

    //四元数
    //可以将AngleAxis赋值给四元数，反之亦然
    Quaterniond q =Quaterniond(rotation_vec);
    cout<<"quaternion from rotation vector:"<<q.coeffs().transpose()<<endl;//coeffs的顺序为xyzw，其中w为实部
    q=Quaterniond(rotation_matrix);//通过旋转矩阵赋值
    cout<<"quaternion from rotation matrix:"<<q.coeffs().transpose()<<endl;
    v_rotation=q*v;//数学上是qvq^(-1)
    cout<<"(1,0,0) after rotation:"<<v_rotation.transpose()<<endl;
    cout<<"should be equal to "<<(q*Quaterniond(0,1,0,0)*q.inverse()).coeffs().transpose()<<endl;
    coordinateTransform();
    return 0;
}