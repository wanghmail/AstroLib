// Rendezvous.cpp : 定义控制台应用程序的入口点。
//

#include "Rendezvous.h"
#include <fstream>

using namespace std;


//***********************************************************************
/// 仿真主程序
/// @Author	Wang Hua
/// @Date	2010-4-15
//***********************************************************************
void CRendezvous::Main()
{
    bool isEnd=false;

    //步长
    double step = 0.2;

    //初始化
    Initialization();

    //循环到仿真结束
    while (!isEnd)
    {
        isEnd = TimeAdvance(step);
        m_Time += step;
    }

    //结果报告生成
    ReportGeneration();

}


//***********************************************************************
/// MonteCarlo仿真主程序
/// @Author	Wang Hua
/// @Date	2010-4-15
//***********************************************************************
void CRendezvous::MainMonteCarlo()
{
    ofstream oss(".\\Output\\MonteCarlo.txt");
    for (int i=0; i<500; i++)
    {
        bool isEnd=false;

        //步长
        double step = 0.2;

        //初始化
        Initialization();

        //偏差仿真
        m_ChCtrl.m_UseMeasureRandom = 1;
        m_ChCtrl.m_UseEngineRandom = 1;

        //循环到仿真结束
        while (!isEnd)
        {
            isEnd = TimeAdvance(step);
            m_Time += step;
        }

        //结果报告生成
        ReportGeneration();

        oss <<m_HistoryData.m_RelPosList.back()[0]<<"	"
            <<m_HistoryData.m_RelPosList.back()[1]<<"	"
            <<m_HistoryData.m_RelPosList.back()[2]<<endl;
        cout<<"第"<<i<<"次仿真结束"<<endl;
    }

    oss.close();

}


//***********************************************************************
/// 初始化
/// @Author	Wang Hua
/// @Date	2010-4-15
//***********************************************************************
void CRendezvous::Initialization()
{
    //初始化时间
    m_Time = 0;

    //初始化追踪器和目标器
    CCoord tgPos(6758137,0,0), tgVel(0, 5707.28, 5138.85);
    CCoord chPos, chVel;
    AsVVLHRelStateToState(CCoord(-3000, 0, 0), CCoord(0, 0, 0),
        tgPos, tgVel, chPos, chVel);
    m_ChDyn.SetName("Chase");
    m_ChDyn.SetPos(chPos);
    m_ChDyn.SetVel(chVel);
    m_TgDyn.SetName("Target");
    m_TgDyn.SetPos(tgPos);
    m_TgDyn.SetVel(tgVel);

    //初始化控制误差
    m_ChCtrl.m_UseMeasureRandom = 0;
    m_ChCtrl.m_UseEngineRandom = 0;
    m_ChCtrl.m_PosStdDev = 0.1;
    m_ChCtrl.m_VelStdDev = 0.002;
    m_ChCtrl.m_EngStdDev = 0.002;
    
    //初始化动力学和控制
    m_TgDyn.Init();
    m_ChDyn.Init();
    m_ChCtrl.Init();

    //清空缓存
    m_HistoryData.m_TimeList.resize(0);
    m_HistoryData.m_RelPosList.resize(0);
    m_HistoryData.m_RelVelList.resize(0);
}


//***********************************************************************
/// 时间推进
/// @Author	Wang Hua
/// @Date	2010-4-15
/// @Return	true=仿真结束
//***********************************************************************
bool CRendezvous::TimeAdvance(double step)
{
    bool    isEnd=false;
    CCoord  impulse;
   
    //追踪器和目标器推进
    m_ChCtrl.TimeAdvance(step, m_ChDyn.GetPos(), m_ChDyn.GetVel(), m_TgDyn.GetPos(), m_TgDyn.GetVel(), 
        2300, CCoord(-400, 0, 0), isEnd, impulse);
    m_ChDyn.TimeAdvance(step, 1, impulse);
    m_TgDyn.TimeAdvance(step, 0, CCoord(0, 0, 0));

    //保存中间数据
    AsStateToVVLHRelState(m_ChDyn.GetPos(), m_ChDyn.GetVel(), m_TgDyn.GetPos(), m_TgDyn.GetVel(),
        m_RelPos, m_RelVel);
    m_HistoryData.m_TimeList.push_back(m_Time);
    m_HistoryData.m_RelPosList.push_back(m_RelPos);
    m_HistoryData.m_RelVelList.push_back(m_RelVel);

    return isEnd;
}


//***********************************************************************
/// 结果报告生成
/// @Author	Wang Hua
/// @Date	2010-4-15
//***********************************************************************
void CRendezvous::ReportGeneration()
{
    ofstream oss(".\\Output\\RelPosVel.txt");
    int n = m_HistoryData.m_TimeList.size();
    for (int i=0; i<n; i++)
    {
        oss <<m_HistoryData.m_TimeList[i]<<"	"
            <<m_HistoryData.m_RelPosList[i][0]<<"	"
			<<m_HistoryData.m_RelPosList[i][1]<<"	"
			<<m_HistoryData.m_RelPosList[i][2]<<"	"
            <<m_HistoryData.m_RelVelList[i][0]<<"	"
			<<m_HistoryData.m_RelVelList[i][1]<<"	"
			<<m_HistoryData.m_RelVelList[i][2]<<endl;
    }
    oss.close();

    //m_ChDyn.ReportGeneration();
    //m_TgDyn.ReportGeneration();

}


int main()
{
    printf("*****************************************************************\n");
    printf("*                                                               *\n");
    printf("*          <<Aerospace Vehicle System Simulation>>              *\n");
    printf("*                   Rendezvous Simulation                       *\n");
    printf("*                                                               *\n");
    printf("*****************************************************************\n\n");

    //执行交会对接仿真
    CRendezvous sim;
    //sim.Main();           //标称仿真
    sim.MainMonteCarlo();   //MonteCarlo仿真
    
    return 0;
}

