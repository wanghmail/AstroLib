// Sim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Sim.h"
#include "DoF6.h"
#include <iostream>
#include <fstream>


using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//***********************************************************************
/// ����������
/// @Author	Wang Hua
/// @Date	2010-4-15
//***********************************************************************
void CSim::Main()
{
	//����
	double step = 0.01;

	//��ʼ��
	Initialization();

	//ѭ����ʱ�����
	while (m_Time<10.0)
	{
		TimeAdvance(step);
		m_Time += step;
	}

	//�����������
	ReportGeneration();

}


//***********************************************************************
/// ��ʼ��
/// @Author	Wang Hua
/// @Date	2010-4-15
//***********************************************************************
void CSim::Initialization()		
{
	//��ʼ��ʱ��
	m_Time = 0;

	//��ʼ��λ���ٶ�
	m_Pos[0] = 0;
	m_Pos[1] = 0;
	m_Pos[2] = 0;
	m_Vel[0] = 0;
	m_Vel[1] = 0;
	m_Vel[2] = 0;

	//��ʼ����Ԫ��
	CEuler euler(0,0,0);
	AsEulerToQuat(euler, 321, m_Quat);

	//��ʼ�����ٶ�
	m_AngVel[0] = 0;
	m_AngVel[1] = 0;
	m_AngVel[2] = 0;

	//��ʼ��������ת������
	m_Mass = 1;
	m_Inertia.MakeUnit();

	//��ջ���
	m_TimeList.resize(0);
	m_PosList.resize(0);
	m_VelList.resize(0);
	m_QuatList.resize(0);
	m_AngVelList.resize(0);
}


//***********************************************************************
/// ʱ���ƽ�
/// @Author	Wang Hua
/// @Date	2010-4-15
//***********************************************************************
void CSim::TimeAdvance(double step)
{
	//����
	double mxList[2][6]={
			      0,       1,      1.1,             2, 2.1, 10,
			3.144/2, 3.144/2, -3.144/2, -.998*3.144/2,   0,  0};
	double myList[2][14]={
			0, 3,     3.1,       4,      4.1,              5, 5.1, 7.5,      7.6,      8.5,     8.6,     9.5, 9.6, 10,
			0, 0, 3.138/2, 3.138/2, -3.138/2, -1.005*3.138/2,   0,   0, -3.138/2, -3.138/2, 3.138/2, 3.138/2,   0,  0};
	double mzList[2][8]={
			0, 6,    6.1,      7,   7.1,     8, 8.1, 10,
			0, 0, -3.1/2, -3.1/2, 3.1/2, 3.1/2,   0,  0};

	int i;
	CCoord force(1,0,0), torque;

	//��ֵ�õ���ǰʱ�������ڷ������ϵ�����
	for (i=0; i<5; i++)
	{
		if (m_Time<mxList[0][i])
			break;
	}
	AsInterp1Ord (mxList[0][i-1], mxList[0][i], mxList[1][i-1], mxList[1][i], m_Time, torque[0]);
	for (i=0; i<13; i++)
	{
		if (m_Time<myList[0][i])
			break;
	}
	AsInterp1Ord (myList[0][i-1], myList[0][i], myList[1][i-1], myList[1][i], m_Time, torque[1]);
	for (i=0; i<7; i++)
	{
		if (m_Time<mzList[0][i])
			break;
	}
	AsInterp1Ord (mzList[0][i-1], mzList[0][i], mzList[1][i-1], mzList[1][i], m_Time, torque[2]);

	CDoF6 dof6;
	dof6.DoF6(step, m_Mass, m_Inertia, force, torque, m_Pos, m_Vel, m_Quat, m_AngVel);

	//
	//�����м�����
	//
	m_TimeList.push_back(m_Time);
	m_PosList.push_back(m_Pos);
	m_VelList.push_back(m_Vel);
	m_QuatList.push_back(m_Quat);
	m_AngVelList.push_back(m_AngVel);
}			


//***********************************************************************
/// �����������
/// @Author	Wang Hua
/// @Date	2010-4-15
//***********************************************************************
void CSim::ReportGeneration()
{
	int i;

	ofstream oss(".\\Output\\Result.txt");
	int n = m_TimeList.size();
	for (i=0; i<n; i++)
	{
		oss <<m_TimeList[i]<<"	"
			<<m_PosList[i][0]<<"	"<<m_PosList[i][1]<<"	"<<m_PosList[i][2]<<"	"
			<<m_VelList[i][0]<<"	"<<m_VelList[i][1]<<"	"<<m_VelList[i][2]<<"	"
			<<m_QuatList[i].m_Qs<<"	"<<m_QuatList[i].m_Qx<<"	"
			<<m_QuatList[i].m_Qy<<"	"<<m_QuatList[i].m_Qz<<"	"
			<<m_AngVelList[i][0]<<"	"<<m_AngVelList[i][1]<<"	"<<m_AngVelList[i][2]<<endl;
	}
	oss.close();

	cout<<"Position = "<<m_Pos[0]<<", "<<m_Pos[1]<<", "<<m_Pos[2]<<endl;
	cout<<"Velocity = "<<m_Vel[0]<<", "<<m_Vel[1]<<", "<<m_Vel[2]<<endl;

}



/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	printf("*****************************************************************\n");
	printf("*                                                               *\n");
	printf("*          <<Aerospace Vehicle System Simulation>>              *\n");         
	printf("*          Six Degrees of Freedom Motion Simulation             *\n");
	printf("*                                                               *\n");
	printf("*****************************************************************\n\n");

	//ִ�з����������ɶȷ���
	CSim sim;
	sim.Main();

	return nRetCode;
}


