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
bool AsCartToModOrbElem(const CCoord3& pos, const CCoord3& vel, double gm, CModOrbElem& modOrb)
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
		modOrb.m_ArgPeri = AsCPI;		// Omag = 0
		aaa = ecc.Dot(pos) / rmag / eccm;
		if (abs(aaa) > 1)		// overflow
		{
			aaa = AsSign(aaa);
		}
		double trueanom = acos(aaa);
		if (pos.Dot(vel) < 0)
		{
			trueanom = AsCTwoPI - trueanom;
		}
		trueanom = fmod(trueanom, AsCTwoPI);
		modOrb.m_TrueA = trueanom;
		return 0;
	}
	else
	{
		//// inclination
		aaa = hrv[2] / hrvmag;
		if (abs(aaa) >= 1)
		{
			aaa = AsSign(aaa);
		}
		double i0 = acos(aaa);

		//// RAAN
		double raan0;
		if (fmod(i0, AsCPI) == 0 && Omag <= error)			// equatorial , undefined     only Omag == 0 
		{
			/// Omag == 0   ---   hrv = 0 or hrv = [0 0 1], i0 = 0, the latter
			raan0 = 0;
		}
		else
		{
			aaa = Ounit[0] / Omag;
			if (abs(aaa) >= 1)
			{
				aaa = AsSign(aaa);
			}
			raan0 = acos(aaa);
			if (Ounit[1] < 0)
			{
				raan0 = AsCTwoPI - raan0;
			}
		}

		//// argument of perigee
		//// true anomaly
		double omig, trueanom;
		if (eccm <= error && fmod(i0, AsCPI) == 0)		// circle  equatorial
		{
			// true longitude     真黄经
			aaa = pos[0] / rmag;
			if (abs(aaa) >= 1)
			{
				aaa = AsSign(aaa);
			}
			double truelon = acos(aaa);// -raan0;	
			if (pos[1] < 0)
			{
				truelon = AsCTwoPI - truelon;			// = raan0 + omig + trueanom  ,raan0 = 0;
			}
			// 逆行轨道 再用 2*pi 减一次
			if (i0 > AsCHalfPI)
			{
				truelon = AsCTwoPI - truelon;
			}
			omig = 0;
			trueanom = truelon;
		}
		else if (eccm <= error && fmod(i0, AsCPI) != 0)	// circle inclined
		{
			// argument of latitude 升交角距
			aaa = Ounit.Dot(pos) / rmag / Omag;
			if (abs(aaa) >= 1)
			{
				aaa = AsSign(aaa);
			}
			double uu = acos(aaa);						//- raan0;	//hrv = [0 0 1], i0 = 0 or pi,
			if (pos[2] < 0)
			{
				uu = AsCTwoPI - uu;			// =  omig + trueanom
			}
			omig = 0;
			trueanom = uu;
		}
		else if (eccm >= error && fmod(i0, AsCPI) == 0)	// 	false appearance     != 0
		{
			// longitude of periapsis,近日点经度   raan0 = 0;
			if (Omag <= error)			// fmod(i0, pi) ===== 0
			{
				aaa = ecc[0] / eccm;
				if (abs(aaa) >= 1)
				{
					aaa = AsSign(aaa);
				}
				double omigtrue = acos(aaa);
				if (ecc[1] < 0)
				{
					omigtrue = AsCTwoPI - omigtrue;		// = raan0 + omig,	raan0 = 0;
				}
				//!!!!!  逆行轨道 再用 2*pi 减一次
				if (i0 > AsCHalfPI)
				{
					omigtrue = AsCTwoPI - omigtrue;
				}
				omig = omigtrue;
			}
			else				// fmod(i0, pi) ≈≈≈≈ 0
			{
				aaa = Ounit.Dot(ecc) / Omag / eccm;
				if (abs(aaa) >= 1)
				{
					aaa = AsSign(aaa);
				}
				omig = acos(aaa);
				if (ecc[2] < 0)
				{
					omig = AsCTwoPI - omig;
				}
			}
			aaa = ecc.Dot(pos) / rmag / eccm;
			if (abs(aaa) > 1)		// overflow
			{
				aaa = AsSign(aaa);
			}
			trueanom = acos(aaa);
			if (pos.Dot(vel) < 0)
			{
				trueanom = AsCTwoPI - trueanom;
			}
		}
		else
		{
			if (Omag <= error)		// hrv = 0 or hrv = [0 0 1], i0 = 0, the latter
			{
				if (i0 < AsCHalfPI)
				{
					omig = 0;
				}
				else
				{
					omig = AsCPI;
				}
			}
			else
			{
				aaa = Ounit.Dot(ecc) / Omag / eccm;
				if (abs(aaa) >= 1)
				{
					aaa = AsSign(aaa);
				}
				omig = acos(aaa);
				if (ecc[2] < 0)
				{
					omig = AsCTwoPI - omig;
				}
			}

			aaa = ecc.Dot(pos) / rmag / eccm;
			if (abs(aaa) > 1)		// overflow
			{
				aaa = AsSign(aaa);
			}
			trueanom = acos(aaa);
			if (pos.Dot(vel) < 0)
			{
				trueanom = AsCTwoPI - trueanom;
			}
		}
		// load orbital element vector
		raan0 = fmod(raan0, AsCTwoPI);
		omig = fmod(omig, AsCTwoPI);
		trueanom = fmod(trueanom, AsCTwoPI);
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


///***********************************************************************
/// 轨道六根数变换位置速度
/// 在参考坐标系下，物体坐标为(r*cos(trueA), r*sin(trueA), 0)
/// 统一单位 m kg rad
/// @Author	Guan Yuming
/// @Date	2023.04.06
/// @Input
/// @Param  Orb					轨道根数
/// @Param	gm					中心体引力常数
/// @Output
/// @Param  cartPos             地心坐标系中位置矢量 
/// @Param  cartVel				地心坐标系中速度矢量 
///***********************************************************************
bool AsOrbElemToCart(const COrbElem& Orb, double gm, CCoord& cartPos, CCoord & cartVel)
{
	//异常状态
	if (
		//质量异常
		gm <= 0 || 
		//半长轴异常
		Orb.m_SMajAx <= 6371e3 ||
		//偏心率异常
		Orb.m_Ecc < 0 ||
		//轨道倾角异常
		Orb.m_I<0 || Orb.m_I>AsCPI ||
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
		double  r, p;
		//根据ecc判断轨道形状,决定半通径取值
		if (Orb.m_Ecc == 0 || Orb.m_Ecc == 1)
			p = Orb.m_SMajAx;
		else if (Orb.m_Ecc > 1)
			p = -Orb.m_SMajAx*(1 - Orb.m_Ecc*Orb.m_Ecc);
		else
			p = Orb.m_SMajAx*(1 - Orb.m_Ecc*Orb.m_Ecc);
		//位置矢量和半通径关系
		r = p / (1 + Orb.m_Ecc*cos(Orb.m_TrueA));
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
		//位置
		cartPos[0] = -1 * r*(cos(Orb.m_RAAN)*cos(Orb.m_ArgPeri + Orb.m_TrueA) - sin(Orb.m_RAAN)*sin(Orb.m_ArgPeri + Orb.m_TrueA)*cos(Orb.m_I));
		cartPos[1] = -1 * r*(sin(Orb.m_RAAN)*cos(Orb.m_ArgPeri + Orb.m_TrueA) + cos(Orb.m_RAAN)*sin(Orb.m_ArgPeri + Orb.m_TrueA)*cos(Orb.m_I));
		cartPos[2] = -1 * r*sin(Orb.m_RAAN + Orb.m_ArgPeri)*sin(Orb.m_I);
		//速度
		//由于运动中除了trueA，其余轨道根数均不变，且轨道六根数可唯一确定位置矢量，因此位置矢量是trueA的函数。
		//v=(dm_r/dtrueA)*(dtrueA/dt)
		cartVel[0] = (-cos(Orb.m_RAAN)*(sin(Orb.m_ArgPeri + Orb.m_TrueA) + Orb.m_Ecc*sin(Orb.m_ArgPeri)) -
			sin(Orb.m_RAAN)*(cos(Orb.m_ArgPeri + Orb.m_TrueA) +
				Orb.m_Ecc*cos(Orb.m_ArgPeri))*cos(Orb.m_I))*sqrt(gm / p) * (-1);
		cartVel[1] = (-sin(Orb.m_RAAN)*(sin(Orb.m_ArgPeri + Orb.m_TrueA) + Orb.m_Ecc*sin(Orb.m_ArgPeri)) +
			cos(Orb.m_RAAN)*(cos(Orb.m_ArgPeri + Orb.m_TrueA) +
				Orb.m_Ecc*cos(Orb.m_ArgPeri))*cos(Orb.m_I))*sqrt(gm / p) * (-1);
		cartVel[2] = ((cos(Orb.m_ArgPeri + Orb.m_TrueA) +
			Orb.m_Ecc*cos(Orb.m_ArgPeri))*sin(Orb.m_I))*sqrt(gm / p) * (-1);
		return	true;
	}
}

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
bool	AsCartToOrbElem(const CCoord& pos, const CCoord& vel, double gm, COrbElem& elem) {

	if (pos.Norm() == 0)
	{
		cout << " 位置输入值不合理!\n ";
		cout << "请重新输入!\n";
		system("PAUSE");
		return 0;
	}

	else if (vel.Norm() == 0)
	{
		cout << " 速度输入值不合理!\n ";
		cout << "请重新输入!\n";
		system("PAUSE");
		return 0;
	}
	CCoord h = pos.Cross(vel);//动量矩矢量：h=r×v
	double hh = h.Norm();//h的大小
	double i = acos(h[2] / hh);//轨道倾角
	CCoord e = 1.0 / gm * vel.Cross(h) - 1.0 / pos.Norm() * pos;//偏心率矢量
	double ee = e.Norm();//偏心率e大小
	double a = hh * hh / gm / (1 - ee * ee);//半长轴a
	double Omiga;//升交点赤经Ω
	if (h[1] == 0)
	{
		cout << " 为赤道轨道!\n ";
		cout << "无升交点!\n";
		cout << setprecision(15) << a << endl;
		cout << setprecision(15) << ee << endl;
		cout << setprecision(15) << i << endl;
		system("PAUSE");
		return 0;
	}
	else
		Omiga = atan(-h[0] / h[1]);
	if (h[0] > 0)
	{
		if (Omiga < 0)
			Omiga = AsCPI + Omiga;
	}
	if (h[0] < 0)
	{
		if (Omiga < 0)
			Omiga = 2 * AsCPI + Omiga;
		else if (Omiga > 0)
			Omiga = AsCPI + Omiga;
	}
	CCoord n(-h[1], h[0], 0);//升交点矢量
	double w = acos(n.Dot(e) / (ee * n.Norm()));//近地点幅角
	if (e[2] < 0)
	{
		w = -w;
		if (w < 0)
		{
			w = 2 * AsCPI + w;
		}
	}

	double temp1 = n.Dot(pos);
	double temp2 = n.Norm() * pos.Norm();
	double u = acos(temp1 / temp2);//升交点角距
	if (pos[2] < 0)
		u = 2 * AsCPI - u;
	double f = u - w;//真近点角
	if (f < 0)
		f = 2 * AsCPI + f;

	elem.m_SMajAx = a;//半长轴a，单位m
	elem.m_Ecc = ee;//偏心率e
	elem.m_I = i;//轨道倾角i，弧度
	elem.m_ArgPeri = w;//近地点纬度幅角w，弧度
	elem.m_RAAN = Omiga;//升交点赤经Ω，弧度
	elem.m_TrueA = f;//真近点角f，弧度
	return 1;
}
