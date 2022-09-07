#include "iostream"
#include <ctime>

using namespace std;

#include <Eigen/Core> //eigen core
#include <Eigen/Dense> //稠密矩阵的代数运算（逆和特征值之类的）

using namespace Eigen;

#define Max_size 50

int main() {
    //这里向量矩阵都用Eigen::Matrix，opencv再说opencv
    Matrix<float, 2, 3> matrix_23f;//2*3的float
    Matrix3d matrix_33d = Matrix3d::Zero();//初始化
    Matrix<double, Dynamic, Dynamic> matrix_DDd;//不知道这一维度大小用Dynamic
    MatrixXd matrix_DDd2;//等同于上面，同理还有X3d,X2f...

    matrix_23f << 1, 2, 3, 4, 5, 6;//赋值
    cout << "matrix_23f:\n" << matrix_23f << endl;
    /* output:
     1 2 3
     4 5 6 */

    //用（）访问矩阵元素
    cout << matrix_23f(1, 2) << endl; //line2,col3 因为不同于matlab，从0开始
    // output: 6

    Vector3d vec_3d;
    vec_3d << 1, 2, 3;
    Matrix<double, 2, 1> mat_res1_21d = matrix_23f.cast<double>() * vec_3d;//不同类型得转化
    cout << mat_res1_21d << endl;
    /* output:
     14
     32 */
    cout << mat_res1_21d.transpose() << endl;//输出转置矩阵

    matrix_33d = Matrix3d::Random(); //随机数矩阵
    cout << matrix_33d << endl;
    cout << "sum:" << matrix_33d.sum() << endl; //各元素求和
    cout << "trace:" << matrix_33d.trace() << endl;//迹
    cout << "times 10:\n" << 10 * matrix_33d << endl;//数乘10
    cout << "inverse:\n" << matrix_33d.inverse() << endl;//逆
    cout << "det:" << matrix_33d.determinant() << endl;//行列式

    //特征值
    SelfAdjointEigenSolver<Matrix3d> eigen_solver(
            matrix_33d.transpose() * matrix_33d);//matrix times its transpose can get a self ad-joint matrix
    cout << "eigen value:\n" << eigen_solver.eigenvalues() << endl;//特征值
    cout << "eigen vectors:\n" << eigen_solver.eigenvectors() << endl;//特征向量

    //解方程
    //mat*x=vec
    Matrix<double, Max_size, Max_size> matrix_MMd = MatrixXd::Random(Max_size, Max_size);
    matrix_MMd = matrix_MMd * matrix_MMd.transpose();//保证半正定
    Matrix<double, Max_size, 1> vec_Md = MatrixXd::Random(Max_size, 1);
    clock_t time_sst = clock();//计时
    //直接求逆，慢
    Matrix<double, Max_size, 1> x = matrix_MMd.inverse() * vec_Md;
    cout << "time of normal inverse:"
         << 1000 * (clock() - time_sst) / (double) CLOCKS_PER_SEC << "ms" << endl;
    cout << "x.trans:" << x.transpose() << endl;

    //使用矩阵分解，如QR分解，快
    time_sst = clock();
    x = matrix_MMd.colPivHouseholderQr().solve(vec_Md);
    cout << "time of QR decomposition:"
         << 1000 * (clock() - time_sst) / (double) CLOCKS_PER_SEC << "ms" << endl;
    cout << "x.trans:" << x.transpose() << endl;

    //对于正定矩阵，可以用cholesky分解,超快
    time_sst = clock();
    x = matrix_MMd.ldlt().solve(vec_Md);
    cout << "time of ldlt decomposition:"
         << 1000 * (clock() - time_sst) / (double) CLOCKS_PER_SEC << "ms" << endl;
    cout << "x.trans:" << x.transpose() << endl;
    return 0;
}