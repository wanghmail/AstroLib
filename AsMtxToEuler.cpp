///***********************************************************************
/// 方向余弦矩阵转为312转序Euler角
/// @Author	Zhang Xuaying
/// @Date	2023.04.01
/// @Input  Mtx  	   方向余弦矩阵
/// @Param	
/// @Param	
/// @Output	eulerangles	欧拉角
///***********************************************************************
#include "AsMtxToEuler.h"
#include "SimDoF6.h"
#include "DoF6.h"

const double PI = 3.141592653;


//定义欧拉角的结构体
struct Angles
{
	double m_Angle2, m_Angle1, m_Angle3;
};


CVector<double> AsMtxToEuler(CMatrix<double>& Mtx)
{
	CMatrix<double> mtx(3, 3);//定义一个矩阵用于存放方向余弦矩阵的数据
	Angles angles; //定义一个结构体，用于储存欧拉角
	mtx = Mtx;

	CVector<double> eulerangles(3);

	bool singular = mtx(1, 2) > 1 - 1e-8;//判断是否出现欧拉角奇异问题	
	if (!singular)
	{
		angles.m_Angle2 = asin(mtx(1, 2)) / PI * 180;
		if (mtx(1, 0) == 0)
		{
			angles.m_Angle1 = 0;
		}
		else
		{
			angles.m_Angle1 = -atan2(mtx(1, 0), mtx(1, 1)) / PI * 180;//偏航
		}

		if (mtx(0, 2) == 0)
		{
			angles.m_Angle3 = 0;
		}
		else
		{
			angles.m_Angle3 = -atan2(mtx(0, 2), mtx(2, 2)) / PI * 180;//俯仰
		}
	}
	else
	{
		cout << "出现欧拉角数值奇异问题" << endl;
		angles.m_Angle1 = 0;
		angles.m_Angle2 = 90;
		angles.m_Angle3 = atan2(mtx(2, 0), mtx(0, 0));
	}
	eulerangles[0] = angles.m_Angle1;
	eulerangles[1] = angles.m_Angle2;
	eulerangles[2] = angles.m_Angle3;
	return eulerangles;
}