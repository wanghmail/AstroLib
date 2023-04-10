////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2006                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsOrbitParam_2023.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsOrbitParam_2023.h"



using namespace std;


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
bool AsModOrbElemToCart(const CModOrbElem& modOrb, double gm, CCoord& pos, CCoord& vel)
{
	double m_PeriRad = modOrb.m_PeriRad;		///<������뾶 periapsis radius in m   
	double m_Ecc = modOrb.m_Ecc;				///<ƫ���� eccentricity   
	double m_I = modOrb.m_I;					///<������ inclination in radians 
	double m_RAAN = modOrb.m_RAAN;				///<������ྭ right ascension of ascending node  
	double m_ArgPeri = modOrb.m_ArgPeri;		///<������� arg of periapsis in rad   
	double m_TrueA = modOrb.m_TrueA;			///<������ true anomaly in radians   
	if (gm <= 0 || m_PeriRad <= 0 || m_Ecc < 0 || m_I<0 || m_I>AsCPI || m_RAAN < -AsCPI || m_RAAN>2 * AsCPI || m_ArgPeri < 0 || m_ArgPeri>2 * AsCPI || m_TrueA < -AsCPI || m_TrueA>2 * AsCPI||(m_Ecc>=1&& m_TrueA>=AsCPI-acos(1.0/m_Ecc)&& m_TrueA<= AsCPI + acos(1.0 / m_Ecc)))
	{
		return 0;
	}
	else {
		double p = m_PeriRad * (1 + m_Ecc);		///<��ͨ�� 
		double miudh = sqrt(gm / p);			///<����

		double sinf = sin(m_TrueA);
		double cosf = cos(m_TrueA);
		double cosi = cos(m_I);
		double sini = sin(m_I);
		double cosAr = cos(m_ArgPeri);
		double sinAr = sin(m_ArgPeri);
		double cosRA = cos(m_RAAN);
		double sinRA = sin(m_RAAN);
		double cosRAsinAr = cosRA * sinAr;
		double sinRAsinAr = sinRA * sinAr;
		double cosRAcosAr = cosRA * cosAr;
		double sinRAcosAr = sinRA * cosAr;

		double r = p / (1 + m_Ecc * cosf);		///<���ľ��С
		double rcosf = r * cosf;
		double rsinf = r * sinf;

		double iex = cosRAcosAr - sinRAsinAr * cosi;	
		double iey = sinRAcosAr + cosRAsinAr * cosi;
		double iez = sini * sinAr;
		double ipx = -cosRAsinAr - sinRAcosAr * cosi;
		double ipy = -sinRAsinAr + cosRAcosAr * cosi;
		double ipz = sini * cosAr;
		pos[0] = rcosf * iex + rsinf * ipx;
		pos[1] = rcosf * iey + rsinf * ipy;
		pos[2] = rcosf * iez + rsinf * ipz;
		vel[0] = -miudh * sinf * iex + miudh * (m_Ecc + cosf) * ipx;
		vel[1] = -miudh * sinf * iey + miudh * (m_Ecc + cosf) * ipy;
		vel[2] = -miudh * sinf * iez + miudh * (m_Ecc + cosf) * ipz;
		return 1;
	}
}