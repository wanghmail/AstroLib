// ExampleAttitude.cpp: implementation of the ExampleAttitude class.
//
//////////////////////////////////////////////////////////////////////

#include "AstroLibExamples.h"
#include "ExampleAttitude.h"


//********************************************************************
/// 姿态参数演示例子.
/// @Author Wang Hua
/// @Date   2008.10.16
//********************************************************************
void ExampleAttitude()
{
	CEuler euler(45*AsCDegToRad, 30*AsCDegToRad, 60*AsCDegToRad);
	CQuaternion q;
	CMatrix<double> mtx(3,3);

	//将按照321顺序旋转的Euler角转换为转移矩阵
	AsEulerToMtx(euler, 321, mtx);

	//将转移矩阵转换为四元数
	AsMtxToQuat(mtx, q);

	//输出结果
    printf("The euler angle is: \n");
    printf("angle1 = %10.6f rad\n", euler.m_Angle1);
    printf("angle2 = %10.6f rad\n", euler.m_Angle2);
    printf("angle3 = %10.6f rad\n", euler.m_Angle3);
    printf("The quaternion is: \n");
    printf("x = %10.6f\n", q.m_Qx);
    printf("y = %10.6f\n", q.m_Qy);
    printf("z = %10.6f\n", q.m_Qz);
    printf("s = %10.6f\n\n", q.m_Qs);
}