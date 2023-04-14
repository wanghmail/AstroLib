
#include "AsAttitudeParam_2023.h"
#include "AsCoordinate.h"
#include "AsMath.h"
#include "AsAttitudeParam.h"
#include <iostream>


using namespace std;


///***********************************************************************
/// The conversion from matrix to axis and angle.
/// @Author	Xiao Yao
/// @Date	2023.4.4
/// @Input
/// @Param	mtx	    坐标转换矩阵
/// @Output
/// @Param	axis	旋转轴矢量
/// @Param	angle	旋转角[0, pi]
///***********************************************************************
void AsMtxToAxAng(const CMatrix<double>& mtx,
	CCoord& axis, double& angle)
{

	CMatrix<double> mm = mtx;
	//************** 判断是否为正交矩阵 ******************
	double p1 = mm[0][0] * mm[0][1] + mm[1][0] * mm[1][1] + mm[2][0] * mm[2][1];
	double p2 = mm[0][0] * mm[0][2] + mm[1][0] * mm[1][2] + mm[2][0] * mm[2][2];
	double p3 = mm[0][2] * mm[0][1] + mm[1][2] * mm[1][1] + mm[2][2] * mm[2][1];
	double p4 = mm[0][0] * mm[0][0] + mm[1][0] * mm[1][0] + mm[2][0] * mm[2][0];
	double p5 = mm[0][1] * mm[0][1] + mm[1][1] * mm[1][1] + mm[2][1] * mm[2][1];
	double p6 = mm[0][2] * mm[0][2] + mm[1][2] * mm[1][2] + mm[2][2] * mm[2][2];
	if (!(abs(p1)<1e-9 && abs(p2)<1e-9 && abs(p3)<1e-9 && 
		  abs(p4-1)<1e-9 && abs(p5 - 1)<1e-9 && abs(p6 - 1)<1e-9) ){
		printf("warning! not Orthogonal Matrix\n");
		angle = 0;
		axis[0] = 0;
		axis[1] = 0;
		axis[2] = 0;
		return;
	}

	// 矩阵的迹
	double T = mm[0][0] + mm[1][1] + mm[2][2];

	if (abs(T - 3) > 1e-9 && abs(T + 1) > 1e-9) {
		double cc = 0.5 * (T - 1);   // cos(angle)
		double ss = sin(acos(cc));   // sin(angle)
		angle = atan2(ss, cc);       // (0, pi)
		axis[0] = 0.5 * (mm[1][2] - mm[2][1]) / ss;
		axis[1] = 0.5 * (mm[2][0] - mm[0][2]) / ss;
		axis[2] = 0.5 * (mm[0][1] - mm[1][0]) / ss;
	}
	else if (abs(T + 1) < 1e-9) {
		// 转轴有两个解，指向相差180°,且角度不影响, 本程序仅输出一个
		// sin(angle) = 0， angle=pi+2k*pi,k为整数
		angle = AsCPI;

		double a12 = mm[0][1] / 2.0;
		double a23 = mm[1][2] / 2.0;
		double a31 = mm[2][0] / 2.0;
		double aa1 = sqrt((1.0 + mm[0][0]) / 2.0);
		double aa2 = sqrt((1.0 + mm[1][1]) / 2.0);
		double aa3 = sqrt((1.0 + mm[2][2]) / 2.0);
		if (a12 > 0 && a23 > 0 && a31 > 0) {
			// a1 a2 a3同号
			axis[0] = aa1;
			axis[1] = aa2;
			axis[2] = aa3;
		}
		else if (a12 > 0) {
			// a1 a2 同号
			axis[0] = aa1;
			axis[1] = aa2;
			axis[2] = -aa3;
		}
		else if (a23 > 0) {
			// a2 a3 同号
			axis[0] = aa1;
			axis[1] = -aa2;
			axis[2] = -aa3;
		}
		else {
			// a1 a3同号
			axis[0] = aa1;
			axis[1] = -aa2;
			axis[2] = aa3;
		}
	}
	else {
		// 转轴不确定
		angle = 0;
		axis[0] = 1;
		axis[1] = 0;
		axis[2] = 0;
	}
}


