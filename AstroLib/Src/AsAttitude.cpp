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
//����������̬���Һ���������R-K���֣����ظ���Ĵ��麯��
///@author	Wang Hua
//Date:		2004.12.9
//Version:	1.0
//Input:	t			�Ա�����ֵ
//			x			��ʼ����ֵ
//Output:	result		����õ��ĺ���ֵ
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

	//��̬��Ԫ���˶�ѧ�����Һ���
	result[0] = 0.5*(-quat.m_Qx*angVel[0]-quat.m_Qy*angVel[1]-quat.m_Qz*angVel[2]);
    result[1] = 0.5*(quat.m_Qs*angVel[0]-quat.m_Qz*angVel[1]+quat.m_Qy*angVel[2]);
    result[2] = 0.5*(quat.m_Qz*angVel[0]+quat.m_Qs*angVel[1]-quat.m_Qx*angVel[2]);
    result[3] = 0.5*(-quat.m_Qy*angVel[0]+quat.m_Qx*angVel[1]+quat.m_Qs*angVel[2]);

	//��̬����ѧ�����Һ���
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
/// ������̬
/// @Author	Wang Hua
/// @Date	2010-4-15
/// @Input	
/// @Param	step		���沽��								sec
/// @Param	torque		���ܿ�������(��ϵ)
/// @Param	inertia		ת������
/// @In/Out
/// @Param	quat		��������ϵ��Թ���ϵ����Ԫ��
/// @Param	angVel		��������ϵ�б�ʾ����Թ���ϵ�Ľ��ٶ�
//***********************************************************************
void CAttitude::AttitudeStep(double step,
			const CMatrix<double>& inertia, const CCoord& torque, 
			CQuaternion& quat, CCoord& angVel)
{
	int i;
	CVector<double> x(7);

	//�����Һ����еĲ���
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

	//����RK4���������̬ģ��
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