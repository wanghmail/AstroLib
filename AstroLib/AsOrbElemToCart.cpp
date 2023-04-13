///***********************************************************************
/// 轨道六根数变换位置速度
/// @Author	Guan Yuming
/// @Date	2023.04.06
/// @Input
/// @Param  m_SMajAx;           长半轴 a
/// @Param  m_Ecc;              偏心率 e
/// @Param  m_I;                轨道倾角 i
/// @Param  m_RAAN;             升交点赤经 Omega
/// @Param  m_ArgPeri;          近供点角 omega
/// @Param  m_TrueA;            真近点角 theta
/// @Param  m_p;                半通径
/// @Param  m_r;                地心坐标系中，中心天体-第二天体位置矢量
/// @Param  m_v;                地心坐标系中, 速度矢量 
/// @Param  mtx_1,mtx_2,mtx_3   3个坐标变换矩阵
/// @Param  m1,m2               中心天体m1质量，第二天体质量m2
/// @Output
/// 在参考坐标系下，物体坐标为(r*cos(trueA), r*sin(trueA), 0)
/// 统一单位 m kg rad double earth_m1 = 5.97237e24, air_m2 = 8500.0
///***********************************************************************
#include <iostream>
#include <stdlib.h>
#include "AstroLib.h"
#include <cmath>
#include "AsOrbElemToCart.h"
#define G 6.67e-11 
using namespace std;
bool AsOrbElemToCart(const COrbElem& Orb, double m1, double m2, CCoord& cartPos, CCoord & cartVel)
{
	//异常状态
	if (
		//质量异常
		m1 <= 0 || m2 <= 0 || 
		//半长轴异常
		Orb.m_SMajAx <= 6371e3 || 
		//偏心率异常
		Orb.m_Ecc < 0 || 
		//轨道倾角异常
		Orb.m_I<0 ||Orb.m_I>AsCPI || 
		//升交点赤经异常
		Orb.m_RAAN < -AsCPI || Orb.m_RAAN > 2 * AsCPI ||
		//近供点角异常
		Orb.m_ArgPeri < 0 || Orb.m_ArgPeri> 2 * AsCPI || 
		//真近点角异常
		Orb.m_TrueA < -AsCPI || Orb.m_TrueA> 2 * AsCPI ||
		//角度关系异常
	   (Orb.m_Ecc >= 1 && Orb.m_TrueA >= AsCPI - acos(1.0 / Orb.m_Ecc) &&
		Orb.m_TrueA <= AsCPI + acos(1.0 / Orb.m_Ecc))
		)
	{
		cout << "One of the six elements or m1 or m2 is wrong" << endl;
		return false;
	}
	else
	{
		// 位置矢量和半通径
		double  r, m_p;
		//根据ecc判断轨道形状,决定半通径取值
		if (Orb.m_Ecc == 0 || Orb.m_Ecc == 1) 
			m_p = Orb.m_SMajAx;
		else if (Orb.m_Ecc > 1) 
			m_p = -Orb.m_SMajAx*(1 - Orb.m_Ecc*Orb.m_Ecc);
		else 
			m_p = Orb.m_SMajAx*(1 - Orb.m_Ecc*Orb.m_Ecc);
		//位置矢量和半通径关系
		r = m_p / (1 + Orb.m_Ecc*cos(Orb.m_TrueA));  
		/*
		//第二天体轨道面-中心天体轨道面，需进行3次坐标旋转变换
		//第一次按3 旋转 -raan
		CMatrix<double> mtx_1(cos(-Orb.m_RAAN), sin(-Orb.m_RAAN), 0,
		                     -sin(-Orb.m_RAAN), cos(-Orb.m_RAAN), 0,
		                                                  0, 0, 1.0);
		//第二次按1 旋转-i
		CMatrix<double> mtx_2(1.0, 0, 0,
		                      0, cos(-Orb.m_I), sin(-Orb.m_I),
		                      0, -sin(-Orb.m_I), cos(-Orb.m_I));
		//第三次旋转按3 旋转-argPeri
		CMatrix<double> mtx_3(cos(-Orb.m_ArgPeri), sin(-Orb.m_ArgPeri), 0,
		                     -sin(-Orb.m_ArgPeri), cos(-Orb.m_ArgPeri), 0,
		                                                        0, 0, 1.0);
		CMatrix<double> R(3, 1);//初始位置矢量R
		R[0][0] = r*cos(Orb.m_TrueA);
		R[1][0] = r*sin(Orb.m_TrueA);
		R[2][0] = 0;
		*/
		//位置,同时换算为km
		cartPos[0] = -1*r*(cos(Orb.m_RAAN)*cos(Orb.m_ArgPeri + Orb.m_TrueA) - sin(Orb.m_RAAN)*sin(Orb.m_ArgPeri + Orb.m_TrueA)*cos(Orb.m_I))/1000;
		cartPos[1] = -1*r*(sin(Orb.m_RAAN)*cos(Orb.m_ArgPeri + Orb.m_TrueA) + cos(Orb.m_RAAN)*sin(Orb.m_ArgPeri + Orb.m_TrueA)*cos(Orb.m_I)) / 1000;
		cartPos[2] = -1*r*sin(Orb.m_RAAN + Orb.m_ArgPeri)*sin(Orb.m_I) / 1000;
		//速度
		//由于运动中除了trueA，其余轨道根数均不变，且轨道六根数可唯一确定位置矢量，因此位置矢量是trueA的函数。
		//v=(dm_r/dtrueA)*(dtrueA/dt)
		double miu;
		miu = G*(m1 + m2);
		//m/s换算为km/s
		cartVel[0] = (-cos(Orb.m_RAAN)*(sin(Orb.m_ArgPeri + Orb.m_TrueA) + Orb.m_Ecc*sin(Orb.m_ArgPeri)) -
			sin(Orb.m_RAAN)*(cos(Orb.m_ArgPeri + Orb.m_TrueA) +
				Orb.m_Ecc*cos(Orb.m_ArgPeri))*cos(Orb.m_I))*sqrt(miu / m_p)/1000*(-1);
		cartVel[1] = (-sin(Orb.m_RAAN)*(sin(Orb.m_ArgPeri + Orb.m_TrueA) + Orb.m_Ecc*sin(Orb.m_ArgPeri)) +
			cos(Orb.m_RAAN)*(cos(Orb.m_ArgPeri + Orb.m_TrueA) +
				Orb.m_Ecc*cos(Orb.m_ArgPeri))*cos(Orb.m_I))*sqrt(miu / m_p) / 1000 * (-1);
		cartVel[2] = ((cos(Orb.m_ArgPeri + Orb.m_TrueA) +
			Orb.m_Ecc*cos(Orb.m_ArgPeri))*sin(Orb.m_I))*sqrt(miu / m_p) / 1000 * (-1);
		return	true;
	}
}

