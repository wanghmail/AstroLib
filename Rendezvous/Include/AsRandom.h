////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsRandom.h
//
//////////////////////////////////////////////////////////////////////
/// @file
/// @brief	������࣬ͷ�ļ�

#if !defined(_ASRANDOM_H_)
#define _ASRANDOM_H_


#include "AsCommon.h"
#include <time.h>
#include <cmath>





//
//Random number generator class.
//
class CRandom
{
	//
	//Operation.
	//
public:
	CRandom();
	CRandom(int seed);
	virtual ~CRandom();

	//
	//seed operate function
	//
	void	Init (int seed);
	int		GetSeed ();

	//
	//Uniform derivates
	//
	double	Rand0 ();

    double  Gauss();		///< ���ɾ�ֵΪ0����Ϊ1��Gauss�ֲ������

	//
	//Attribute.
	//
private:
	int		m_Seed;		    ///< seed of random

	//Gauss().
    int		m_iset;
    double	m_gset;

};

//
//Generate the seed of random according current system time.
//
inline int AsRandomSeed()
{
    time_t ltime;
    time(& ltime);
    return static_cast<int>(-ltime-rand());
}


//
//Inline funciton
//
//***********************************************************************
/// ��ʼ����������ӣ���Ҫ���������ֲ��������ʱ��������������
/// @Author	Wang Hua
/// @Date	2007.4.29
/// @Input	
/// @Param	seed	��������ӣ�������
//***********************************************************************
inline void CRandom::Init(int seed)
{
	m_Seed = seed;
}


//***********************************************************************
/// ���ص�ǰ�����������
/// @Author	Wang Hua
/// @Date	2007.4.29
/// @Output	
/// @Param	seed	��������ӣ�������
//***********************************************************************
inline int CRandom::GetSeed()
{
	return m_Seed;
}





#endif // !defined(_ASRANDOM_H_)
