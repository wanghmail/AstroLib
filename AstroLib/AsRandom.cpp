////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//             College of Aerospace and Material Engineering              //
//               National University of Defense Technology                //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsRandom.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsRandom.h"
#include "AsMath.h"
#include <iostream>


using namespace std;
AsMUsingNamespace


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRandom::CRandom()
{
	m_Seed = -1;
}


CRandom::CRandom(int seed)
{
}


CRandom::~CRandom()
{

}


//********************************************************************
//Return a uniform random deviate between 0.0 to 1.0.
//Author:	Wang Hua
//Input:	m_Seed	Set or reset m_Seed to any integer value(except the unlikely
//					value MASK) to initialize the sequence; m_Seed must not be
//					altered between calls for successive deviates in a sequence.
//Return:			random number between 0.0 to 1.0.
//********************************************************************
double CRandom::Rand0()
{
	//<<Numerical Recipes in C++>> P283 ran0

	const int		IA=16807, IM=2147483647, IQ=127773;
	const int		IR=2836, MASK=123459876;
	const double	AM=1.0/double(IM);

	int	k;
	double	ans;

	//XORing with MASK allows use of zero and other simple bit patterns for m_Seed.
	m_Seed ^= MASK;
	k = m_Seed/IQ;
	//Compute m_Seed=(IA*m_Seed) %IM without overflows by Schrage's method.
	m_Seed = IA*(m_Seed-k*IQ)-IR*k;
	if (m_Seed<0)
		m_Seed += IM;
	//Convert m_Seed to a floating result.
	ans = AM*m_Seed;
	//Unmark before return.
	m_Seed ^= MASK;
	return ans;
}


