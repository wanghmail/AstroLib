////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2000-2021                         //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsPlanetProp.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsPlanetProp.h"
#include "AsCoordinate.h"
#include "AsMath.h"
#include "AsTimeSystem.h"
#include "AsAttitudeParam.h"
#include <string>


using namespace std;



//********************************************************************
/// 计算地心赤道坐标系中的太阳位置
/// calcuLates the Geocentric Equatorial position for the SUN given the Julian Date.
/// @Author	Wang Hua
/// @Date	2004.11.15
/// @Input
/// @Param	julianDate    Julian Date
/// @Output
/// @Param	sunPos        Geocentric Equatorial Position vector of the SUN	(meter)
/// @Return	
//********************************************************************
void AsSunProp(double JD, CCoord& RSun)
{
	//the distance between sun and earth. IAU1976
	const double AU = 1.4959787e11;

	double RSun4, MeanLong, MeanAnomaly, EclpLong, Obliquity, TUT1, TTDB;

	// Initialize values
	TUT1 = (JD - 2451545.0) / 36525.0;

	//Mean Longitude
	MeanLong = 280.4606184 + 36000.77005361 * TUT1;
	MeanLong = fmod(MeanLong, 360.0);  // deg

	TTDB        = TUT1;
	MeanAnomaly = 357.5277233 + 35999.05034 * TTDB;
	MeanAnomaly = AsDegToRad(MeanAnomaly);  // rad

	//Ecliptic Longitude
	EclpLong  = MeanLong + 1.914666471 * sin(MeanAnomaly) + 
			    0.019994643 * sin(2.0 * MeanAnomaly); // deg
	EclpLong  = AsDegToRad(EclpLong);

	//Mean Obliquity of the Ecliptic
	Obliquity = 23.439291 - 0.0130042 * TTDB;  // deg
	Obliquity = AsDegToRad(Obliquity);

	// Find magnitude of SUN vector, THEN components
	RSun4   = 1.000140612 - 0.016708617 * cos(MeanAnomaly) - 
			  0.000139589 * cos(2.0 * MeanAnomaly);   // in AU's
	RSun4   = RSun4 * AU;	//AU to meter
	RSun[0] = RSun4 * cos(EclpLong);
	RSun[1] = RSun4 * cos(Obliquity) * sin(EclpLong);
	RSun[2] = RSun4 * sin(Obliquity) * sin(EclpLong);
}

