// ChCtrl.h: interface for the CChCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CHCTRL_H__)
#define _CHCTRL_H__


#include "AstroLib.h"


//
//追踪器制导控制类
//
class CChCtrl  
{
public:
	CChCtrl();
	virtual ~CChCtrl();

	void	Init();
	void	TimeAdvance(double step,
			const CCoord& chPos, const CCoord& chVel, const CCoord& tgPos, const CCoord& tgVel, 
			double flyTimeApproach, const CCoord& endPos, bool& isEnd, CCoord& impulse);
	

private:
	void	GuidanceImpulse1(
			const CCoord& chPos, const CCoord& chVel, 
			const CCoord& tgPos, const CCoord& tgVel, const CCoord& endRelPos, 
			double flyTime, CCoord& impulse);
	void	GuidanceImpulse2(const CCoord& chPos, const CCoord& chVel, const CCoord& tgPos, const CCoord& tgVel,
			const CCoord& endRelVel, CCoord& impulse);

    void    Measure(const CCoord& chPos, const CCoord& chVel,
			const CCoord& tgPos, const CCoord& tgVel, 
            CCoord& relPos, CCoord& relVel);
    void    Engine(CCoord& impulse);

public:
    //测量误差
    int			m_UseMeasureRandom;			// 是否考虑测量误差:0=无随机;1=随机
    int			m_UseEngineRandom;			// 是否考虑执行误差:0=无随机;1=随机
    double      m_PosStdDev;                // 位置测量标准差
    double      m_VelStdDev;                // 速度测量标准差
    double      m_EngStdDev;                // 执行误差标准差

	//
	//内部参数
	//
private:
	double	    m_FlyTime;					// 从施加第一次冲量开始的飞行时间
	int		    m_PulseNum;					// 冲量次数

    CRandom		m_RandomPos[3];				// 位置随机数
    CRandom		m_RandomVel[3];				// 速度随机数
    CRandom		m_RandomEng[3];				// 执行机构随机数

	
};


#endif // !defined(_CHCTRL_H__)
