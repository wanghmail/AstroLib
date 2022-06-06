////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsOrbitParam.h
//////////////////////////

#if! defined (_ASORBITPARAM_H_)
#define  _ASORBITPARAM_H_


#include "AsCoordinate.h"


AsMBeginNamespace


//
//Cartesian state elements
//  Position and velocity in cartesian coordinates.
//
class CCartState
{
public:
	CCartState();
	CCartState(const CCoord3& pos, const CCoord3& vel);
	CCartState(double xPos, double yPos, double zPos, double xVel, double yVel, double zVel);

	const CCartState	operator -() const;
	const CCartState&	operator+=(const CCartState& state);
	const CCartState	operator +(const CCartState& state) const;
	const CCartState&	operator-=(const CCartState& state);
	const CCartState	operator -(const CCartState& state) const;

public:
	CCoord3	m_Pos;		///< λ�� position(m)
	CCoord3	m_Vel;		///< �ٶ� velocity(m/s)
};


//
//Modified orbital elements
//
class CModOrbElem
{
public:
	CModOrbElem();
	CModOrbElem(double periRad, double ecc, double i, double raan, double argPeri, double trueA);

public:
	double	m_PeriRad;	///<������뾶 periapsis radius in m
	double	m_Ecc;		///<ƫ���� eccentricity
	double	m_I;		///<������ inclination in radians
	double	m_RAAN;		///<������ྭ right ascension of ascending node
	double	m_ArgPeri;	///<������� arg of periapsis in rad
	double	m_TrueA;	///<������ true anomaly in radians
};


//
//classic orbit element.
//
class COrbElem
{
public:
	COrbElem();
	COrbElem(double sMajAx, double ecc, double i, double raan, double argPeri, double trueA);

public:
	double	m_SMajAx;	///< ������ Semimajor axis
	double	m_Ecc;		///< ƫ���� Eccentricity
	double	m_I;		///< ������ Inclination
	double	m_RAAN;		///< ������ྭ Right Ascension of the ascending node (RAAN)
	double	m_ArgPeri;	///< ������� Argument of Perigee
	double	m_TrueA;	///< ������ True Anomaly
};




AsMEndNamespace


#endif //_ASORBITPARAM_H_