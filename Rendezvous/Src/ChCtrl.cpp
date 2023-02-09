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
/// ��ʼ��
/// @author	Wang Hua
/// @Date	2005.11.16
//*********************************************************************
void CChCtrl::Init()
{
	m_FlyTime  = 0;
	m_PulseNum = 0;

    //�������ʼ��
    int seed = AsRandomSeed();
    for (int i=0; i<3; i++)
    {
        m_RandomPos[i].Init(seed-i*3-1);
        m_RandomVel[i].Init(seed-i*3-2);
        m_RandomEng[i].Init(seed-i*3-3);
    }
}


//********************************************************************
/// �������㣬�����������
/// @author	Wang Hua
/// @Date	2022-6-29
/// @Input
/// @Param	chPos			׷��λ��
/// @Param	chVel			׷���ٶ�
/// @Param	tgPos			Ŀ��λ��
/// @Param	tgVel			Ŀ���ٶ�
/// @Output
/// @Param	relPos			���λ��
/// @Param	relVel			����ٶ�
//********************************************************************
void CChCtrl::Measure(const CCoord& chPos, const CCoord& chVel,
            const CCoord& tgPos, const CCoord& tgVel,
            CCoord& relPos, CCoord& relVel)
{
    //�������λ���ٶ�
    AsStateToVVLHRelState(chPos, chVel, tgPos, tgVel, relPos, relVel);

    //���Ӳ���ƫ��
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
/// ����ִ�нṹִ�к������
/// @author	Wang Hua
/// @Date	2022-6-29
/// @In/Out
/// @Param	impulse			���λ��
//********************************************************************
void CChCtrl::Engine(CCoord& impulse)
{
    //���Ӳ���ƫ��
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
/// ����CW�������Ƽ�����һ����Ҫʩ�ӵĳ���
/// @Author	Wang Hua
/// @Date	2005.11.17
/// @Input
/// @Param	chPos			׷��λ��(m)
/// @Param	chVel			׷���ٶ�(m/s)
/// @Param	tgPos			Ŀ��λ��(m)
/// @Param	tgVel			Ŀ���ٶ�(m/s)
/// @Param	endRelPos		�Ƶ��յ��������λ��(m)
/// @Param	flyTime			�Ƶ�����ʱ��
/// @Output
/// @Param	impulse			����õ��ĳ���
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
//����CW�������Ƽ�����һ����Ҫʩ�ӵĳ�����׼����ʼ����
/// @Author	Wang Hua
/// @Date	2005.11.17
/// @Input
/// @Param	chPos			׷��λ��(m)
/// @Param	chVel			׷���ٶ�(m/s)
/// @Param	tgPos			Ŀ��λ��(m)
/// @Param	tgVel			Ŀ���ٶ�(m/s)
/// @Param	endRelPos		�Ƶ��յ���������ٶ�(m/s)
/// @Output
/// @Param	impulse			����õ��ĳ���
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
/// �������,ֱ�Ӽӳ���
/// @author	Wang Hua
/// @Date	2006.8.12
/// @Param	step			ʱ�䲽��
/// @Param	chPos			����ϵ�е�׷����λ��(m)
/// @Param	chVel			����ϵ�е�׷�����ٶ�(m/s)
/// @Param	tgPos			����ϵ�е�Ŀ����λ��(m)
/// @Param	tgVel			����ϵ�е�Ŀ�����ٶ�(m/s)
/// @Param	flyTimeApproach	�ӽ��η���ʱ��
/// @Output
/// @Param	isEnd			�Ƿ񱾽׶ν���
/// @Param	impulse			VVLHϵ�е�����
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

	//���ն�
	if (m_PulseNum==0)
	{
		m_PulseNum = 1;
		m_FlyTime  = 0;

        GuidanceImpulse1(chPos, chVel, tgPos, tgVel, endPos, flyTimeApproach, impulse);

		//ֱ�Ӽӳ�����impulse ҪתΪ׷��VVLHϵ
		//AsVVLHToICSMtx(tgPos, tgVel, mtxTgVVLhToICS);
		//AsICSToVVLHMtx(chPos, chVel, mtxChICSToVVLH);
		//impulse = mtxChICSToVVLH*mtxTgVVLhToICS*impulse;
	}
	//�ն˳���
	else if (m_PulseNum==1 && m_FlyTime>=flyTimeApproach)
	{
		m_PulseNum = 2;
		m_FlyTime  = 0;

        GuidanceImpulse2(chPos, chVel, tgPos, tgVel, CCoord(0,0,0), impulse);

		//ֱ�Ӽӳ�����impulse ҪתΪ׷��VVLHϵ
		//AsVVLHToICSMtx(tgPos, tgVel, mtxTgVVLhToICS);
		//AsICSToVVLHMtx(chPos, chVel, mtxChICSToVVLH);
		//impulse = mtxChICSToVVLH*mtxTgVVLhToICS*impulse;
	}
	else if (m_PulseNum==2)
	{
		isEnd = true;
	}

	//����ʩ�Ӻ�ʱ���ۻ�
	m_FlyTime += step;
}



