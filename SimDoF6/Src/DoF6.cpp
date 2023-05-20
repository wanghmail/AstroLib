// DoF6.cpp: implementation of the CDoF6 class.
//
//////////////////////////////////////////////////////////////////////

#include "SimDoF6.h"
#include "DoF6.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDoF6::CFuncOrbAtt::CFuncOrbAtt() : m_Inertia(3,3)
{
}

CDoF6::CFuncOrbAtt::~CFuncOrbAtt()
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
void CDoF6::CFuncOrbAtt::operator()(double t, const CVector<double> &x, CVector<double> &result) const
{
	int i;
	CCoord     pos, vel, accel, angVel;
	CQuaternion quat;

	for (i=0; i<3; i++)
	{
		pos[i]    = x[i];
		vel[i]    = x[3+i];
		angVel[i] = x[10+i];
	}
	quat.m_Qs = x[6];
	quat.m_Qx = x[7];
	quat.m_Qy = x[8];
	quat.m_Qz = x[9];

	//飞行器质心动力学方程
	accel = m_Force.RotateTo(quat.Conjugate()) / m_Mass;
	for (i=0; i<3; i++)
	{
		result[i]   = x[i+3];
		result[i+3] = accel[i];
	}

	//姿态四元数运动学方程右函数
	result[6] = 0.5*(-quat.m_Qx*angVel[0]-quat.m_Qy*angVel[1]-quat.m_Qz*angVel[2]);
	result[7] = 0.5*( quat.m_Qs*angVel[0]-quat.m_Qz*angVel[1]+quat.m_Qy*angVel[2]);
	result[8] = 0.5*( quat.m_Qz*angVel[0]+quat.m_Qs*angVel[1]-quat.m_Qx*angVel[2]);
	result[9] = 0.5*(-quat.m_Qy*angVel[0]+quat.m_Qx*angVel[1]+quat.m_Qs*angVel[2]);

	//姿态动力学方程右函数
	angVel = (m_Inertia.Inv())*(m_Torque-angVel*(m_Inertia*angVel));
	for (i=0; i<3; i++)
		result[i+10] = angVel[i];

}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDoF6::CDoF6()
{

}

CDoF6::~CDoF6()
{

}


//***********************************************************************
/// 计算6自由度运动方程
/// @Author	Wang Hua
/// @Date	2010-4-15
/// @Input	
/// @Param	step		仿真步长								sec
/// @Param	mass		质量									kg
/// @Param	force		所受控制力(体系)
/// @Param	torque		所受控制力矩(体系)
/// @Param	inertia		转动惯量
/// @In/Out
/// @Param	pos			飞行器位置
/// @Param	vel			速度
/// @Param	quat		质心坐标系相对惯性系的四元数
/// @Param	angVel		质心坐标系中表示的相对惯性系的角速度
//***********************************************************************
void CDoF6::DoF6 (double step, double mass, 
			const CMatrix<double>& inertia, const CCoord& force, const CCoord& torque, 
			CCoord& pos, CCoord& vel, CQuaternion& quat, CCoord& angVel)
{
	int i;
	CVector<double> x(13);

	//设置右函数中的参数
	m_FuncOrbAtt.m_Force       = force;
	m_FuncOrbAtt.m_Torque      = torque;
	m_FuncOrbAtt.m_Inertia     = inertia;
	m_FuncOrbAtt.m_Mass        = mass;

	for (i=0; i<3; i++)
	{
		x[i]    = pos[i];
		x[3+i]  = vel[i];
		x[10+i] = angVel[i];
	}
	x[6] = quat.m_Qs;
	x[7] = quat.m_Qx;
	x[8] = quat.m_Qy;
	x[9] = quat.m_Qz;

	//利用RK4方法求解飞行器六自由度动力学模型
	double t=0;
	AsODERungeKutta4(m_FuncOrbAtt, step, t, x);

	for (i=0; i<3; i++)
	{
		pos[i]    = x[i];
		vel[i]    = x[3+i];
		angVel[i] = x[10+i];
	}
	quat.m_Qs = x[6];
	quat.m_Qx = x[7];
	quat.m_Qy = x[8];
	quat.m_Qz = x[9];
}