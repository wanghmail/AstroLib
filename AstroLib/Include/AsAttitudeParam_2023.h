#pragma once

#include "AsCoordinate.h"
#include "AsMath.h"
#include "AsAttitudeParam.h"




///***********************************************************************
/// The conversion from matrix to axis and angle.
/// @Author	Xiao Yao
/// @Date	2023.4.2
/// @Input
/// @Param	mtx	    ����ת������
/// @Output
/// @Param	axis	��ת��ʸ��
/// @Param	angle	��ת��[0, pi]
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


///***********************************************************************
/// 方向余弦阵转321转序欧拉角
/// @Author	Li Sibei
/// @Date	2023.04.06
/// @Input
/// @Param	mtx		方向余弦阵
/// @Output
/// @Param	euler	欧拉角
///***********************************************************************
void AsMtxToEuler321(
	const CMatrix<double>& mtx, 
	CEuler& euler);