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
//*����Ԫ��ת����ŷ����_123ת��ŷ���ǵ�λΪ�ȣ�*
//************************************************
//���룺
//		��׼����Ԫ����q
//		����ŷ���ǣ�  Ang
//		��������ѡ��cho (cho=1���������ǣ�cho=3������������)
//************************************************
bool AsQuatToEuler_123(const CQuaternion& q, CEuler& Ang, const int cho);