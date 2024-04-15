// Spacecraft.h: interface for the CMulSpacecraft class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_Spacecraft_H__INCLUDED_)
#define _Spacecraft_H__INCLUDED_

#pragma once


#include "AstroLib.h"
#include <fstream>


//
//����������ѧ��,�����˺������Ĳ����Ͳ���
//
class CMulSpacecraft
{
    //
    // Operation.
    //
public:
    CMulSpacecraft();
    virtual ~CMulSpacecraft();


    //
    //���Բ�������
    //
    inline void		SetName(const std::string& name);		// ���ú���������
    inline void		SetPos(const CCoord& pos);	            // ���ú�����λ��
    inline void		SetVel(const CCoord& vel);	            // ���ú������ٶ�
    inline void		SetElapsedSec(double sec);				// �����ۼ�ʱ��
    inline const std::string& GetName() const;			// �õ�����������
    inline const CCoord& GetPos() const;		        // �õ�������λ��
    inline const CCoord& GetVel() const;		        // �õ��������ٶ�
    inline double				GetElapsedSec() const;		// �õ��ۼ�ʱ��

    void	Init();                                         // �����ʼ��
    void	TimeAdvance(double step, int burnCoordSys,
        const CCoord& burnValue);                       // ʱ���ƽ�
    void    ReportGeneration();                             // �����������

private:
    //
    //�����м�����
    //
    void	UpdateBuffer();

    //
    // Attribute.
    //
public:
    std::string		m_Name;				// ����������
    CCoord		    m_Pos;		        // ������λ��,���Ĺ�������ϵ��ʾ
    CCoord		    m_Vel;		        // �������ٶ�,���Ĺ�������ϵ��ʾ
    double			m_ElapsedSec;		// �ۼ�ʱ��(s)


public:
    //
    //���������ݻ�����
    //
    struct CHistoryData
    {
        std::vector<double>		m_ElapsedSec;			// �����ܹ�����ʱ��[s]
        std::vector<CCoord>		m_Pos;				    // λ��״̬[m]
        std::vector<CCoord>		m_Vel;				    // �ٶ�״̬[m/s]
    };
    CHistoryData	m_HistoryData;	    // ��ʷ����

protected:
    COrbit			m_Orbit;		    // ���������

};


//
//inline function.
//

// ���ú���������
void		CMulSpacecraft::SetName(const std::string& name)
{
    m_Name = name;
}

// ���ú�����״̬
inline void		CMulSpacecraft::SetPos(const CCoord& pos)
{
    m_Pos = pos;
}

inline void		CMulSpacecraft::SetVel(const CCoord& vel)
{
    m_Vel = vel;
}

// �����ۼ�ʱ��
inline void		CMulSpacecraft::SetElapsedSec(double sec)
{
    m_ElapsedSec = sec;
}

// �õ�����������
inline const std::string& CMulSpacecraft::GetName() const
{
    return m_Name;
}

// �õ�������״̬
inline const CCoord& CMulSpacecraft::GetPos() const
{
    return m_Pos;
}

inline const CCoord& CMulSpacecraft::GetVel() const
{
    return m_Vel;
}

// �õ��ۼ�ʱ��
inline double					CMulSpacecraft::GetElapsedSec() const
{
    return m_ElapsedSec;
}




#endif // !defined(_Spacecraft_H__INCLUDED_)
