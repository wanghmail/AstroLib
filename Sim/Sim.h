
#if !defined(AFX_SIM_H__B0B3B510_F9D9_4417_84A3_59D66930CEE9__INCLUDED_)
#define AFX_SIM_H__B0B3B510_F9D9_4417_84A3_59D66930CEE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "resource.h"
#include "AstroLib.h"


//
//�����������ɶȷ���ʵ��
//
class CSim
{
public:
	void Main();						///< ����������

private:
	void Initialization();				///< ��ʼ��
	void TimeAdvance(double step);		///< ʱ���ƽ�
	void ReportGeneration();			///< �����������


public:
	//��������м�����
	std::vector<double>	m_TimeList;
	std::vector<CCoord> m_PosList;
	std::vector<CCoord> m_VelList;
	std::vector<CQuaternion> m_QuatList;
	std::vector<CCoord> m_AngVelList;


public:
	double m_Time;						///< ������ʱ��
	CCoord3 m_Pos;						///< ������λ��
	CCoord3 m_Vel;						///< �������ٶ�
	CQuaternion m_Quat;					///< ��������Ԫ��
	CCoord m_AngVel;					///< ���������ٶ�

	double m_Mass;						///< ����������
	CMatrix<double> m_Inertia;			///< ������ת������
};



#endif // !defined(AFX_SIM_H__B0B3B510_F9D9_4417_84A3_59D66930CEE9__INCLUDED_)