///***********************************************************************
///将四元数转化成欧拉角_123转序
/// @Author	Liu Tianqing
/// @Date	2023.04.06
/// @Input
/// @Param	q		标准化四元数
/// @Output
/// @Param	Ang		欧拉角(rad)
///***********************************************************************
bool AsQuatToEuler123(const CQuaternion& q, CEuler& Ang)
{
	//设置默认参数
	double epsilon = 1e-6;		//精确度
	Ang = { 0, 0, 0 };
	//q0, q1, q2, q3分别对应q.m_Qs, q.m_Qx, q.m_Qy, q.m_Qz
	double q0q0 = q.m_Qs*q.m_Qs;
	double q1q1 = q.m_Qx*q.m_Qx;
	double q2q2 = q.m_Qy*q.m_Qy;
	double q3q3 = q.m_Qz*q.m_Qz;
	double q_norm2 = q0q0 + q1q1 + q2q2 + q3q3;
	if (abs(q_norm2 - 1) > epsilon)			//判断输入的四元数是否标准化
	{
		cout << "请输入标准化四元数！！！" << endl;
		return false;
	}
	else if (abs(q0q0 - 1) < epsilon) {}	//没有旋转时，直接输出
	else
	{
		double q0q1 = q.m_Qs*q.m_Qx;
		double q0q2 = q.m_Qs*q.m_Qy;
		double q0q3 = q.m_Qs*q.m_Qz;
		double q1q2 = q.m_Qx*q.m_Qy;
		double q1q3 = q.m_Qx*q.m_Qz;
		double q2q3 = q.m_Qy*q.m_Qz;

		double a02 = q0q0 - q2q2;
		double a13 = q1q1 - q3q3;
		double sy = 2 * (q1q3 + q0q2);   //俯仰角 angle.y 的正弦值

		//判断是否出现奇异，当出现奇异时，令 Angle3或Angle1 为 0，确保程序可以继续运行，这里默认 Angle3为0
		if (sy - 1 >= 0)		//判断俯仰角为pi/2
		{
			Ang.m_Angle2 = AsCHalfPI;
			Ang.m_Angle1 = 2*atan(q.m_Qx / q.m_Qy);
		}
		else if (sy + 1 <= 0)   //判断俯仰角为-pi/2
		{
			Ang.m_Angle2 = -AsCHalfPI;
			Ang.m_Angle1 = -2*atan(q.m_Qx / q.m_Qy);
		}
		else
		{
			Ang.m_Angle2 = asin(2 * (q1q3 + q0q2));
			Ang.m_Angle1 = atan2(2 * (q0q1 - q2q3), a02 - a13);
			Ang.m_Angle3 = atan2(2 * (q0q3 - q1q2), a02 + a13);
		}
	}
	return true;
}


///***********************************************************************
/// 方向余弦阵转321转序欧拉角
/// @Author	Li Sibei
/// @Date	2023.04.06
/// @Input
/// @Param	mtx		方向余弦阵
/// @Output
/// @Param	euler	欧拉角
///***********************************************************************
void AsMtxToEuler321(const CMatrix<double>& mtx, CEuler& euler)
{
	//异常数据处理1：输入非正交矩阵
	CMatrix<double> MTX1(mtx);
	CMatrix<double> mtx1 = MTX1.Transpose();
	CMatrix<double> mtx2 = MTX1.Inv();
	CMatrix<double> mtx3 = mtx1 - mtx2;
	for (int i = 0; i <= 2; i++)
	{
		for (int j = 0; j <= 2; j++)
		{
			if (mtx3[i][j] > 1e-15 || mtx3[i][j] < -1e-15)
			{
				printf("Error: 方向余弦阵非正交矩阵!\n");
				return;
			}
		}
	}
	//异常数据处理2：奇异
	if (mtx[0][2] == 1)
	{
		euler.m_Angle1 = -atan2(mtx[2][1], mtx[1][1]);
		euler.m_Angle2 = -asin(mtx[0][2]);
		euler.m_Angle3 = 0;
		return;
	}
	else if (mtx[0][2] == -1)
	{
		euler.m_Angle1 = atan2(mtx[2][1], mtx[1][1]);
		euler.m_Angle2 = -asin(mtx[0][2]);
		euler.m_Angle3 = 0;
		return;
	}
	//一般情况求解
	else
	{
		euler.m_Angle1 = atan2(mtx[0][1], mtx[0][0]);
		euler.m_Angle2 = -asin(mtx[0][2]);
		euler.m_Angle3 = atan2(mtx[1][2], mtx[2][2]);
	}
}


