////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

//AsTimeSystem.h
//
//////////////////////////

#if! defined (_ASTIMESYSTEM_H_)
#define  _ASTIMESYSTEM_H_


#include "AsCommon.h"
#include <string>
#include <time.h>





class CCTimeSpan;


//
// Date Conversion Constants
//
const double AsCMJDOffset  = 2400000.5;
const double AsCTTMinusTAI = 32.184;


//
// Coordinate System Epochs
//
const double AsCB1950Epoch = 2433282.4234591;
const double AsCJ2000Epoch = 2451545.0;


//
// Earth_Motion_Constants
//
const double AsCEarthSiderealDay  = 86164.09054;
const double AsCEarthSiderealYear = 365.25636;


//
//Gregorian calendar time.
//
class CCTime
{
	//
	//operation.
	//
public:
	CCTime();
	CCTime(int year, int mon, int day, int hour, int min, double sec);
	CCTime(const CCTime& time);
	explicit CCTime(time_t time);
	explicit CCTime(const tm* ptm);
	virtual ~CCTime();
	
	//
	//Attribute.
	//
public:
	int		m_Year;		///< year
	int		m_Mon;		///< month,  1 - 12
	int		m_Day;		///< day,    1 - 31
	int		m_Hour;		///< hour,   0 - 23
	int		m_Min;		///< minute, 0 - 59
	double	m_Sec;		///< second, 0 - 59.99..
};


//
//Time span class.
//
class CCTimeSpan
{
	//
	//operation.
	//
public:
	CCTimeSpan();
	CCTimeSpan(int day,int hour,int min,double sec);
	virtual ~CCTimeSpan();
	//********************************************************************
	/// Get total Days.
	/// @Author	Wang Hua
	/// @Date	2004.9.16
	//********************************************************************
	double				GetTotalDay () const;

	//********************************************************************
	/// Get total Hours.
	/// @Author	Wang Hua
	/// @Date	2004.9.15
	//********************************************************************
	double				GetTotalHour() const;

	//********************************************************************
	/// Get Total Minutes.
	/// @Author	Wang Hua
	/// @Date	2004.9.15
	//********************************************************************
	double				GetTotalMin () const;

	//********************************************************************
	/// Get total Seconds.
	/// @Author	Wang Hua
	/// @Date	2004.9.15
	//********************************************************************
	double				GetTotalSec () const;

	//********************************************************************
	/// update the timespan according the total sec.
	/// @Author	Wang Hua
	/// @Date	2004.11.19
	//********************************************************************
	void				FromSec     (double sec);

	//********************************************************************
	//update the timespan according the total day.
	//Author:	Wang Hua
	//Date:		2004.11.19
	//********************************************************************
	void				FromDay     (double days);


	//
	//Attribute.
	//
public:
	int		m_Day;		///< 天数
	int		m_Hour;		///< 小时数
	int		m_Min;		///< 分钟数
	double	m_Sec;		///< 秒数
};


//
//General Date Functions.
//


//********************************************************************
/// 判断是否闰年
/// According Gregorian Calendar determine if the year is leap year.
/// @Author	Wang Hua
/// @Date	2004.9.21
/// @Input
/// @Param  year    年
/// @Return         true=闰年; false=不是闰年
//********************************************************************
bool	AsIsLeapYear  (int year);

//********************************************************************
/// 根据时间计算儒略日
/// @Author	Wang Hua
/// @Date	2004.9.16
/// @Input	t       date in CCTime format
/// @Output	JD		Julian Date
//********************************************************************
void    AsTimeToJD(const CCTime& t, double& JD);






#endif //_ASTIMESYSTEM_H_


