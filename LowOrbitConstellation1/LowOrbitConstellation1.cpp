// LowOrbitConstellation1.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <iostream>
#include "LowOrbitConstellation1.h"

//***********************************************************************
/// 仿真主程序
/// @Author	Li Pengfei	
/// @Email  3396832223@qq.com
/// @Date	2024-4-18
//***********************************************************************

void CEx01::Main()
{

	//步长
	double step =1;
	double duration = 86400;
	
	//初始化
	Initialization();

	//循环到仿真结束
	while (m_Time<=duration)
	{
		TimeAdvance(step,duration);
		m_Time += step;
	}

	//结果报告生成
	ReportGeneration();

	
	//视场分析
	if (m_Name == "目标卫星")
	{
		Coverage();
	}
}
//***********************************************************************
/// 初始化
/// @Author	Li Pengfei
/// @Date	2024-4-18
//***********************************************************************

void CEx01::Initialization()
{
	m_Time = 0;

	//清空缓存
	m_TimeList.resize(0);
	m_PosList.resize(0);
	m_VelList.resize(0);

}

//***********************************************************************
/// 时间推进
/// @Author	Wang Hua,Li Pengfei
/// @Date	2024-4-18
//***********************************************************************

void CEx01::TimeAdvance(double step,double duration)
{

	COrbit			orb;

	//保存中间数据
	m_TimeList.push_back(m_Time);
	m_PosList.push_back(m_Pos);
	m_VelList.push_back(m_Vel);

	//定步长方法
	orb.OrbitStep(step, m_Pos, m_Vel);
}


//***********************************************************************
/// 星历报告生成
/// @Author	Wang Hua,Li Pengfei
/// @Date	2024-4-18
//***********************************************************************

void CEx01::ReportGeneration()
{
	int i;
	FILE* fp;
	if (m_Name == "卫星1")
	{

		 fopen_s(&fp,".\\Output\\观测卫星1星历.txt", "w");
	}
	else if (m_Name == "卫星2")
	{

		 fopen_s(&fp, ".\\Output\\观测卫星2星历.txt", "w");
	}
	else
	{
		 fopen_s(&fp, ".\\Output\\目标卫星星历.txt", "w");
	}

	for (i = 0; i < m_TimeList.size(); i++)
	{
		fprintf(fp, "%16.3f    ", m_TimeList[i]);
		fprintf(fp, "%15.3f    %15.3f    %15.3f    ",
			m_PosList[i][0],
			m_PosList[i][1],
			m_PosList[i][2]);
		fprintf(fp, "%12.3f    %12.3f    %12.3f    ",
			m_VelList[i][0],
			m_VelList[i][1],
			m_VelList[i][2]);
		fprintf(fp, "\n");
	}
	fclose(fp);

} 

int main()
{
	printf("*****************************************************************\n");
	printf("*                                                               *\n");
	printf("*                     <<低轨星座仿真>>                          *\n");
	printf("*                                                               *\n");
	printf("*                                                               *\n");
	printf("*****************************************************************\n\n");
	CEx01 sim1,sim2,sim3;
	sim1.m_Name = "卫星1";	sim1.m_Pos = CCoord(7378.137000e3, 0, 0);	sim1.m_Vel = CCoord(0, 6.459428e3, 3.507183e3);
	sim2.m_Name = "卫星2";  sim2.m_Pos = CCoord(6933.181884e3, 2523.468755e3, 0); sim2.m_Vel = CCoord(-2.209252e3, 6.069878e3, 3.507183e3);
	sim3.m_Name = "目标卫星";	sim3.m_Pos = CCoord(3769.339212e3, -4772.223026e3, -2759.616475e3); sim3.m_Vel = CCoord(6.369371e3, 3.964980e3, 1.843208e3);

	sim1.Main(); sim2.Main(); sim3.Main();
	return 0;
}

