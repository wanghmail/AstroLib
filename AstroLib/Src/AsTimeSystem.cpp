////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2006                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsTimeSystem.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsTimeSystem.h"
#include "AsMath.h"
#include <cassert>
#include <sstream>
#include <iomanip>



using namespace std;



//////////////////////////////////////////////////////////////////////
// CCTime

CCTime::CCTime() : m_Year(2000), m_Mon(1), m_Day(1), m_Hour(0), m_Min(0), m_Sec(0.0)

{
}


CCTime::CCTime(int year, int mon, int day, int hour, int min, double sec)
	: m_Year(year), m_Mon(mon), m_Day(day), m_Hour(hour), m_Min(min), m_Sec(sec)
{
}


CCTime::CCTime(const CCTime& time)
	: m_Year(time.m_Year), m_Mon(time.m_Mon), m_Day(time.m_Day), 
	  m_Hour(time.m_Hour), m_Min(time.m_Min), m_Sec(time.m_Sec)
{
}


//********************************************************************
//由于time_t代表 the seconds elapsed since midnight (00:00:00), January 1, 1970, 
//  coordinated universal time (UTC). 所以由这个构造函数得到的是UTC时间.
//Author:	Wang Hua
//Date:		2005.10.10
//********************************************************************
CCTime::CCTime(time_t time)
{
	tm *ptm = gmtime(&time);
	assert(ptm);

	m_Year = ptm->tm_year + 1900;
	m_Mon  = ptm->tm_mon+1;
	m_Day  = ptm->tm_mday;
	m_Hour = ptm->tm_hour;
	m_Min  = ptm->tm_min;
	m_Sec  = ptm->tm_sec;
}


//********************************************************************
//由于time_t代表 the seconds elapsed since midnight (00:00:00), January 1, 1970, 
//  coordinated universal time (UTC). 所以由这个构造函数得到的是UTC时间.
//Author:	Wang Hua
//Date:		2005.10.10
//********************************************************************
CCTime::CCTime(const tm* ptm)
{
	m_Year = ptm->tm_year + 1900;
	m_Mon  = ptm->tm_mon+1;
	m_Day  = ptm->tm_mday;
	m_Hour = ptm->tm_hour;
	m_Min  = ptm->tm_min;
	m_Sec  = ptm->tm_sec;
}


CCTime::~CCTime()
{
}




//////////////////////////////////////////////////////////////////////
// CCTimeSpan

CCTimeSpan::CCTimeSpan() : m_Day(0), m_Hour(0), m_Min(0), m_Sec(0.0)
{
}


CCTimeSpan::CCTimeSpan(int day,int hour,int min,double sec) : m_Day(day), m_Hour(hour), m_Min(min), m_Sec(sec)
{
}


CCTimeSpan::~CCTimeSpan()
{
}


//********************************************************************
//Get total Days.
//Author:	Wang Hua
//Date:		2004.9.16
//********************************************************************
double CCTimeSpan::GetTotalDay() const
{
	return m_Day+m_Hour*AsCHourToDay+m_Min*AsCMinToDay+m_Sec*AsCSecToDay;
}


//********************************************************************
//Get total Hours.
//Author:	Wang Hua
//Date:		2004.9.15
//********************************************************************
double CCTimeSpan::GetTotalHour() const
{
	return AsCDayToHour*m_Day+m_Hour+m_Min*AsCMinToHour+m_Sec*AsCSecToHour;
}


//********************************************************************
//Get Total Minutes.
//Author:	Wang Hua
//Date:		2004.9.15
//********************************************************************
double CCTimeSpan::GetTotalMin() const
{
	return m_Day*AsCDayToMin+m_Hour*AsCHourToMin+m_Min+m_Sec*AsCSecToMin;
}


//********************************************************************
//Get total Seconds.
//Author:	Wang Hua
//Date:		2004.9.15
//********************************************************************
double CCTimeSpan::GetTotalSec() const
{
	return m_Day*AsCDayToSec+m_Hour*AsCHourToSec+m_Min*AsCMinToSec+m_Sec;
}


//********************************************************************
//update the timespan according the total sec.
//Author:	Wang Hua
//Date:		2004.11.19
//********************************************************************
void CCTimeSpan::FromSec(double sec)
{
	double temp;
	m_Day  = int(sec*AsCSecToDay);
	temp   = sec-m_Day*AsCDayToSec;
	m_Hour = int(temp*AsCSecToHour);
	temp   = temp-m_Hour*AsCHourToSec;
	m_Min  = int(temp*AsCSecToMin);
	m_Sec  = temp-m_Min*AsCMinToSec;
}


//********************************************************************
//update the timespan according the total day.
//Author:	Wang Hua
//Date:		2004.11.19
//********************************************************************
void CCTimeSpan::FromDay(double days)
{
	double Temp;
	m_Day  = int(days);
	Temp   = (days - m_Day) * AsCDayToHour;
	m_Hour = int(Temp);
	Temp   = (Temp - m_Hour) * AsCHourToMin;
	m_Min  = int(Temp);
	m_Sec  = (Temp - m_Min) * AsCMinToSec;
}


//********************************************************************
/// 判断是否闰年
/// According Gregorian Calendar determine if the year is leap year.
/// @Author	Wang Hua
/// @Date	2004.9.21
/// @Input
/// @Param  year    年
/// @Return         true=闰年; false=不是闰年
//********************************************************************
bool AsIsLeapYear(int year)
{
	if ( (year%400==0) || (year%100!=0 && year%4==0) )
		return true;
	else
		return false;
}


//********************************************************************
/// 根据历元时间计算儒略日
/// @Author	Wang Hua
/// @Date	2004.9.16
/// @Input	t       date in CCTime format
/// @Output	JD		Julian Date
//********************************************************************
void AsTimeToJD(const CCTime& t, double& JD)
{
    JD = 367.0 * t.m_Year -
        int((7 * (t.m_Year + int((t.m_Mon + 9) / 12))) * 0.25) +
        int( 275 * t.m_Mon / 9 ) +
        t.m_Day + 1721013.5 +
        ((t.m_Sec / 60.0 + t.m_Min) / 60.0 + t.m_Hour) / 24.0;
}
