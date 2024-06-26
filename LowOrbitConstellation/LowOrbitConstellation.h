﻿
#if !defined(_CEx01_H__INCLUDED_)
#define _CEx01_H__INCLUDED_

#pragma once


#include "AstroLib.h"


class CEx01
{
public:
	void Main();						///< 仿真主程序

private:
	void Initialization();				///< 初始化
	void TimeAdvance(double step,double duration);		///< 时间推进
	void ReportGeneration();			///< 星历报告生成
	void Coverage();						///< 视场分析
	void Readxingli(int i, vector<double>& time, vector<double>& pos);	///< 读取星历

public:
	//保存仿真中间数据
	std::vector<double>	m_TimeList;
	std::vector<CCoord> m_PosList; 
	std::vector<CCoord> m_VelList; 


public:
	double      m_Time;					///< 飞行器时间
	CCoord      m_Pos;					///< 飞行器位置
	CCoord      m_Vel;					///< 飞行器速度
	string      m_Name;					///< 飞行器名称
};


#endif // !defined(_CEx01_H__INCLUDED_)
