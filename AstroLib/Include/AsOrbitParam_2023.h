#pragma once
#include "AsOrbitParam.h"
#include "AsCoordinate.h"
#include "AsMath.h"


//********************************************************************
/// 修正轨道根数转换为位置速度/modified orbit element to cartesian state element.
/// @Author	Zhang Yu
/// @Date	2023/4/2
/// @Input
/// @Param	modOrb	修正轨道根数
/// @Param	gm		中心体引力常数
/// @Output
/// @Param	pos		位置
/// @Param	vel		速度
/// @Return			true=成功; false=输入错误
//********************************************************************
bool	AsModOrbElemToCart(
	const CModOrbElem& modOrb,
	double gm,
	CCoord& pos,
	CCoord& vel);

//********************************************************************
/// cartesian state element to modified orbit element
/// @Author	Zhang Sai
/// @Date	2023.04.03
/// @Input
/// @Param	pos		位置
/// @Param	vel		速度
/// @Param	gm		中心体引力常数
/// @Output
/// @Param	modOrb	m_I			[0, pi]
///					m_RAAN		[0, 2pi]
///					m_ArgPeri	[0, 2pi]
///					m_TrueA		[0, 2pi]
/// @Return			true=成功; false=输入错误
//********************************************************************
bool	AsCartToModOrbElem(const CCoord3& pos, const CCoord3& vel, double gm, CModOrbElem& modOrb);

///***********************************************************************
/// 轨道六根数变换位置速度
/// 在参考坐标系下，物体坐标为(r*cos(trueA), r*sin(trueA), 0)
/// 统一单位 m kg rad
/// @Author	Guan Yuming
/// @Date	2023.04.06
/// @Input
/// @Param  Orb					轨道根数
/// @Output
/// @Param  cartPos;            地心坐标系中位置矢量 
/// @Param  cartVel				地心坐标系中速度矢量 
///***********************************************************************
bool	AsOrbElemToCart(const COrbElem& Orb, double gm, CCoord& cartPos, CCoord & cartVel);

//********************************************************************
/// 由位置坐标s.m_Pos和速度矢量s.m_Vel计算经典轨道根数
/// @Author	Zhang Zhongneng
/// @Date	2023.04.06
/// @Input
/// @Param	pos		位置
/// @Param	vel		速度
/// @Param	gm		中心体引力常数
/// @Output
/// @Param	elem	飞行器轨道根数
/// @Return			true=成功; false=输入错误
//********************************************************************
bool	AsCartToOrbElem(
	const CCoord& pos,
	const CCoord& vel,
	double gm,
	COrbElem& elem);
