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
//����������̬���Һ���������R-K���֣����ظ���Ĵ��麯��
///@author	Wang Hua
//Date:		2004.12.9
//Version:	1.0
//Input:	t			�Ա�����ֵ
//			x			��ʼ����ֵ
//Output:	result		����õ��ĺ���ֵ
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

	//���������Ķ���ѧ����
	accel = m_Force.RotateTo(quat.Conjugate()) / m_Mass;
	for (i=0; i<3; i++)
	{
		result[i]   = x[i+3];
		result[i+3] = accel[i];
	}

	//��̬��Ԫ���˶�ѧ�����Һ���
	result[6] = 0.5*(-quat.m_Qx*angVel[0]-quat.m_Qy*angVel[1]-quat.m_Qz*angVel[2]);
	result[7] = 0.5*( quat.m_Qs*angVel[0]-quat.m_Qz*angVel[1]+quat.m_Qy*angVel[2]);
	result[8] = 0.5*( quat.m_Qz*angVel[0]+quat.m_Qs*angVel[1]-quat.m_Qx*angVel[2]);
	result[9] = 0.5*(-quat.m_Qy*angVel[0]+quat.m_Qx*angVel[1]+quat.m_Qs*angVel[2]);

	//��̬����ѧ�����Һ���
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
/// ����6���ɶ��˶�����
/// @Author	Wang Hua
/// @Date	2010-4-15
/// @Input	
/// @Param	step		���沽��								sec
/// @Param	mass		����									kg
/// @Param	force		���ܿ�����(��ϵ)
/// @Param	torque		���ܿ�������(��ϵ)
/// @Param	inertia		ת������
/// @In/Out
/// @Param	pos			������λ��
/// @Param	vel			�ٶ�
/// @Param	quat		��������ϵ��Թ���ϵ����Ԫ��
/// @Param	angVel		��������ϵ�б�ʾ����Թ���ϵ�Ľ��ٶ�
//***********************************************************************
void CDoF6::DoF6 (double step, double mass, 
			const CMatrix<double>& inertia, const CCoord& force, const CCoord& torque, 
			CCoord& pos, CCoord& vel, CQuaternion& quat, CCoord& angVel)
{
	int i;
	CVector<double> x(13);

	//�����Һ����еĲ���
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

	//����RK4�����������������ɶȶ���ѧģ��
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