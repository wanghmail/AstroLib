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

///******************************************************
/// J2000系转到LVLH系转换矩阵
/// @author Zhou Ruiyi
/// @data   2023.4
/// @Version    1.0
/// @Input  
/// @Param  pos       位置
/// @Param  vel       速度
/// @Output
/// @Param  mtx       坐标转移矩阵
///******************************************************
void 	AsICSToLVLHMtx(const CCoord& pos, const CCoord& vel, CMatrix<double>& mtx);

//********************************************************************
/// LVLH到J2000的坐标转换矩阵
/// @Author	Duan Yu
/// @Date	2023/4/15
/// @Input :
/// @Param	pos			参考航天器位置
/// @Param	vel         参考航天器速度
/// @Output :
/// @Param	mtx		    LVLH到ICS的坐标转换矩阵
/// @Return			    true=成功; false=输入错误
//********************************************************************
void	AsLVLHtoICSMtx(const CCoord3& pos, const CCoord3& vel, CMatrix<double>& mtx);

//********************************************************************
/// LVLH到J2000的转换
/// @Author	Duan Yu
/// @Date	2023/4/15
/// @Input :
/// @Param	rSr         LVLH系目标航天器位置
/// @Param	rSv         LVLH系目标航天器速度
/// @Param	RefSr       J2000系参考航天器位置
/// @Param	RefSv       J2000系参考航天器速度
/// @Output :
/// @Param	S1r		    J2000系目标航天器位置
/// @Param	S1v		    J2000系目标航天器速度
/// @Return			    true=成功; false=输入错误
//********************************************************************
void	AsLVLHtoICS(const CCoord3& rSr, const CCoord3& rSv, 
	const CCoord3& RefSr, const CCoord3& RefSv, 
	CCoord3& S1r, CCoord3& S1v);
//********************************************************************
///柱坐标系到直角坐标系的位置速度转换
/// @Author	liuzhongbo
/// @Date	2023/6/13
/// @Input :
/// @Param	ppolar      柱坐标系航天器位置
/// @Param	vpolar         柱坐标系航天器速度
/// @Output :
/// @Param	pcart		   直角坐标系航天器位置
/// @Param	vcart		   直角坐标系航天器速度
//********************************************************************
void AsPolarToCart(const CPolar3 & ppolar, const CPolar3 & vpolar, CCoord& pcart, CCoord& vcart);

//********************************************************************
/// 直角坐标位置速度转换为柱坐标位置速度
/// @Author	Hou Xinyu
/// @Date	2023/6/13
/// @Input :
/// @Param	cart        直角坐标位置 
/// @Param	vcart       直角坐标速度
/// @Output :
/// @Param	polar		柱坐标位置
/// @Param	vpolar		柱坐标速度
//********************************************************************
void AsCartToPolar(const CCoord& cart, const CCoord& vcart,	CPolar3& polar, CPolar3& vpolar);