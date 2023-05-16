////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

//AsMath.h
//
//////////////////////////////////////////////////////////////////////


#if !defined(_ASMATH_H_)
#define _ASMATH_H_


#include <cmath>
#include <cassert>
#include <float.h>
#include <limits.h>
#include <vector>
#include "AsCommon.h"





template<class>
class CVector;


//
// Constants representing extreme values.
//
#define AsCHuge         HUGE_VAL
#define AsCIntMax       INT_MAX							///< 2147483647, maximum (signed) int value
#define AsCIntMin       INT_MIN							///< -2147483647-1, Minimum (signed) int value
#define AsCUItnMax      UINT_MAX						///< 4294967295(0xffffffff), Maximum unsigned int value 
#define AsCDblMax       DBL_MAX							///< 1.7976931348623158e+308, maximum double value
#define AsCDblMin       (-DBL_MAX)						///< -1.7976931348623158e+308, minimum double value
#define AsCDblEpsilon	DBL_EPSILON						///< 2.2204460492503131e-016, Smallest such that 1.0+DBL_EPSILON !=1.0 


//
// Constants representing various multiples of PI.
//
const double AsCPI          = 3.141592653589793238462643383279502884197169399375105;
const double AsCTwoPI       = 6.2831853071795864769;		///< AsCPI+AsCPI
const double AsCFourPI      = 12.5663706144;				///< 4*PI
const double AsCHalfPI      = 1.5707963267948966192;		///< 0.5*AsCPI
const double AsCThirdPI     = 1.0471975511965977461;		///< PI/3
const double AsCQuarterPI   = 0.7853981633974483096;		///< PI/4
const double AsCSqrPI       = 9.86960440108935861883;		///< PI*PI, PI squared



//===================================
// Data for physical constant systems
//===================================

// 
//           Item                   Value                      Comments
//
const double AsCGrav              = 6.673e-11;				///< Constant of gravitation	[m^3/kg*s^2] 
const double AsCEarthGrav         = 3.986004418e14;			///< Geocentric gravitation constant	[m^3/s^2]
const double AsCSunGrav           = 1.327122e20;			///< Heliocentric gravitation constant [m^3/s^2]
const double AsCMoonGrav          = 4.9027949e12;			///< moon gravitation constant [m^3/s^2]
const double AsCAU                = 1.49597870691e11;		///< Astronomical unit in meters (IAU 1976 value)[m]
const double AsCSunRadius         = 695990000.0;			///< Equatorial radius of the Sun [m]
const double AsCEarthRadius       = 6378137.0;				///< Equatorial radius of the Earth (WGS84)[m]
const double AsCMoonRadius        = 1738200.0;				///< Equatorial radius of the Moon [m]
const double AsCEarthFlatFact     = 1.0/298.25642;			///< Flattening factor of the Earth
const double AsCLightSpeed        = 2.99792458e8;			///< speed of light (IAU 1976 value) [m/s]		
const double AsCEarthAngVel	      = 7.2921157394331994078908023245342e-5;///< Nominal mean angular velocity of the Earth (AsCTwoPI/86164.0919) [rad/s]


const double EARTH_J2             = 1.0826261e-3;			///< 地球摄动J2项
const double EARTH_J3             = -2.54e-6;				///< 地球摄动J3项
const double EARTH_J4             = -1.61e-6;				///< 地球摄动J4项


//  
// Constants:   Time_Dimension_Constants 
//
const double AsCSecToMin          = 0.0166666666666666667;
const double AsCSecToHour         = 0.000277777777777777778;
const double AsCSecToDay          = 0.0000115740740740740741;
const double AsCSecToCenday       = 0.0000115740740740740741*100;				///< (AsCSecToDay * 100)

const double AsCMinToSec          = 60.0;
const double AsCMinToHour         = 0.0166666666666666667;
const double AsCMinToDay          = 0.000694444444444444444;

const double AsCHourToSec         = 3600.0;
const double AsCHourToMin         = 60.0;
const double AsCHourToDay         = 0.0416666666666666667;

const double AsCDayToSec          = 8.64e04;
const double AsCDayToMin          = 1440.0;
const double AsCDayToHour         = 24.0;

