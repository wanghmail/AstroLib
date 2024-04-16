// Spacecraft.cpp: implementation of the CMulSpacecraft class.
//
//////////////////////////////////////////////////////////////////////

#include "MulSpacecraft.h"

using namespace std;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMulSpacecraft::CMulSpacecraft()
{
    // ��ʼ������
    m_Name = "Vehicle";
    m_ElapsedSec = 0.0;
    m_Pos[0] = 0.0;
    m_Pos[1] = 0.0;
    m_Pos[2] = 0.0;
}


CMulSpacecraft::~CMulSpacecraft()
{

}


//********************************************************************
//��ʼ��,״̬�ļ�,��ʼ������
//Author:	Wang Hua
//Date:		2005.10.22
//Input:	fileName	Ҫ����ĺ����������ļ�����
//********************************************************************
void CMulSpacecraft::Init()
{
    //��������ֵ
    m_ElapsedSec = 0.0;		//Epoch second set to zero.

    //�����ʷ����
    m_HistoryData.m_ElapsedSec.resize(0);
    m_HistoryData.m_Pos.resize(0);
    m_HistoryData.m_Vel.resize(0);
}


//********************************************************************
/// ����ʱ��step���λ��,������,������̬
/// @Author	Wang Hua
/// @Date	2004.11.19
/// @Input
/// @Param	step			ʱ�䲽��
/// @Param	burnCoordSys	��������ϵ: 0=J2000;1=VVLH;2=VNC;3=������ϵ
/// @Param	burnValue		���������ܵĳ���
/// @Output	
/// @Return									
//********************************************************************
void CMulSpacecraft::TimeAdvance(double step, int burnCoordSys,
    const CCoord& burnValue)
{
    CCoord			burnValueInertial, burnValueBody, momentVCF, sunPos;
    CMatrix<double> mtx;

    // �ٶ�����
    switch (burnCoordSys)
    {
    case 0:
        m_Vel += burnValue;
        break;
    case 1:
        AsVVLHToICSMtx(m_Pos, m_Vel, mtx);
        m_Vel += burnValue.RotateTo(mtx);
        break;
    }

    // �������
    m_Orbit.OrbitStep(step, m_Pos, m_Vel);

    // ����ʱ��
    m_ElapsedSec += step;

    UpdateBuffer();
}


//********************************************************************
//�������ݻ�����
//Author:	Wang Hua
//Date:		2005.10.22
//********************************************************************
void CMulSpacecraft::UpdateBuffer()
{
    m_HistoryData.m_ElapsedSec.push_back(m_ElapsedSec);
    m_HistoryData.m_Pos.push_back(m_Pos);
    m_HistoryData.m_Vel.push_back(m_Vel);
}


//***********************************************************************
/// �����������
/// @Author	Wang Hua
/// @Date	2022-6-30
//***********************************************************************
void CMulSpacecraft::ReportGeneration()
{
    ofstream oss(".\\Output\\" + m_Name + ".txt");
    int n = m_HistoryData.m_ElapsedSec.size();
    for (int i = 0; i < n; i++)
    {
        oss << m_HistoryData.m_ElapsedSec[i] << "	"
            << m_HistoryData.m_Pos[i][0] << "	"
            << m_HistoryData.m_Pos[i][1] << "	"
            << m_HistoryData.m_Pos[i][2] << "	"
            << m_HistoryData.m_Vel[i][0] << "	"
            << m_HistoryData.m_Vel[i][1] << "	"
            << m_HistoryData.m_Vel[i][2] << endl;
    }
    oss.close();
}