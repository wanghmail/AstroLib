
#if !defined(AFX_SIM_H__B0B3B510_F9D9_4417_84A3_59D66930CEE9__INCLUDED_)
#define AFX_SIM_H__B0B3B510_F9D9_4417_84A3_59D66930CEE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "resource.h"
#include "AstroLib.h"


//
//飞行器六自由度仿真实现
//
class CSim
{
public:
	void Main();						///< 仿真主程序

private:
	void Initialization();				///< 初始化
	void TimeAdvance(double step);		///< 时间推进
	void ReportGeneration();			///< 结果报告生成


public:
	//保存仿真中间数据
	std::vector<double>	m_TimeList;
	std::vector<CCoord> m_PosList;
	std::vector<CCoord> m_VelList;
	std::vector<CQuaternion> m_QuatList;
	std::vector<CCoord> m_AngVelList;


public:
	double m_Time;						///< 飞行器时间
	CCoord3 m_Pos;						///< 飞行器位置
	CCoord3 m_Vel;						///< 飞行器速度
	CQuaternion m_Quat;					///< 飞行器四元数
	CCoord m_AngVel;					///< 飞行器角速度

	double m_Mass;						///< 飞行器质量
	CMatrix<double> m_Inertia;			///< 飞行器转动惯量
};



#endif // !defined(AFX_SIM_H__B0B3B510_F9D9_4417_84A3_59D66930CEE9__INCLUDED_)
