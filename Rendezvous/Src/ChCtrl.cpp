// ChCtrl.cpp: implementation of the CChCtrl class.
//
//////////////////////////////////////////////////////////////////////


#include "ChCtrl.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChCtrl::CChCtrl()
{
    m_UseMeasureRandom = 0;
    m_UseEngineRandom = 0;
    m_PosStdDev = 0;
    m_VelStdDev = 0;
    m_FlyTime  = 0;
    m_PulseNum = 0;
}

CChCtrl::~CChCtrl()
{

}


//*********************************************************************
/// 初始化
/// @author	Wang Hua
/// @Date	2005.11.16
//*********************************************************************
void CChCtrl::Init()
{
	m_FlyTime  = 0;
	m_PulseNum = 0;

    //随机数初始化
    int seed = AsRandomSeed();
    for (int i=0; i<3; i++)
    {
        m_RandomPos[i].Init(seed-i*3-1);
        m_RandomVel[i].Init(seed-i*3-2);
        m_RandomEng[i].Init(seed-i*3-3);
    }
}


//********************************************************************
/// 测量计算，包含测量误差
/// @author	Wang Hua
/// @Date	2022-6-29
/// @Input
/// @Param	chPos			追踪位置
/// @Param	chVel			追踪速度
/// @Param	tgPos			目标位置
/// @Param	tgVel			目标速度
/// @Output
/// @Param	relPos			相对位置
/// @Param	relVel			相对速度
//********************************************************************
void CChCtrl::Measure(const CCoord& chPos, const CCoord& chVel,
            const CCoord& tgPos, const CCoord& tgVel,
            CCoord& relPos, CCoord& relVel)
{
    //计算相对位置速度
    AsStateToVVLHRelState(chPos, chVel, tgPos, tgVel, relPos, relVel);

    //增加测量偏差
    if (m_UseMeasureRandom==0)
    {
        return;
    }
    else if (m_UseMeasureRandom==1)
    {
        relPos[0] += m_RandomPos[0].Gauss()*m_PosStdDev;
        relPos[1] += m_RandomPos[1].Gauss()*m_PosStdDev;
        relPos[2] += m_RandomPos[2].Gauss()*m_PosStdDev;
        relVel[0] += m_RandomVel[0].Gauss()*m_VelStdDev;
        relVel[1] += m_RandomVel[1].Gauss()*m_VelStdDev;
        relVel[2] += m_RandomVel[2].Gauss()*m_VelStdDev;
    }

}


//********************************************************************
/// 计算执行结构执行后的脉冲
/// @author	Wang Hua
/// @Date	2022-6-29
/// @In/Out
/// @Param	impulse			相对位置
//********************************************************************
void CChCtrl::Engine(CCoord& impulse)
{
    //增加测量偏差
    if (m_UseEngineRandom==0)
    {
        return;
    }
    else if (m_UseEngineRandom==1)
    {
        impulse[0] += m_RandomEng[0].Gauss()*m_EngStdDev;
        impulse[1] += m_RandomEng[1].Gauss()*m_EngStdDev;
        impulse[2] += m_RandomEng[2].Gauss()*m_EngStdDev;
    }

}


//********************************************************************
/// 根据CW方程外推计算下一步需要施加的冲量
/// @Author	Wang Hua
/// @Date	2005.11.17
/// @Input
/// @Param	chPos			追踪位置(m)
/// @Param	chVel			追踪速度(m/s)
/// @Param	tgPos			目标位置(m)
/// @Param	tgVel			目标速度(m/s)
/// @Param	endRelPos		制导终点所需相对位置(m)
/// @Param	flyTime			制导飞行时间
/// @Output
/// @Param	impulse			计算得到的冲量
//********************************************************************
void CChCtrl::GuidanceImpulse1(
			const CCoord& chPos, const CCoord& chVel, 
            const CCoord& tgPos, const CCoord& tgVel, 
			const CCoord& endRelPos, 
			double flyTime, CCoord& impulse)
{
    double orbAngVel;
	CCoord impulse2;
    CCoord OrbAngVelVec;
    CCoord relPos, relVel;

    Measure(chPos, chVel, tgPos, tgVel, relPos, relVel);

    AsCartStateToOrbAngVel(tgPos, tgVel, OrbAngVelVec);
    orbAngVel = OrbAngVelVec.Norm();
	AsCWImpulse(relPos, relVel, endRelPos,
		CCoord(0,0,0), flyTime, orbAngVel, impulse, impulse2);

    Engine(impulse);
}


