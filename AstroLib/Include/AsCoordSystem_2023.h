#pragma once
#include"AsCoordinate.h"
//********************************************************************
/// 地固系到发射坐标系的转换矩阵
/// 发射坐标系：x轴平行地面指向发射方向,y垂直地面铅锤向上,z成右手直角坐标系
/// @author	fangxuankun
/// @Date	2023.4.5
/// @Input
/// @Param	A0			发射方位角A0，定义为发射方向与当地正北的夹角，顺时针度量为正
/// @Param	Lon	        发射点的经度
/// @Param	Lan	        发射点的纬度
/// @Output
/// @Param	mtx			地固系到发射坐标系的转换矩阵
//********************************************************************
void	AsCBFToLCMtx(
	double A0,
	double Lon,
	double Lat,
	CMatrix<double>& mtx);

