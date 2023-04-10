////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2006                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsCoordSystem_2023.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsCoordSystem_2023.h"


using namespace std;
//********************************************************************
/// 地固系到发射坐标系的转换矩阵
/// 发射坐标系：x轴平行地面指向发射方向,y垂直地面铅锤向上,z成右手直角坐标系
/// @author	fangxuankun
/// @Date	2023.4.5
/// @Input
/// @Param	A0			发射方位角A0，定义为发射方向与当地正北的夹角，顺时针度量为正（rad）
/// @Param	Lon	        发射点的经度（rad）
/// @Param	Lan	        发射点的纬度（rad）
/// @Output
/// @Param	mtx			地固系到发射坐标系的转换矩阵
//********************************************************************
void	AsCBFToLCMtx(double A0,double Lon,double Lat,CMatrix<double>& mtx)
{
	double a, b, c;
	a = Lon - AsCHalfPI;
	b = Lat;
	c = -A0 - AsCHalfPI;

	CMatrix<double>Mx(1, 0, 0, 0, cos(b), sin(b), 0, -sin(b), cos(b));
	CMatrix<double>My(cos(c), 0, -sin(c), 0, 1, 0, sin(c), 0, cos(c));
	CMatrix<double>Mz(cos(a), sin(a), 0, -sin(a), cos(a), 0, 0, 0, 1);
	CMatrix<double>M = My*Mx*Mz;
	mtx = M;
}
