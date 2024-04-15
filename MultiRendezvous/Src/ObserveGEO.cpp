// ObserveGEO.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <iostream>
#include "ObserveGEO.h"
using namespace std;

//***********************************************************************
/// 仿真主程序
/// @Author	Wang Hua
/// @Date	2010-4-15
//***********************************************************************
void CMulRendezvous::Main()
{

    bool isEnd = false;

    //步长
    double step = 5;

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
/// 初始化
/// @Author	Wang Hua
/// @Date	2010-4-15
//***********************************************************************
void CMulRendezvous::Initialization()
{
    //初始化时间
    m_Time = 0;

    //初始化追踪器和目标器
    CCoord chPos(-1.219064685855942e+07, -4.036403817972275e+07, 4.757418138915581e+04), chVel(2.943490270326994e+03, -8.890749827986162e+02, -13.681119407780425);
    CCoord tgPos, tgVel;
    AsVVLHRelStateToState(CCoord(100000, 0, 0), CCoord(0, 0, 0),
        chPos, chVel, tgPos, tgVel);
    m_ChDyn.SetName("Chase");
    m_ChDyn.SetPos(chPos);
    m_ChDyn.SetVel(chVel);
    m_TgDyn.resize(3);
    m_TgDyn.at(0).SetName("Target0");
    m_TgDyn.at(0).SetPos(tgPos);
    m_TgDyn.at(0).SetVel(tgVel);
    AsVVLHRelStateToState(CCoord(200000, 0, 0), CCoord(0, 0, 0),
        chPos, chVel, tgPos, tgVel);
    m_TgDyn.at(1).SetName("Target1");
    m_TgDyn.at(1).SetPos(tgPos);
    m_TgDyn.at(1).SetVel(tgVel);
    AsVVLHRelStateToState(CCoord(400000, 0, 0), CCoord(0, 0, 0),
        chPos, chVel, tgPos, tgVel);
    m_TgDyn.at(2).SetName("Target2");
    m_TgDyn.at(2).SetPos(tgPos);
    m_TgDyn.at(2).SetVel(tgVel);

    //初始化控制误差
    m_ChCtrl.m_UseMeasureRandom = 0;
    m_ChCtrl.m_UseEngineRandom = 0;
    m_ChCtrl.m_PosStdDev = 0.1;
    m_ChCtrl.m_VelStdDev = 0.002;
    m_ChCtrl.m_EngStdDev = 0.002;

    //初始化动力学和控制
    m_TgDyn.at(0).Init();
    m_TgDyn.at(1).Init();
    m_TgDyn.at(2).Init();
    m_ChDyn.Init();
    m_ChCtrl.Init();

    //清空缓存
    m_HistoryData.m_TimeList.resize(0);
    m_HistoryData.m_chList.resize(0);
    m_HistoryData.m_ch_vList.resize(0);
    //m_HistoryData.m_tg0List.resize(0);
    //m_HistoryData.m_tg0_vList.resize(0);
    m_HistoryData.m_tg1List.resize(0);
    m_HistoryData.m_tg1_vList.resize(0);
    m_HistoryData.m_tg2List.resize(0);
    m_HistoryData.m_tg2_vList.resize(0);
    m_HistoryData.m_aimList.resize(0);
}


//***********************************************************************
/// 时间推进
/// @Author	Wang Hua
/// @Date	2010-4-15
/// @Return	true=仿真结束
//***********************************************************************
bool CMulRendezvous::TimeAdvance(double step)
{
    bool    isEnd = false;
    CCoord  impulse;
    vector<double> t_list(3);
    t_list[0] = 20000; t_list[1] = 20000; t_list[2] = 40000;//访问时间

    //追踪器和目标器推进
    m_ChCtrl.TimeAdvance(step, m_ChDyn.GetPos(), m_ChDyn.GetVel(), m_TgDyn.at(m_aim).GetPos(), m_TgDyn.at(m_aim).GetVel(),
        t_list[m_aim], CCoord(0, 0, 1000), isEnd, impulse, m_aim);
    m_ChDyn.TimeAdvance(step, 1, impulse);
    m_TgDyn.at(0).TimeAdvance(step, 0, CCoord(0, 0, 0));
    m_TgDyn.at(1).TimeAdvance(step, 0, CCoord(0, 0, 0));
    m_TgDyn.at(2).TimeAdvance(step, 0, CCoord(0, 0, 0));

    //保存中间数据
    
    m_HistoryData.m_TimeList.push_back(m_Time);
    
    AsStateToVVLHRelState(m_ChDyn.GetPos(), m_ChDyn.GetVel(),m_TgDyn.at(0).GetPos(), m_TgDyn.at(0).GetVel(), 
        m_RelPos, m_RelVel);
    m_HistoryData.m_chList.push_back(m_RelPos);
    m_HistoryData.m_ch_vList.push_back(m_RelVel);
    int aim_flag = -1;
    if (m_aim == 0)
    {
        if (m_RelPos.Norm() < 5000)
        {
            cout << m_Time << "秒，可以看见目标1"<< endl;
            m_HistoryData.m_aimList.push_back(1);
            aim_flag = 1;
        }
    }

    
    AsStateToVVLHRelState(m_TgDyn.at(1).GetPos(), m_TgDyn.at(1).GetVel(),m_TgDyn.at(0).GetPos(), m_TgDyn.at(0).GetVel(), 
        m_RelPos, m_RelVel);
    m_HistoryData.m_tg1List.push_back(m_RelPos);
    m_HistoryData.m_tg1_vList.push_back(m_RelVel);
    if (m_aim == 1)
    {
        if ((m_TgDyn.at(1).GetPos() - m_ChDyn.GetPos()).Norm() < 5000)
        {
            cout << m_Time << "秒，可以看见目标2" << endl;
            m_HistoryData.m_aimList.push_back(2);
            aim_flag = 1;
        }
    }

    AsStateToVVLHRelState(m_TgDyn.at(2).GetPos(), m_TgDyn.at(2).GetVel(),m_TgDyn.at(0).GetPos(), m_TgDyn.at(0).GetVel(), 
        m_RelPos, m_RelVel);
    m_HistoryData.m_tg2List.push_back(m_RelPos);
    m_HistoryData.m_tg2_vList.push_back(m_RelVel);
    if (m_aim == 2)
    {
        if ((m_TgDyn.at(2).GetPos() - m_ChDyn.GetPos()).Norm() < 5000)
        {
            cout << m_Time << "秒，可以看见目标3" << endl;
            m_HistoryData.m_aimList.push_back(3);
            aim_flag = 1;
        }
    }
    if (aim_flag < 0)
    {
        m_HistoryData.m_aimList.push_back(0);
    }
    

    if (isEnd == true)
    {
        m_aim += 1;
        isEnd = false;
    }
    if (m_aim > 2)
    {
        isEnd = true;
    }
    return isEnd;
}


//***********************************************************************
/// 结果报告生成
/// @Author	Wang Hua
/// @Date	2010-4-15
//***********************************************************************
void CMulRendezvous::ReportGeneration()
{
    ofstream oss(".\\Output\\RelPosVel.txt");
    int n = m_HistoryData.m_TimeList.size();
    for (int i = 0; i < n; i++)
    {
        oss << m_HistoryData.m_TimeList[i] << "	"
            << m_HistoryData.m_chList[i][0] << "	"
            << m_HistoryData.m_chList[i][1] << "	"
            << m_HistoryData.m_chList[i][2] << "	"
            << m_HistoryData.m_tg1List[i][0] << "	"
            << m_HistoryData.m_tg1List[i][1] << "	"
            << m_HistoryData.m_tg1List[i][2] << "	"
            << m_HistoryData.m_tg2List[i][0] << "	"
            << m_HistoryData.m_tg2List[i][1] << "	"
            << m_HistoryData.m_tg2List[i][2] << "	"
            << m_HistoryData.m_aimList[i] << endl;
    }
    oss.close();

    m_ChDyn.ReportGeneration();
    m_TgDyn[0].ReportGeneration();

}


int main()
{
    printf("*****************************************************************\n");
    printf("* ObserveGEO Example\n");
    printf("*****************************************************************\n\n");


    //执行交会对接仿真
    CMulRendezvous sim;
    sim.Main();           //标称仿真
    //sim.MainMonteCarlo();   //MonteCarlo仿真

    return 0;
}




