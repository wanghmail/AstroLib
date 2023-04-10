#pragma once
#include "AsOrbitParam.h"
#include "AsCoordinate.h"
#include "AsMath.h"


//********************************************************************
/// �����������ת��Ϊλ���ٶ�/modified orbit element to cartesian state element.
/// @Author	Zhang Yu
/// @Date	2023/4/2
/// @Input
/// @Param	modOrb	�����������
/// @Param	gm		��������������
/// @Output
/// @Param	pos		λ��
/// @Param	vel		�ٶ�
/// @Return			true=�ɹ�; false=�������
//********************************************************************
bool	AsModOrbElemToCart(
	const CModOrbElem& modOrb,
	double gm,
	CCoord& pos,
	CCoord& vel);