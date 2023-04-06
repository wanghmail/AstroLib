///***********************************************************************
/// 轨道六根数变换位置速度
/// @Author	Guan Yuming
/// @Date	2023.04.06
/// @Input
/// @Param  m_SMajAx;           长半轴 a
/// @Param  m_Ecc;              偏心率 e
/// @Param  m_I;                轨道倾角 i
/// @Param  m_RAAN;             升交点赤经 Omega
/// @Param  m_ArgPeri;          近供点角 omega
/// @Param  m_TrueA;            真近点角 theta
/// @Param  m_p;                半通径
/// @Param  m_r;                地心坐标系中，中心天体-第二天体位置矢量
/// @Param  m_v;                地心坐标系中, 速度矢量 
/// @Param  mtx_1,mtx_2,mtx_3   3个坐标变换矩阵
/// @Param  m1,m2               中心天体m1质量，第二天体质量m2
/// @Output
/// 在参考坐标系下，物体坐标为(r*cos(trueA), r*sin(trueA), 0)
///***********************************************************************
#include <iostream>
#include <stdlib.h>
#include "AsVector.h"
#include "AsMatrix.h"
#include <cmath>
#define G 6.67e-11
using namespace std;
void AsOrb2RV(double sMajAx, double ecc, double i, double raan, double argPeri, double trueA,
	double m1,double m2)
{
	double m_r,r, m_p;// 位置矢量和半通径
	if (ecc == 0 || ecc == 1) m_p = sMajAx;
	else if (ecc > 1) m_p = -sMajAx*(1 - ecc*ecc);
	else m_p = sMajAx*(1 - ecc*ecc);
	r = m_p / (1 + ecc*cos(trueA));  //位置矢量和半通径关系
	//第二天体轨道面----中心天体轨道面，需进行3次坐标旋转变换
	//第一次按3 旋转 -raan
	double pi = 3.1415926;
	CMatrix<double> mtx_1(cos(-raan), sin(-raan), 0,
			             -sin(-raan), cos(-raan), 0,
			                                0, 0, 1.0);
	//第二次按1 旋转-i
	CMatrix<double> mtx_2(1.0, 0, 0,
		                  0, cos(-i), sin(-i),
		                  0, -sin(-i), cos(-i));
	//第三次旋转按3 旋转-argPeri
	CMatrix<double> mtx_3(cos(-argPeri), sin(-argPeri), 0,
		                 -sin(-argPeri), cos(-argPeri), 0,
		                                          0, 0, 1.0);
	//初始位置矢量R
	//CVector<double> R(r*cos(trueA), r*sin(trueA),0); 不知道如何转置-，-！
	CMatrix<double> R(3, 1);
	R[0] = r*cos(trueA);
	R[1] = r*sin(trueA);
	R[2] = 0;
	m_r = mtx_3*mtx_2*mtx_1*R;//地心坐标系下的位置矢量
	//速度
	//由于运动中除了trueA，其余轨道根数均不变，
	//且轨道六根数可唯一确定位置矢量，因此位置矢量是trueA的函数。
	//v=(dm_r/dtrueA)*(dtrueA/dt)
	double miu,m_v;
	miu = G*m1*m2;
	CMatrix<double> mtx_4(3, 1);
	mtx_4[0] = -cos(raan)*(sin(argPeri + trueA) + ecc*sin(argPeri)) - 
		sin(raan)*(cos(argPeri + trueA) +
		ecc*cos(argPeri))*cos(i);
	mtx_4[1] = -sin(raan)*(sin(argPeri + trueA) + ecc*sin(argPeri)) + 
		cos(raan)*(cos(argPeri + trueA) +
		ecc*cos(argPeri))*cos(i);
	mtx_4[2] = (cos(argPeri + trueA) + ecc*cos(argPeri))*sin(i);
	m_v = sqrt(miu / m_p)*mtx_4;
	cout << "位置为：" << m_r << endl;
	cout << "速度为：" << m_p << endl;
}