//********************************************************************
//根据CW方程外推计算下一步需要施加的冲量，准备开始调姿
/// @Author	Wang Hua
/// @Date	2005.11.17
/// @Input
/// @Param	chPos			追踪位置(m)
/// @Param	chVel			追踪速度(m/s)
/// @Param	tgPos			目标位置(m)
/// @Param	tgVel			目标速度(m/s)
/// @Param	endRelPos		制导终点所需相对速度(m/s)
/// @Output
/// @Param	impulse			计算得到的冲量
//********************************************************************
void CChCtrl::GuidanceImpulse2(const CCoord& chPos, const CCoord& chVel, 
            const CCoord& tgPos, const CCoord& tgVel,
			const CCoord& endRelVel, CCoord& impulse)
{
    CCoord relPos, relVel;

    Measure(chPos, chVel, tgPos, tgVel, relPos, relVel);    
	impulse = endRelVel-relVel;
}


//*********************************************************************
/// 计算控制,直接加冲量
/// @author	Wang Hua
/// @Date	2006.8.12
/// @Param	step			时间步长
/// @Param	chPos			惯性系中的追踪器位置(m)
/// @Param	chVel			惯性系中的追踪器速度(m/s)
/// @Param	tgPos			惯性系中的目标器位置(m)
/// @Param	tgVel			惯性系中的目标器速度(m/s)
/// @Param	flyTimeApproach	接近段飞行时间
/// @Output
/// @Param	isEnd			是否本阶段结束
/// @Param	impulse			VVLH系中的脉冲
/// @Return	
//*********************************************************************
void CChCtrl::TimeAdvance(double step,
			const CCoord& chPos, const CCoord& chVel, 
			const CCoord& tgPos, const CCoord& tgVel, 
			double flyTimeApproach, const CCoord& endPos,
			bool& isEnd, CCoord& impulse)
{
	isEnd = false;
	impulse.SetDataAll(0);

	//至终端
	if (m_PulseNum==0)
	{
		m_PulseNum = 1;
		m_FlyTime  = 0;

        GuidanceImpulse1(chPos, chVel, tgPos, tgVel, endPos, flyTimeApproach, impulse);

		//直接加冲量，impulse 要转为追踪VVLH系
		//AsVVLHToICSMtx(tgPos, tgVel, mtxTgVVLhToICS);
		//AsICSToVVLHMtx(chPos, chVel, mtxChICSToVVLH);
		//impulse = mtxChICSToVVLH*mtxTgVVLhToICS*impulse;
	}
	//终端冲量
	else if (m_PulseNum==1 && m_FlyTime>=flyTimeApproach)
	{
		m_PulseNum = 2;
		m_FlyTime  = 0;

        GuidanceImpulse2(chPos, chVel, tgPos, tgVel, CCoord(0,0,0), impulse);

		//直接加冲量，impulse 要转为追踪VVLH系
		//AsVVLHToICSMtx(tgPos, tgVel, mtxTgVVLhToICS);
		//AsICSToVVLHMtx(chPos, chVel, mtxChICSToVVLH);
		//impulse = mtxChICSToVVLH*mtxTgVVLhToICS*impulse;
	}
	else if (m_PulseNum==2)
	{
		isEnd = true;
	}

	//脉冲施加后时间累积
	m_FlyTime += step;
}



