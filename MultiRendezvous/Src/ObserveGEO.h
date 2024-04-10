
#if !defined(AFX_ASTROLIBEXAMPLES_H__A94E219F_5EC1_4DF2_973D_CFEB16259FBE__INCLUDED_)
#define AFX_ASTROLIBEXAMPLES_H__A94E219F_5EC1_4DF2_973D_CFEB16259FBE__INCLUDED_

#pragma once

#include "AstroLib.h"
#include "Spacecraft.h"
#include "ChCtrl.h"


//
//航天器交会对接总控
//
class CRendezvous
{
public:
    void Main();						    // 仿真主程序

private:
    void Initialization();				    // 初始化
    bool TimeAdvance(double step);		    // 时间推进
    void ReportGeneration();			    // 结果报告生成


public:
    double          m_Time;					// 仿真时间
    int             m_aim = 0;
    CCoord          m_RelPos;               // 相对速度
    CCoord          m_RelVel;               // 相对速度

public:
    //保存仿真中间数据
    struct CHistoryData
    {
        std::vector<double>	m_TimeList;		// 仿真总共消耗时间[s]
        std::vector<CCoord> m_chList;		// 相对位置状态[m]
        std::vector<CCoord> m_tg1List;		// 相对位置状态[m]
        std::vector<CCoord> m_tg2List;		// 相对位置状态[m]

        std::vector<CCoord> m_ch_vList;		// 相对速度状态[m/s]
        std::vector<CCoord> m_tg1_vList;	// 相对速度状态[m/s]
        std::vector<CCoord> m_tg2_vList;	// 相对速度状态[m/s]

        std::vector<CCoord> m_sun0List;		// 阳光角[弧度]
        std::vector<CCoord> m_sun1List;		// 阳光角[弧度]
        std::vector<CCoord> m_sun2List;		// 阳光角[弧度]
    };
    CHistoryData	m_HistoryData;	    // 历史数据

private:
    CChCtrl         m_ChCtrl;               // 追踪器控制
    CSpacecraft     m_ChDyn;                // 追踪器动力学
    std::vector <CSpacecraft>     m_TgDyn;  // 目标器动力学

};



#endif // !defined(AFX_ASTROLIBEXAMPLES_H__A94E219F_5EC1_4DF2_973D_CFEB16259FBE__INCLUDED_)
