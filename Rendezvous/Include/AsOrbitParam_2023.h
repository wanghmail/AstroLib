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