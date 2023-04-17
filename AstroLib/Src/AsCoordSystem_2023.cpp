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
	mtx = My*Mx*Mz;;
}


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
void AsICSToLVLHMtx(const CCoord& pos, const CCoord& vel, CMatrix<double>& mtx)
{
	CCoord h   =  pos* vel;
	CCoord e_z = h/h.Norm();
	CCoord e_x = pos/pos.Norm();
	CCoord e_y = e_z * e_x;

	mtx[0][0] = e_x[0]; mtx[0][1] = e_x[1]; mtx[0][2] = e_x[2];
	mtx[1][0] = e_y[0]; mtx[1][1] = e_y[1]; mtx[1][2] = e_y[2];
	mtx[2][0] = e_z[0]; mtx[2][1] = e_z[1]; mtx[2][2] = e_z[2];
}


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
void AsLVLHtoICSMtx(const CCoord3& pos, const CCoord3& vel, CMatrix<double> & mtx)
{
	double Temp2_idx_0;
	double Temp2_idx_1;
	double Temp2_idx_2;
	double Temp_idx_0;
	double Temp_idx_1;
	double Temp_idx_2;
	double norm_Temp;
	double norm_Temp2;
	double norm_pos;
	norm_pos = sqrt((pos[0] * pos[0] + pos[1] * pos[1]) + pos[2] * pos[2]);
	Temp_idx_0 = pos[1] * vel[2] - vel[1] * pos[2];
	Temp_idx_1 = vel[0] * pos[2] - pos[0] * vel[2];
	Temp_idx_2 = pos[0] * vel[1] - vel[0] * pos[1];
	norm_Temp = sqrt((Temp_idx_0 * Temp_idx_0 + Temp_idx_1 * Temp_idx_1) +
		Temp_idx_2 * Temp_idx_2);

	Temp2_idx_0 = Temp_idx_1 * pos[2] - pos[1] * Temp_idx_2;
	Temp2_idx_1 = pos[0] * Temp_idx_2 - Temp_idx_0 * pos[2];
	Temp2_idx_2 = Temp_idx_0 * pos[1] - pos[0] * Temp_idx_1;
	norm_Temp2 = sqrt((Temp2_idx_0 * Temp2_idx_0 + Temp2_idx_1 * Temp2_idx_1) +
		Temp2_idx_2 * Temp2_idx_2);

	if (norm_pos == 0)
	{
		mtx[0][0] = 0;
		mtx[1][0] = 0;
		mtx[2][0] = 0;
	}
	else
	{
		mtx[0][0] = pos[0] / norm_pos;
		mtx[1][0] = pos[1] / norm_pos;
		mtx[2][0] = pos[2] / norm_pos;
	}
	if (norm_Temp == 0)
	{
		mtx[0][2] = 0;
		mtx[1][2] = 0;
		mtx[2][2] = 0;
	}
	else
	{
		mtx[0][2] = Temp_idx_0 / norm_Temp;
		mtx[1][2] = Temp_idx_1 / norm_Temp;
		mtx[2][2] = Temp_idx_2 / norm_Temp;
	}
	if (norm_Temp2 == 0)
	{
		mtx[0][1] = 0;
		mtx[1][1] = 0;
		mtx[2][1] = 0;
	}
	else
	{
		mtx[0][1] = Temp2_idx_0 / norm_Temp2;
		mtx[1][1] = Temp2_idx_1 / norm_Temp2;
		mtx[2][1] = Temp2_idx_2 / norm_Temp2;
	}
}


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
void AsLVLHtoICS(const CCoord3& rSr, const CCoord3& rSv, 
	const CCoord3& RefSr, const CCoord3& RefSv, 
	CCoord3& S1r, CCoord3& S1v)
{
    CMatrix<double> mtx;
	CCoord3  S1pos;
	CCoord3  angle;
	double r2;
	AsLVLHtoICSMtx( RefSr, RefSv, mtx);
	S1pos = mtx * rSr;
	r2 = RefSr[0] * RefSr[0] + RefSr[1] * RefSr[1] + RefSr[2] * RefSr[2];
	angle = RefSr.Cross(RefSv)/r2;
	S1r = RefSr + S1pos;
	S1v = RefSv + mtx * rSv + angle.Cross(S1pos);
}