//
// Constants:   Angle_Dimension_Constants 
//
const double AsCRadToDeg          = 57.2957795130823208767;		///< 180.0/AsCPI
const double AsCDegToRad          = 0.0174532925199432957;		///< AsCPI/180.0
const double AsCArcSecToRad       = 4.848136811095359935899141e-6;	///< AsCDegToRad/3600.0
const double AsCSecArcToRad       = 0.0000048481368110953*15;	///< AsCArcSecToRad*15





//Find the maximum of a and b.
template <class _Type>
inline	const _Type AsMax(_Type a, _Type b) {return a>b ? a : b;}
//why the form Max(const _Type &a, const _Type &b) in the release edition is not good??????


//********************************************************************
/// Find the maximum of a, b and c.
/// @Author	Wang Hua
/// @Date	2006.3.8
//********************************************************************
template <class _Type>
inline	const _Type AsMax(
			_Type a, 
			_Type b, 
			_Type c)
{
	_Type x = a>b ? a : b;
	return x>c ? x : c;
}


//********************************************************************
/// Find the maximum of a, b, c and d.
/// @Author	Wang Hua
/// @Date	2006.3.8
//********************************************************************
template <class _Type>
inline	const _Type AsMax(
			_Type a, 
			_Type b, 
			_Type c, 
			_Type d)
{
	_Type x = a>b ? a : b;
	      x = x>c ? x : c;
	return x>d ? x : d;
}


//Find the minimum of a and b.
template <class _Type>
inline	const _Type AsMin(
			const _Type& a, 
			const _Type& b)	
{return a<b ? a : b;}


//********************************************************************
/// Find the minimum of a, b and c.
/// @Author	Wang Hua
/// @Date	2006.3.8
//********************************************************************
template <class _Type>
inline	const _Type AsMin(
			const _Type& a, 
			const _Type& b, 
			const _Type& c)
{
	_Type x = a<b ? a : b;
	return x<c ? x : c;
}


//********************************************************************
/// Find the minimum of a, b, c and d.
/// @Author	Wang Hua
/// @Date	2006.3.8
//********************************************************************
template <class _Type>
inline	const _Type AsMin(
			const _Type& a, 
			const _Type& b, 
			const _Type& c, 
			const _Type& d)
{
	_Type x = a<b ? a : b;
	      x = x<c ? x : c;
	return x<d ? x : d;
}


//Returns the sign of a number.
template <class _Type>
inline int AsSign(const _Type & x)
{
    return (x < (_Type)0) ? -1 : ((x > (_Type)0) ? 1 : 0);
}


//********************************************************************
/// @Author	Wang Hua
/// @Date	2006.6.5
//********************************************************************
template <class _Type>
inline const _Type AsSign(const _Type & x, const _Type & s)
{
    return s>=0 ? ( x>=0 ? x : -x ) : ( x>=0 ? -x : x );
}


//Degree to radian.
inline double AsDegToRad(double deg)
{
	return deg*AsCDegToRad;
}


//Radian to degree.
inline double AsRadToDeg(double rad)
{
	return rad*AsCRadToDeg;
}


//********************************************************************
/// 四舍五入算法
/// @Author	Wang Hua
/// @Date	2007.10.25
//********************************************************************
inline double AsRound(double num)
{
	return num>0 ? floor(num+0.5) : ceil(num-0.5);
}


//********************************************************************
/// rounds off a number.  The order indicates the place to which 
///  the round off occurs.  For example an order of 0 will round to 
///  the nearest 1, an order of 1 will round to the nearest 
///  10, an order of 2 will round to the nearest 100, etc.
/// @Author	Wang Hua
/// @Date	2005.5.2
//********************************************************************
inline double AsRound (double num, int order)
{
	double co = pow(10, order);
	return AsRound( num/co ) * co;
}


//开方
inline double AsSqrt(double x)
{
    if (x<0)
        return 0;

    return sqrt(x);
}

inline double AsSqr(double x)
{
	return x*x;
}


inline double AsCube(double x)
{
	return x*x*x;
}








#endif // !defined(_ASMATH_H_)
