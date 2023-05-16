// Spacecraft.h: interface for the CSpacecraft class.
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
class CSpacecraft
{
//
// Operation.
//
public:
	CSpacecraft();
	virtual ~CSpacecraft();


	//
	//���Բ�������
	//
	inline void		SetName (const std::string& name);		// ���ú���������
	inline void		SetPos (const CCoord& pos);	            // ���ú�����λ��
	inline void		SetVel (const CCoord& vel);	            // ���ú������ٶ�
	inline void		SetElapsedSec (double sec);				// �����ۼ�ʱ��
	inline const std::string&	GetName() const;			// �õ�����������
	inline const CCoord&		GetPos() const;		        // �õ�������λ��
	inline const CCoord&		GetVel() const;		        // �õ��������ٶ�
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
 void		CSpacecraft::SetName(const std::string& name)
{m_Name=name;}			

// ���ú�����״̬
inline void		CSpacecraft::SetPos(const CCoord& pos)
{m_Pos=pos;}	

inline void		CSpacecraft::SetVel(const CCoord& vel)
{m_Vel=vel;}	

// �����ۼ�ʱ��
inline void		CSpacecraft::SetElapsedSec(double sec)		             
{m_ElapsedSec=sec;}	

// �õ�����������
inline const std::string&		CSpacecraft::GetName() const          
{return m_Name;}			

// �õ�������״̬
inline const CCoord&		CSpacecraft::GetPos() const
{return m_Pos;}		

inline const CCoord&		CSpacecraft::GetVel() const
{return m_Vel;}		

// �õ��ۼ�ʱ��
inline double					CSpacecraft::GetElapsedSec() const    
{return m_ElapsedSec;}	
	
		


#endif // !defined(_Spacecraft_H__INCLUDED_)
