#pragma once

#include "AsCoordinate.h"
#include "AsMath.h"
#include "AsAttitudeParam.h"

///***********************************************************************
/// The conversion from matrix to axis and angle.
/// @Author	Xiao Yao
/// @Date	2023.4.2
/// @Input
/// @Param	mtx	    ת
/// @Output
/// @Param	axis	תʸ
/// @Param	angle	ת[0, pi]
///***********************************************************************
void AsMtxToAxAng(const CMatrix<double>& mtx,
	CCoord& axis, double& angle);

//************************************************
//*将四元数转化成欧拉角_123转序（欧拉角单位为度）*
//************************************************
//输入：
//		标准化四元数：q
//		代求欧拉角：  Ang
//		卫星类型选择：cho (cho=1：自旋卫星；cho=3：非自旋卫星)
//************************************************
bool AsQuatToEuler_123(const CQuaternion& q, CEuler& Ang, const int cho)
{
	//设置默认参数
	double pi = acos(-1);   //计算pi值
	double deg = 180 / pi;    //弧度->度的转换
	double epsilon = 1e-6;  //精确度
	Ang = { 0, 0, 0 };
	//q0, q1, q2, q3分别对应q.m_Qs, q.m_Qx, q.m_Qy, q.m_Qz
	double q0q0 = q.m_Qs*q.m_Qs;
	double q1q1 = q.m_Qx*q.m_Qx;
	double q2q2 = q.m_Qy*q.m_Qy;
	double q3q3 = q.m_Qz*q.m_Qz;
	double q_norm2 = q0q0 + q1q1 + q2q2 + q3q3;
	if (abs(q_norm2 - 1) > epsilon)   //判断输入的四元数是否标准化
	{
		cout << "请输入标准化四元数！！！" << endl;
		return false;
	}
	else if (abs(q0q0 - 1) < epsilon) {}   //没有旋转时，直接输出
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

        //判断是否出现奇异，当出现奇异时，令 Angle3或Angle1 为 0，确保程序可以继续运行
		if (sy - 1 >= 0)   //判断俯仰角为pi/2
		{
			Ang.m_Angle2 = 90;
			Ang.m_Angle1 = (3 - cho)*atan(q.m_Qx / q.m_Qy)*deg;
			Ang.m_Angle3 = (cho - 1)*atan(q.m_Qx / q.m_Qy)*deg;
		}
		else if (sy + 1 <= 0)   //判断俯仰角为-pi/2
		{
			Ang.m_Angle2 = -90;
			Ang.m_Angle1 = -(3 - cho)*atan(q.m_Qx / q.m_Qy)*deg;
			Ang.m_Angle3 = -(cho - 1)*atan(q.m_Qx / q.m_Qy)*deg;
		}
		else
		{
			Ang.m_Angle2 = asin(2 * (q1q3 + q0q2))*deg;
			Ang.m_Angle1 = atan2(2 * (q0q1 - q2q3), a02 - a13)*deg;
			Ang.m_Angle3 = atan2(2 * (q0q3 - q1q2), a02 + a13)*deg;
		}
	}
	return true;
};