///***********************************************************************
/// 方向余弦矩阵转为313转序Euler角
/// @Author	Wang Weili
/// @Date	2023.4
/// @Input
/// @Param	mtx		坐标转移矩阵
/// @Output
/// @Param	euler	欧拉角(rad)
///***********************************************************************
void AsMtxToEuler313(const CMatrix<double>& mtx, CEuler& euler)
{
	if (mtx[2][2] < +1)
	{
		if (mtx[2][2] > -1)
		{
			euler.m_Angle1 = atan2(mtx[2][0], -mtx[2][1]);
			euler.m_Angle2 = acos(mtx[2][2]);
			euler.m_Angle3 = atan2(mtx[0][2], mtx[1][2]);
		}
		else //mtx[2][2]==-1
		{
			euler.m_Angle1 = atan2(mtx[0][1], mtx[0][0]);
			euler.m_Angle2 = AsCPI;
			euler.m_Angle3 = 0;
		}
	}
	else //mtx[2][2]==+1
	{
		euler.m_Angle1 = atan2(mtx[0][1], mtx[0][0]);
		euler.m_Angle2 = 0;
		euler.m_Angle3 = 0;
	}
}


///***********************************************************************
/// 方向余弦矩阵转为312转序Euler角
/// @Author	Zhang Xuaying
/// @Date	2023.04.01
/// @Input  
/// @Param	mtx  		方向余弦矩阵
/// @Output	
/// @Param	euler		欧拉角(rad)
///***********************************************************************
void AsMtxToEuler312(const CMatrix<double>& mtx, CEuler& euler)
{
	bool singular = mtx(1, 2) > 1 - 1e-8;//判断是否出现欧拉角奇异问题	
	if (!singular)
	{
		euler.m_Angle2 = asin(mtx(1, 2));
		//偏航
		if (mtx(1, 0) == 0)
		{
			euler.m_Angle1 = 0;
		}
		else
		{
			euler.m_Angle1 = -atan2(mtx(1, 0), mtx(1, 1));
		}

		//俯仰
		if (mtx(0, 2) == 0)
		{
			euler.m_Angle3 = 0;
		}
		else
		{
			euler.m_Angle3 = -atan2(mtx(0, 2), mtx(2, 2));
		}
	}
	else
	{
		cout << "出现欧拉角数值奇异问题 " << endl;
		euler.m_Angle1 = 0;
		euler.m_Angle2 = AsCHalfPI;
		euler.m_Angle3 = atan2(mtx(2, 0), mtx(0, 0));
	}
}


///******************************************************
/// 根据转序和坐标转移矩阵计算Euler角，转序为2-3-1
/// @author Sun Fengyan
/// @data   2023.4
/// @Version    1.0
/// @Input  
/// @Param  mtx       坐标转移矩阵
/// @Output
/// @Param  euler     返回的欧拉角(单位：rad）
///******************************************************
void  AsMtxToEuler231(const CMatrix<double>& mtx, CEuler& euler)
{
	euler.m_Angle2 = asin(mtx[0][1]); 

	if (abs(mtx[0][1] - 1) < 1e-8 ) //euler.m_Angle2 = pi/2
	{
		euler.m_Angle1 = atan2(mtx[1][2], mtx[2][2]);
		euler.m_Angle3 = 0;
	}
	else if (abs(mtx[0][1] + 1) < 1e-8 ) //euler.m_Angle2 = -pi/2
	{
		euler.m_Angle1 = -atan2(mtx[1][2], mtx[2][2]);
		euler.m_Angle3 = 0;
	}
	else 
	{
		euler.m_Angle1 = atan2(-mtx[0][2], mtx[0][0]);
		euler.m_Angle3 = atan2(-mtx[2][1], mtx[1][1]);
		
	}
}

#include <iostream>
#include <cmath>
using namespace std;

class Vector
{
public:
	double x, y, z;

