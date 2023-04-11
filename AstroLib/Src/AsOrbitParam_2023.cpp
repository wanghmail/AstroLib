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
bool AsModOrbElemToCart(const CModOrbElem& modOrb, double gm, CCoord& pos, CCoord& vel)
{
	double m_PeriRad = modOrb.m_PeriRad;		///<近拱点半径 periapsis radius in m   
	double m_Ecc = modOrb.m_Ecc;				///<偏心率 eccentricity   
	double m_I = modOrb.m_I;					///<轨道倾角 inclination in radians 
	double m_RAAN = modOrb.m_RAAN;				///<升交点赤经 right ascension of ascending node  
	double m_ArgPeri = modOrb.m_ArgPeri;		///<近拱点角 arg of periapsis in rad   
	double m_TrueA = modOrb.m_TrueA;			///<真近点角 true anomaly in radians   
	if (gm <= 0 || m_PeriRad <= 0 || m_Ecc < 0 || m_I<0 || m_I>AsCPI || m_RAAN < -AsCPI || m_RAAN>2 * AsCPI || m_ArgPeri < 0 || m_ArgPeri>2 * AsCPI || m_TrueA < -AsCPI || m_TrueA>2 * AsCPI||(m_Ecc>=1&& m_TrueA>=AsCPI-acos(1.0/m_Ecc)&& m_TrueA<= AsCPI + acos(1.0 / m_Ecc)))
	{
		return 0;
	}
	else {
		double p = m_PeriRad * (1 + m_Ecc);		///<半通径 
		double miudh = sqrt(gm / p);			///<常数

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

		double r = p / (1 + m_Ecc * cosf);		///<地心距大小
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

bool	AsCartToModOrbElem(const CCoord3& pos, const CCoord3& vel, double gm, CModOrbElem& modOrb)
{
	////// Vallado.Fundamentals of Astrodynamics and Applications
	if ((pos.Norm() == 0) || (vel.Norm() == 0) || (gm <= 0))
	{
		modOrb.m_PeriRad = 0.0;
		modOrb.m_Ecc = 0.0;
		modOrb.m_I = 0.0;
		modOrb.m_RAAN = 0.0;
		modOrb.m_ArgPeri = 0.0;
		modOrb.m_TrueA = 0.0;
		return 0;
	}

	double pi2 = 2.0 * pi;
	double pihalf = 0.5 * pi;
	double error = 1e-15;
	double aaa;

	CCoord3 hrv, ecc, Ounit;
	CCoord3 Kunit(0, 0, 1.0);
	double rmag, vmag, hrvmag, Omag;
	hrv = pos.Cross(vel);
	Ounit = Kunit.Cross(hrv);
	rmag = pos.Norm();
	vmag = vel.Norm();
	hrvmag = hrv.Norm();
	Omag = Ounit.Norm();

	double sma, eccm, energy;
	// eccentricity
	ecc = (vel / gm).Cross(hrv);
	ecc = ecc - pos / rmag;
	eccm = ecc.Norm();
	energy = pow(vmag, 2.0) / 2 - gm / rmag;
	// altitude of the perigee
	if (eccm != 1)
	{
		sma = -1 * gm / (2 * energy);
		modOrb.m_PeriRad = sma*(1 - eccm);
	}
	else			// parabola
	{
		modOrb.m_PeriRad = 0;
	}

	if (hrvmag <= error)		// rectilinear motion  =0
	{
		modOrb.m_PeriRad = 0;
		modOrb.m_Ecc = 1;
		modOrb.m_I = 0;
		modOrb.m_RAAN = 0;
		modOrb.m_ArgPeri = pi;		// Omag = 0
		aaa = ecc.Dot(pos) / rmag / eccm;
		if (abs(aaa) > 1)		// overflow
		{
			aaa = Sign(aaa);
		}
		double trueanom = acos(aaa);
		if (pos.Dot(vel) < 0)
		{
			trueanom = pi2 - trueanom;
		}
		trueanom = fmod(trueanom, pi2);
		modOrb.m_TrueA = trueanom;
		return 0;
	}
	else
	{
		//// inclination
		aaa = hrv[2] / hrvmag;
		if (abs(aaa) >= 1)
		{
			aaa = Sign(aaa);
		}
		double i0 = acos(aaa);
		
		//// RAAN
		double raan0;
		if (fmod(i0, pi) == 0 && Omag <= error)			// equatorial , undefined     only Omag == 0 
		{
			/// Omag == 0   ---   hrv = 0 or hrv = [0 0 1], i0 = 0, the latter
			raan0 = 0;	
		}
		else
		{
			aaa = Ounit[0] / Omag;
			if (abs(aaa) >= 1)
			{
				aaa = Sign(aaa);
			}
			raan0 = acos(aaa);
			if (Ounit[1] < 0)
			{
				raan0 = pi2 - raan0;
			}
		}

		//// argument of perigee
		//// true anomaly
		double omig, trueanom;
		if (eccm <= error && fmod(i0, pi) == 0)		// circle  equatorial
		{
			// true longitude     真黄经
			aaa = pos[0] / rmag;
			if (abs(aaa) >= 1)
			{
				aaa = Sign(aaa);
			}
			double truelon = acos(aaa);// -raan0;	
			if (pos[1] < 0)
			{
				truelon = pi2 - truelon;			// = raan0 + omig + trueanom  ,raan0 = 0;
			}
			// 逆行轨道 再用 2*pi 减一次
			if (i0 > pihalf)
			{
				truelon = pi2 - truelon;			
			}
			omig = 0;
			trueanom = truelon;
		}
		else if (eccm <= error && fmod(i0, pi) != 0)	// circle inclined
		{
			// argument of latitude 升交角距
			aaa = Ounit.Dot(pos) / rmag / Omag;
			if (abs(aaa) >= 1)
			{
				aaa = Sign(aaa);
			}
			double uu = acos(aaa);						//- raan0;	//hrv = [0 0 1], i0 = 0 or pi,
			if (pos[2] < 0)
			{
				uu = pi2 - uu;			// =  omig + trueanom
			}
			omig = 0;
			trueanom = uu;
		}
		else if (eccm >= error && fmod(i0, pi) == 0)	// 	false appearance     != 0
		{
			// longitude of periapsis,近日点经度   raan0 = 0;
			if (Omag <= error)			// fmod(i0, pi) ===== 0
			{
				aaa = ecc[0] / eccm;
				if (abs(aaa) >= 1)
				{
					aaa = Sign(aaa);
				}
				double omigtrue = acos(aaa);	
				if (ecc[1] < 0)
				{
					omigtrue = pi2 - omigtrue;		// = raan0 + omig,	raan0 = 0;
				}
				//!!!!!  逆行轨道 再用 2*pi 减一次
				if (i0 > pihalf)
				{
					omigtrue = pi2 - omigtrue;			
				}
				omig = omigtrue;
			}
			else				// fmod(i0, pi) ≈≈≈≈ 0
			{
				aaa = Ounit.Dot(ecc) / Omag / eccm;
				if (abs(aaa) >= 1)
				{
					aaa = Sign(aaa);
				}
				omig = acos(aaa);
				if (ecc[2] < 0)
				{
					omig = pi2 - omig;
				}
			}
			aaa = ecc.Dot(pos) / rmag / eccm;
			if (abs(aaa) > 1)		// overflow
			{
				aaa = Sign(aaa);
			}
			trueanom = acos(aaa);
			if (pos.Dot(vel) < 0)
			{
				trueanom = pi2 - trueanom;
			}
		}
		else
		{
			if (Omag <= error)		// hrv = 0 or hrv = [0 0 1], i0 = 0, the latter
			{
				if (i0 < pihalf)
				{
					omig = 0;
				}
				else
				{
					omig = pi;
				}
			}
			else
			{
				aaa = Ounit.Dot(ecc) / Omag / eccm;
				if (abs(aaa) >= 1)
				{
					aaa = Sign(aaa);
				}
				omig = acos(aaa);
				if (ecc[2] < 0)
				{
					omig = pi2 - omig;
				}
			}

			aaa = ecc.Dot(pos) / rmag / eccm;
			if (abs(aaa) > 1)		// overflow
			{
				aaa = Sign(aaa);
			}
			trueanom = acos(aaa);
			if (pos.Dot(vel) < 0)
			{
				trueanom = pi2 - trueanom;
			}
		}
		// load orbital element vector
		raan0 = fmod(raan0, pi2);
		omig = fmod(omig, pi2);
		trueanom = fmod(trueanom, pi2);
		if (eccm < error)
		{
			eccm = 0;
		}
		modOrb.m_Ecc = eccm;
		modOrb.m_I = i0;
		modOrb.m_RAAN = raan0;
		modOrb.m_ArgPeri = omig;
		modOrb.m_TrueA = trueanom;
		return 1;
	}
}