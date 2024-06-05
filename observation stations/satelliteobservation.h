
#if !defined(_satelliteobservation_H__INCLUDED_)
#define _satelliteobservation_H__INCLUDED_

#pragma once


#include "AstroLib.h"
#include "satelliteParameter.h"
// 常量声明
#define pi 3.141592653589793

//
//卫星观测时段
//
class Csatobs
{
public:
	void Main();						    // 仿真主程序
 

private:
	void Initialization();				    // 初始化
	bool TimeAdvance(double step);		    // 时间推进
	void ReportGeneration();			    // 结果报告生成


public:
	double m_Time;					// 仿真时间
	double Time0[6];//初始时间
	double Vel[3]; //卫星地心惯性系速度
	double Pos[3]; //卫星地心惯性式位置
	double theta;  //地面可观测卫星的最小仰角
	double lla[3]; //地面站经纬高数组
	double step;
	vector<vector<double>> Window_all; // 窗口时间
	vector<vector<string>> All_Timeperiod[2]; 
	vector<int> obs;
	vector<double> Duration;

	CCoord     m_Pos;               // 卫星地心惯性系位置
    CCoord     m_Vel;               // 卫星地心惯性系速度

public:
	//保存仿真中间数据
	std::vector<double>	m_TimeList;
	std::vector<CCoord> m_PosList;
	std::vector<CCoord> m_VelList;
  
};



#endif // _satelliteobservation_H__INCLUDED_
