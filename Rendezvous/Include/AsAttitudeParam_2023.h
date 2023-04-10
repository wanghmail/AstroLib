#pragma once

#include "AsCoordinate.h"
#include "AsMath.h"
#include "AsAttitudeParam.h"




///***********************************************************************
/// The conversion from matrix to axis and angle.
/// @Author	Xiao Yao
/// @Date	2023.4.2
/// @Input
/// @Param	mtx	    ?
/// @Output
/// @Param	axis	??
/// @Param	angle	?[0, pi]
///***********************************************************************
void AsMtxToAxAng(const CMatrix<double>& mtx,
	CCoord& axis, double& angle);



//************************************************
//*将四元数转化成欧拉角_123转序（欧拉角单位为度）*
//************************************************
//输入：
//		标准化四元数：q
//		代求欧拉角：  Ang
//		卫星类型选择：cho (cho=1：自旋卫星；cho=3：非自旋卫星)
//************************************************
bool AsQuatToEuler_123(const CQuaternion& q, CEuler& Ang, const int cho);