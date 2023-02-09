////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2000-2021                         //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsAtmosphere.h
//
//////////////////////////////////////////////////////////////////////
/// @file
/// @brief	����ģ��ͷ�ļ�

#if !defined(ASATMOSPHERE_H_)
#define ASATMOSPHERE_H_


#include "AsCommon.h"




//====================================================================
//
// Grouping: ����ģ��ϵ�к���
//
//====================================================================

//********************************************************************
/// finds the atmospheric density using exponential atmosphere model.
///  returns the density based on the exponential model. 
///  refDensity * exp((refHeight - height) / scaleHeight);
/// @Author	Wang Hua
/// @Date	2007.1.22
/// @Input
/// @Param	refDensity		reference density
/// @Param	refHeight		reference height
/// @Param	scaleHeight		scale height
/// @Param	height			the altitude of interest
/// @Return					Density (kg/m^3)
//********************************************************************
double	AsExponentialAtmDens (
			double refDensity, 
			double refHeight, 									
			double scaleHeight, 
			double height);

//********************************************************************
/// ���ù�ʽ���GB1920-80��׼��������������ܶȡ�ѹǿ���¶�
/// <<Զ�̻������ѧ>> P41
/// @Author	�ź��� 2002-10-17�� Wang Hua 2009-11-3
/// @Date	2009-11-3
/// @Input	
/// @Param	alt			���θ߶ȣ��ף�
/// @Output	
/// @Param	density     �����ܶȣ���λ��ǧ��/�����ף�
/// @Param	pressure    ����ѹ������λ������
/// @Param	temperature �¶�    ����λ��K��
/// @Param	sonicSpeed	����    ����λ��m/s��
/// @Return				
//********************************************************************
void    AsGB1980FitAtm(
            double alt, 
            double& density, 
            double& pressure, 
            double& temperature,
			double& sonicSpeed);





#endif // !defined(_ASATMOSPHERE_H_)
