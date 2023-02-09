////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsRandom.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsRandom.h"
#include "AsMath.h"
#include <iostream>


using namespace std;



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
//生成[0,1]区间的随机数
//Return a uniform random deviate between 0.0 to 1.0.
//Author:	Wang Hua
//Input:	m_Seed	Set or reset m_Seed to any integer value(except the unlikely
//					value MASK) to initialize the sequence; m_Seed must not be
//					altered between calls for successive deviates in a sequence.
//Return:			random number between 0.0 to 1.0.
//********************************************************************
double CRandom::Rand0()
{
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


//********************************************************************
//Return a normally distributed deviate with zero mean and unit variance.
/// @Author	Wang Hua
/// @Return			random number.
//********************************************************************
double CRandom::Gauss()
{
    double fac, rsq, v1, v2;

    //Reinitialize.
    if (m_Seed < 0)
        m_iset = 0;
    //We don't have an extra deviate handy, so pick two uniform numbers
    //	in the square extending from -1 to +1 in each direction.
    if (m_iset == 0)
    {
        do
        {
            v1=2.0*Rand0()-1.0;
            v2=2.0*Rand0()-1.0;
            //see if they are in the unit circle, and if they are not, try again.
            rsq=v1*v1+v2*v2;
        } while (rsq >= 1.0 || rsq == 0.0);
        fac=sqrt(-2.0*log(rsq)/rsq);
        //Now make the Box-Muller transformation to get two normal deviates.
        //	Return one and save the other for next time.
        m_gset=v1*fac;
        //Set flag.
        m_iset=1;
        return v2*fac;
    }
    else
    {
        //We have an extra deviate handy, so unset the flag, and return it.
        m_iset=0;
        return m_gset;
    }
}
