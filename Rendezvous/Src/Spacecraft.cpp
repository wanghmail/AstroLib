// Spacecraft.cpp: implementation of the CSpacecraft class.
//
//////////////////////////////////////////////////////////////////////

#include "Spacecraft.h"

using namespace std;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpacecraft::CSpacecraft()
{
	// 初始化数据
	m_Name				 = "Vehicle";
	m_ElapsedSec         = 0.0;
	m_Pos[0] = 0.0;	
	m_Pos[1] = 0.0;	
	m_Pos[2] = 0.0;	
}  


CSpacecraft::~CSpacecraft()
{

}


//********************************************************************
//初始化,状态文件,初始化对象
//Author:	Wang Hua
//Date:		2005.10.22
//Input:	fileName	要载入的航天器数据文件名称
//********************************************************************
void CSpacecraft::Init()
{
    //变量赋初值
    m_ElapsedSec    = 0.0;		//Epoch second set to zero.

    //清空历史数据
    m_HistoryData.m_ElapsedSec.resize(0);
    m_HistoryData.m_Pos.resize(0);
    m_HistoryData.m_Vel.resize(0);
}


//********************************************************************
/// 计算时间step后的位置,先算轨道,再算姿态
/// @Author	Wang Hua
/// @Date	2004.11.19
/// @Input
/// @Param	step			时间步长
/// @Param	burnCoordSys	机动坐标系: 0=J2000;1=VVLH;2=VNC;3=体坐标系
/// @Param	burnValue		航天器所受的冲量
/// @Output	
/// @Return									
//********************************************************************
void CSpacecraft::TimeAdvance(double step, int burnCoordSys,
    const CCoord& burnValue)
{
    CCoord			burnValueInertial, burnValueBody, momentVCF, sunPos;
    CMatrix<double> mtx;

    // 速度增量
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

    // 轨道计算
    m_Orbit.OrbitStep(step, m_Pos, m_Vel);

    // 递推时间
    m_ElapsedSec += step;

    UpdateBuffer();
}


//********************************************************************
//更新数据缓冲区
//Author:	Wang Hua
//Date:		2005.10.22
//********************************************************************
void CSpacecraft::UpdateBuffer()
{
    m_HistoryData.m_ElapsedSec.push_back(m_ElapsedSec);
    m_HistoryData.m_Pos.push_back(m_Pos);
    m_HistoryData.m_Vel.push_back(m_Vel);
}


//***********************************************************************
/// 结果报告生成
/// @Author	Wang Hua
/// @Date	2022-6-30
//***********************************************************************
void CSpacecraft::ReportGeneration()
{
    ofstream oss(".\\Output\\"+m_Name+".txt");
    int n = m_HistoryData.m_ElapsedSec.size();
    for (int i=0; i<n; i++)
    {
        oss <<m_HistoryData.m_ElapsedSec[i]<<"	"
            <<m_HistoryData.m_Pos[i][0]<<"	"
			<<m_HistoryData.m_Pos[i][1]<<"	"
			<<m_HistoryData.m_Pos[i][2]<<"	"
            <<m_HistoryData.m_Vel[i][0]<<"	"
			<<m_HistoryData.m_Vel[i][1]<<"	"
			<<m_HistoryData.m_Vel[i][2]<<endl;
    }
    oss.close();
}