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
/// @brief	随机数类，头文件

#if !defined(_ASRANDOM_H_)
#define _ASRANDOM_H_


#include "AsCommon.h"


AsMBeginNamespace


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



	//
	//Attribute.
	//
private:
	int		m_Seed;		///< seed of random


};


//
//Inline funciton
//
//***********************************************************************
/// 初始化随机数种子，需要产生独立分布的随机数时必须调用这个函数
/// @Author	Wang Hua
/// @Date	2007.4.29
/// @Input	
/// @Param	seed	随机数种子，负整数
//***********************************************************************
inline void CRandom::Init(int seed)
{
	m_Seed = seed;
}


//***********************************************************************
/// 返回当前的随机数种子
/// @Author	Wang Hua
/// @Date	2007.4.29
/// @Output	
/// @Param	seed	随机数种子，负整数
//***********************************************************************
inline int CRandom::GetSeed()
{
	return m_Seed;
}


AsMEndNamespace


#endif // !defined(_ASRANDOM_H_)
