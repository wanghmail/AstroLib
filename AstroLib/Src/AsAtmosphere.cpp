////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2000-2021                         //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsAtmosphere.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsAtmosphere.h"
#include "AsMath.h"
#include <cmath>


using namespace std;



//********************************************************************
//returns the density based on the exponential model. Inputs
//  include the reference density, reference alt, scale alt,
//  and the altitude of interest. The exponent is the expression
//  ((reference alt - altitude)/scale alt).
/// @Author	Wang Hua
/// @Date	2007.1.22
/// @Input	refDensity		
//			refHeight
//			scaleHeight
//			alt
/// @Return	Density                  kg/m^3
//********************************************************************
double AsExponentialAtmDens (
			double refDensity,
			double refHeight,
			double scaleHeight,
			double alt)
{
	return refDensity * exp((refHeight - alt) / scaleHeight);
}


//********************************************************************
/// 采用公式拟合GB1920-80标准大气，计算大气密度、压强和温度
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
void AsGB1980FitAtm(double alt, 
            double& density, double& pressure, 
			double& temperature, double& sonicSpeed)
{
	const double G0		= 9.797645;
    const double r0     = 6.356766e+06;	//(kilometer)
	const double rousl	= 1.2250;		//零高度标准密度  （单位：千克/立方米）
	const double psl	= G0*1.0332e4;	//零高度标准压强  （单位：Pa ）	101325.0;

    double w;

    //处理高度小于0的情况
    if(alt<0.0) alt = 0.0;

	//《远程火箭弹道学》P39,41
	double height=alt/(1.0+alt/r0);
	
	if(alt>=0.0&&alt<=11019.1)
	{
		w=1.0-height/44330.8;
		temperature = 288.15*w;
		density=pow(w,4.2559);
		pressure=psl*density*w;
		density=rousl*density;
	}
	else if(alt<=20063.1)
	{
		//张海联 14:48 2003-3-31，修改公里和米的换算
		//w=exp((14.9647-height)/6.3416);
		w=exp((14964.7-height)/6341.6);
		temperature = 216.65;
		pressure=psl*0.11953*w;
		density=rousl*0.15898*w;				//将0.1598变为0.15898
	}
	else if(alt<=32161.9)
	{
		w=1.0+(height-24902.1)/221552.0;
		temperature = 221.552*w;
		density=pow(w,(-35.1629));
		pressure=psl*2.5158e-02*density*w;
		density=rousl*3.2722e-02*density;
	}
	else if(alt<=47350.1)
	{
		w=1.0+(height-39749.9)/89410.7;
		temperature = 250.350*w;
		density=pow(w,(-13.2011));                 //修改，先计算 pow(w,(-13.2011))
		pressure=psl*2.8338e-03*density*w;            //王华，2009-11-3，从2.8388改为2.8338
		density=rousl*3.2618E-03*density;
	}
	else if(alt<=51412.5)
	{
		w=exp((48625.2-height)/7922.3);
		temperature = 270.650;
		pressure=psl*8.9155e-4*w;                      //修改：8.9155e-4
		density=rousl*9.4920e-04*w;
	}
	else if(alt<=71802.0)
	{
		w=1.0-(height-59439.0)/88221.8;
		temperature = 247.021*w;
		density=pow(w,11.2011);
		pressure=psl*2.1671e-04*density*w;
		density=rousl*2.5280e-04*density;
	}
	else if(alt<=86000.0)
	{
		w=1.0-(height-78030.3)/100295.0;
		temperature = 200.590*w;
		density=pow(w,16.0816);
		pressure=psl*1.2274e-05*density*w;
		density=rousl*1.7632e-05*density;
	}
	else if(alt<=91000.0)
	{
		w=exp((87284.8-height)/5470.0);
		temperature = 186.870;
		pressure=psl*(2.2730+(1.042e-3)*height)*(1.0e-6)*w;
		density=rousl*3.6411e-06*w;
	}
	else
	{
		density = 0;
		pressure = 0;
		temperature = 186.870;
	}

	sonicSpeed=20.0468*AsSqrt(temperature);
}
