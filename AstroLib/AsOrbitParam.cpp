////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2006                          //
//                              Wang Hua                                  //
//             College of Aerospace and Material Engineering              //
//               National University of Defense Technology                //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsOrbitParam.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsOrbitParam.h"
#include "AsInterpolation.h"


AsMUsingNamespace


//////////////////////////////////////////////////////////////////////
//CCartState
CCartState::CCartState() : m_Pos(0.0, 0.0, 0.0), m_Vel(0.0, 0.0, 0.0)
{
}


CCartState::CCartState(const CCoord3& pos, const CCoord3& vel) : m_Pos(pos), m_Vel(vel)
{
}


CCartState::CCartState(double xPos, double yPos, double zPos, double xVel, double yVel, double zVel)
	: m_Pos(xPos, yPos, zPos), m_Vel(xVel, yVel, zVel)
{
}

//重载求负运算
const CCartState CCartState::operator -() const
{
	return CCartState(-m_Pos, -m_Vel);
}

//重载相减运算
const CCartState& CCartState::operator -=(const CCartState& state)
{
	m_Pos -= state.m_Pos;
	m_Vel -= state.m_Vel;
	return *this;
}


const CCartState& CCartState::operator +=(const CCartState& state)
{
	m_Pos += state.m_Pos;
	m_Vel += state.m_Vel;
	return *this;
}


const CCartState CCartState::operator -(const CCartState& state) const
{
	CCartState s=*this;
	s -= state;
	return s;
}


const CCartState CCartState::operator +(const CCartState& state) const
{
	CCartState s=*this;
	s += state;
	return s;
}


//////////////////////////////////////////////////////////////////////
//CModOrbElem
CModOrbElem::CModOrbElem() 
			: m_PeriRad(0.0), m_Ecc(0.0), m_I(0.0), 
			  m_RAAN(0.0), m_ArgPeri(0.0), m_TrueA(0.0)

{
}


CModOrbElem::CModOrbElem(double periRad, double ecc, double i, 
						 double raan, double argPeri, double trueA)
	: m_PeriRad(periRad), m_Ecc(ecc), m_I(i), 
	  m_RAAN(raan), m_ArgPeri(argPeri), m_TrueA(trueA)
{
}


//////////////////////////////////////////////////////////////////////
//COrbElem
COrbElem::COrbElem() : m_SMajAx(0.0), m_Ecc(0.0), m_I(0.0), m_RAAN(0.0), m_ArgPeri(0.0), m_TrueA(0.0)
{
}


COrbElem::COrbElem(double sMajAx, double ecc, double i, 
				   double raan, double argPeri, double trueA)
	: m_SMajAx(sMajAx), m_Ecc(ecc), m_I(i), 
	  m_RAAN(raan), m_ArgPeri(argPeri), m_TrueA(trueA)
{
}



