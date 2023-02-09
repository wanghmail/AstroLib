
#if !defined(_RENDEZVOUS_H__INCLUDED_)
#define _RENDEZVOUS_H__INCLUDED_

#pragma once


#include "AstroLib.h"
#include "Spacecraft.h"
#include "ChCtrl.h"


//
//����������Խ��ܿ�
//
class CRendezvous
{
public:
	void Main();						    // ����������
    void MainMonteCarlo();                  // MonteCarlo����������

private:
	void Initialization();				    // ��ʼ��
	bool TimeAdvance(double step);		    // ʱ���ƽ�
	void ReportGeneration();			    // �����������


public:
	double          m_Time;					// ����ʱ��

    CCoord          m_RelPos;               // ����ٶ�
    CCoord          m_RelVel;               // ����ٶ�

public:
	//��������м�����
    struct CHistoryData
    {
	    std::vector<double>	m_TimeList;			// �����ܹ�����ʱ��[s]
	    std::vector<CCoord> m_RelPosList;		// ���λ��״̬[m]
	    std::vector<CCoord> m_RelVelList;		// ����ٶ�״̬[m/s]
    };
    CHistoryData	m_HistoryData;	    // ��ʷ����

private:
    CChCtrl         m_ChCtrl;               // ׷��������
    CSpacecraft     m_ChDyn;                // ׷��������ѧ
    CSpacecraft     m_TgDyn;                // Ŀ��������ѧ

};



#endif // _RENDEZVOUS_H__INCLUDED_
