////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2000-2021                         //
//                              Wang Hua                                  //
//               National University of Defense Technology                //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsPlanetProp.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(ASPLANETPROP_H_)
#define ASPLANETPROP_H_


#include "AsCommon.h"
#include "AsMatrix.h"
#include <string>




class CCoord;



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
void	AsSunProp    (
			double julianDate, 
			CCoord& sunPos);



#endif // !defined(_ASPLANETPROP_H_)
