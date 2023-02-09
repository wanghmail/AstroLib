// ChCtrl.h: interface for the CChCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CHCTRL_H__)
#define _CHCTRL_H__


#include "AstroLib.h"


//
//׷�����Ƶ�������
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
    //�������
    int			m_UseMeasureRandom;			// �Ƿ��ǲ������:0=�����;1=���
    int			m_UseEngineRandom;			// �Ƿ���ִ�����:0=�����;1=���
    double      m_PosStdDev;                // λ�ò�����׼��
    double      m_VelStdDev;                // �ٶȲ�����׼��
    double      m_EngStdDev;                // ִ������׼��

	//
	//�ڲ�����
	//
private:
	double	    m_FlyTime;					// ��ʩ�ӵ�һ�γ�����ʼ�ķ���ʱ��
	int		    m_PulseNum;					// ��������

    CRandom		m_RandomPos[3];				// λ�������
    CRandom		m_RandomVel[3];				// �ٶ������
    CRandom		m_RandomEng[3];				// ִ�л��������

	
};


#endif // !defined(_CHCTRL_H__)
