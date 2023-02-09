////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2000-2021                         //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsAtmosphere.h
//
//////////////////////////////////////////////////////////////////////
/// @file
/// @brief	大气模型头文件

#if !defined(ASATMOSPHERE_H_)
#define ASATMOSPHERE_H_


#include "AsCommon.h"




//====================================================================
//
// Grouping: 大气模型系列函数
//
//====================================================================

//********************************************************************
/// finds the atmospheric density using exponential atmosphere model.
///  returns the density based on the exponential model. 
///  refDensity * exp((refHeight - height) / scaleHeight);
/// @Author	Wang Hua
/// @Date	2007.1.22
/// @Input
/// @Param	refDensity		reference density
/// @Param	refHeight		reference height
/// @Param	scaleHeight		scale height
/// @Param	height			the altitude of interest
/// @Return					Density (kg/m^3)
//********************************************************************
double	AsExponentialAtmDens (
			double refDensity, 
			double refHeight, 									
			double scaleHeight, 
			double height);

//********************************************************************
/// 采用公式拟合GB1920-80标准大气，计算大气密度、压强和温度
/// <<远程火箭弹道学>> P41
/// @Author	张海联 2002-10-17， Wang Hua 2009-11-3
/// @Date	2009-11-3
/// @Input	
/// @Param	alt			几何高度（米）
/// @Output	
/// @Param	density     大气密度（单位：千克/立方米）
/// @Param	pressure    大气压力（单位：帕）
/// @Param	temperature 温度    （单位：K）
/// @Param	sonicSpeed	音速    （单位：m/s）
/// @Return				
//********************************************************************
void    AsGB1980FitAtm(
            double alt, 
            double& density, 
            double& pressure, 
            double& temperature,
			double& sonicSpeed);





#endif // !defined(_ASATMOSPHERE_H_)
