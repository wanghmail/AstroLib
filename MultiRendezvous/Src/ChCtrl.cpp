// ChCtrl.cpp: implementation of the CChCtrl class.
//
//////////////////////////////////////////////////////////////////////


#include "ChCtrl.h"
#include "izzoLam.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChCtrl::CChCtrl()
{
    m_UseMeasureRandom = 0;
    m_UseEngineRandom = 0;
    m_PosStdDev = 0;
    m_VelStdDev = 0;
    for (int i = 0; i < 100; i++)
    {
        m_FlyTime[i] = 0;
    }
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
    for (int i = 0; i < 100; i++)
    {
        m_FlyTime[i] = 0;
    }
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
/// 根据CW方程/lambert方程外推计算下一步需要施加的冲量
/// @Author	Wang Hua/Wang Hanwei
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
    if(1){
        double orbAngVel;
        CCoord OrbAngVelVec;
        CCoord relPos, relVel, m_tgPos, m_tgVel, m_tgPos1, m_tgVel1;
        Measure(tgPos, tgVel, chPos, chVel, relPos, relVel);
        AsVVLHRelStateToState(relPos, relVel, chPos, chVel, m_tgPos, m_tgVel);
        COrbit m_Orbit;
        for (int i = 0; i < flyTime; i++)
        {
            m_Orbit.OrbitStep(1, m_tgPos, m_tgVel);
        }
        AsVVLHRelStateToState(endRelPos, CCoord(0,0,0), m_tgPos, m_tgVel, m_tgPos1, m_tgVel1);
        m_tgPos = m_tgPos1;
        double r1[3]{ chPos[0], chPos[1], chPos[2] }, r2[3]{ m_tgPos[0],m_tgPos[1],m_tgPos[2] }, tof = flyTime, mu = 3.986e14, v1[1][3], v2[1][3];
        int way = 1, multi_revs = 0;
        solve_lambert_izzo(r1, r2, tof, mu, way, multi_revs, v1, v2);
        CCoord impulse2(v1[0][0], v1[0][1], v1[0][2]);
        AsStateToVVLHRelState(chPos, impulse2, chPos, chVel, m_tgPos, impulse);
        Engine(impulse);
    }else{
        double orbAngVel;
        CCoord impulse2;
        CCoord OrbAngVelVec;
        CCoord relPos, relVel;
        Measure(chPos, chVel, tgPos, tgVel, relPos, relVel);
        AsCartStateToOrbAngVel(tgPos, tgVel, OrbAngVelVec);
        orbAngVel = OrbAngVelVec.Norm();
        AsCWImpulse(relPos, relVel, endRelPos,
            CCoord(0, 0, 0), flyTime, orbAngVel, impulse, impulse2);
        Engine(impulse);
    }
    
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
	impulse = -relVel;
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
			bool& isEnd, CCoord& impulse, int aim)
{
	isEnd = false;
	impulse.SetDataAll(0);

	//至终端
	if (m_PulseNum==0)
	{
		m_PulseNum = 1;
        GuidanceImpulse1(chPos, chVel, tgPos, tgVel, endPos, flyTimeApproach, impulse);
        double t = 0;
        for (int i = 0; i <= 3; i++)
        {
            t = t + m_FlyTime[i];
        }
        CCTime c_t(2024, 3, 11, 12, 0, 0.0+t);
        AsTimeToJD(c_t, t);
        printf("儒略日（JD）：%f  远距离脉冲分量：%f %f %f \n", t, impulse[0], impulse[1], impulse[2]);
	}
	//终端冲量
	else if (m_PulseNum==1 && m_FlyTime[aim] >=flyTimeApproach)
	{
        double t = 0;
        for (int i = 0; i <= 3; i++)
        {
            t = t + m_FlyTime[i];
        }
        CCTime c_t(2024, 3, 11, 12, 0, 0.0 + t);
        AsTimeToJD(c_t, t);
        GuidanceImpulse2(chPos, chVel, tgPos, tgVel, endPos, impulse);
        printf("儒略日（JD）：%f  近距离脉冲分量：%f %f %f \n", t, impulse[0], impulse[1], impulse[2]);
        printf("儒略日（JD）：%f  观测目标%d \n", t, aim + 1);
		m_PulseNum = 2;
	}
    else if (m_PulseNum == 2 && m_FlyTime[aim] >= flyTimeApproach+1800)
    {
        isEnd = true;
        m_PulseNum = 0;
    }

	//脉冲施加后时间累积
	m_FlyTime[aim] += step;
}



