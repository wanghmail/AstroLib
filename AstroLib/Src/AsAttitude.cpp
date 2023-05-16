////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2000-2021                         //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
//
// Attitude.cpp: implementation of the CAttitude class.
//
//////////////////////////////////////////////////////////////////////


#include "AsAttitude.h"
#include "AsDResolve.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAttitude::CFuncAtt::CFuncAtt() : m_Inertia(3,3)
{
}

CAttitude::CFuncAtt::~CFuncAtt()
{

}

//********************************************************************
//计算轨道和姿态的右函数，用于R-K积分，重载父类的纯虚函数
///@author	Wang Hua
//Date:		2004.12.9
//Version:	1.0
//Input:	t			自变量的值
//			x			初始函数值
//Output:	result		计算得到的函数值
//Return:	
//********************************************************************
void CAttitude::CFuncAtt::operator()(double t, const CVector<double> &x, CVector<double> &result) const
{
	int i;
	CCoord      angVel;
	CQuaternion quat;

	for (i=0; i<3; i++)
	{
		angVel[i] = x[i];
	}
	quat.m_Qs = x[3];
    quat.m_Qx = x[4];
    quat.m_Qy = x[5];
    quat.m_Qz = x[6];

	//姿态四元数运动学方程右函数
	result[0] = 0.5*(-quat.m_Qx*angVel[0]-quat.m_Qy*angVel[1]-quat.m_Qz*angVel[2]);
    result[1] = 0.5*(quat.m_Qs*angVel[0]-quat.m_Qz*angVel[1]+quat.m_Qy*angVel[2]);
    result[2] = 0.5*(quat.m_Qz*angVel[0]+quat.m_Qs*angVel[1]-quat.m_Qx*angVel[2]);
    result[3] = 0.5*(-quat.m_Qy*angVel[0]+quat.m_Qx*angVel[1]+quat.m_Qs*angVel[2]);

	//姿态动力学方程右函数
	angVel = (m_Inertia.Inv())*(m_Torque-angVel*(m_Inertia*angVel));
	for (i=0; i<3; i++)
		result[i+4] = angVel[i];
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAttitude::CAttitude()
{

}

CAttitude::~CAttitude()
{

}


//***********************************************************************
/// 计算姿态
/// @Author	Wang Hua
/// @Date	2010-4-15
/// @Input	
/// @Param	step		仿真步长								sec
/// @Param	torque		所受控制力矩(体系)
/// @Param	inertia		转动惯量
/// @In/Out
/// @Param	quat		质心坐标系相对惯性系的四元数
/// @Param	angVel		质心坐标系中表示的相对惯性系的角速度
//***********************************************************************
void CAttitude::AttitudeStep(double step,
			const CMatrix<double>& inertia, const CCoord& torque, 
			CQuaternion& quat, CCoord& angVel)
{
	int i;
	CVector<double> x(7);

	//设置右函数中的参数
	m_FuncAtt.m_Torque      = torque;
	m_FuncAtt.m_Inertia     = inertia;

	for (i=0; i<3; i++)
	{
		x[i] = angVel[i];
	}
	x[3] = quat.m_Qs;
	x[4] = quat.m_Qx;
	x[5] = quat.m_Qy;
	x[6] = quat.m_Qz;

	//利用RK4方法求解姿态模型
	double t=0;
	AsODERungeKutta4(m_FuncAtt, step, t, x);

	for (i=0; i<3; i++)
	{
		angVel[i] = x[i];
	}
	quat.m_Qs = x[3];
    quat.m_Qx = x[4];
    quat.m_Qy = x[5];
    quat.m_Qz = x[6];
}