	Vector(double x_, double y_, double z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	Vector operator-(const Vector& vec) const
	{
		return Vector(x - vec.x, y - vec.y, z - vec.z);
	}

	Vector operator*(const Vector& vec) const
	{
		return Vector(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
	}

	double dot(const Vector& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	double norm() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector normalize() const
	{
		double length = norm();
		return Vector(x / length, y / length, z / length);
	}
};

class Matrix
{
public:
	double data[3][3];

	Matrix()
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				data[i][j] = 0.0;
			}
		}
	}

	Vector operator*(const Vector& vec) const
	{
		return Vector(data[0][0] * vec.x + data[0][1] * vec.y + data[0][2] * vec.z,
			data[1][0] * vec.x + data[1][1] * vec.y + data[1][2] * vec.z,
			data[2][0] * vec.x + data[2][1] * vec.y + data[2][2] * vec.z);
	}

	Matrix operator*(const Matrix& mat) const
	{
		Matrix result;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					result.data[i][j] += data[i][k] * mat.data[k][j];
				}
			}
		}
		return result;
	}

	void alg(double alg[3][3])//矩阵求逆
	{
		double a1 = data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]);
		double a2 = data[0][1] * (data[1][2] * data[2][0] - data[1][0] * data[2][2]);
		double a3 = data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
		double det = a1 + a2 + a3;
		if (det == 0)
		{
			cout << "矩阵不可逆";
		}
		else
		{
			alg[0][0] = (data[1][1] * data[2][2] - data[1][2] * data[2][1]) / det;
			alg[1][0] = (data[1][2] * data[2][0] - data[1][0] * data[2][2]) / det;
			alg[2][0] = (data[1][0] * data[2][1] - data[2][0] * data[1][1]) / det;
			alg[0][1] = (data[0][2] * data[2][1] - data[0][1] * data[2][2]) / det;
			alg[1][1] = (data[0][0] * data[2][2] - data[2][0] * data[0][2]) / det;
			alg[2][1] = (data[0][1] * data[2][0] - data[0][0] * data[2][1]) / det;
			alg[0][2] = (data[0][1] * data[1][2] - data[1][1] * data[0][2]) / det;
			alg[1][2] = (data[1][0] * data[0][2] - data[0][0] * data[1][2]) / det;
			alg[2][2] = (data[0][0] * data[1][1] - data[1][0] * data[0][1]) / det;
		}
	}
};

void LvlhToInertial(const Vector& pos, const Vector& vel, double s[3][3])
{
	Vector r(pos.x, pos.y, pos.z);
	Vector v(vel.x, vel.y, vel.z);
	Vector h = r * v;
	Vector e_z = h.normalize();
	Vector e_x = r.normalize();
	Vector e_y = e_z * e_x;

	Matrix lvlh_to_intertial;
	lvlh_to_intertial.data[0][0] = e_x.x; lvlh_to_intertial.data[0][1] = e_y.x; lvlh_to_intertial.data[0][2] = e_z.x;
	lvlh_to_intertial.data[1][0] = e_x.y; lvlh_to_intertial.data[1][1] = e_y.y; lvlh_to_intertial.data[1][2] = e_z.y;
	lvlh_to_intertial.data[2][0] = e_x.z; lvlh_to_intertial.data[2][1] = e_y.z; lvlh_to_intertial.data[2][2] = e_z.z;
	lvlh_to_intertial.alg(s);
}

int main()
{
	// 输入卫星的位置和速度
	Vector pos(0.0, 1.0, 0.0);
	Vector vel(1.0, 0.0, 0.0);
	double s[3][3];
	if (pos.x == 0 )
		if (pos.y ==0)
			if (pos.z == 0)
				cout << "Input position error" << endl;//对错误的位置输入进行报告
	if (vel.x==0)
		if (vel.x==0)
			if (vel.z==0)
				cout << "Input velocity error" << endl;//对错误的速度输入进行报告
	

	// 计算从ECI坐标系到LVLH坐标系的旋转矩阵
	LvlhToInertial(pos, vel, s);

	// 输出旋转矩阵
	cout << "The transformation matrix from ECI frame to LVLH frame is:" << endl;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << s[i][j] << "\t";
		}
		cout << endl;
	}

	return 0;
}