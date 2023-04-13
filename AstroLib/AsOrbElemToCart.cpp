///***********************************************************************
/// ����������任λ���ٶ�
/// @Author	Guan Yuming
/// @Date	2023.04.06
/// @Input
/// @Param  m_SMajAx;           ������ a
/// @Param  m_Ecc;              ƫ���� e
/// @Param  m_I;                ������ i
/// @Param  m_RAAN;             ������ྭ Omega
/// @Param  m_ArgPeri;          ������� omega
/// @Param  m_TrueA;            ������ theta
/// @Param  m_p;                ��ͨ��
/// @Param  m_r;                ��������ϵ�У���������-�ڶ�����λ��ʸ��
/// @Param  m_v;                ��������ϵ��, �ٶ�ʸ�� 
/// @Param  mtx_1,mtx_2,mtx_3   3������任����
/// @Param  m1,m2               ��������m1�������ڶ���������m2
/// @Output
/// �ڲο�����ϵ�£���������Ϊ(r*cos(trueA), r*sin(trueA), 0)
/// ͳһ��λ m kg rad double earth_m1 = 5.97237e24, air_m2 = 8500.0
///***********************************************************************
#include <iostream>
#include <stdlib.h>
#include "AstroLib.h"
#include <cmath>
#include "AsOrbElemToCart.h"
#define G 6.67e-11 
using namespace std;
bool AsOrbElemToCart(const COrbElem& Orb, double m1, double m2, CCoord& cartPos, CCoord & cartVel)
{
	//�쳣״̬
	if (
		//�����쳣
		m1 <= 0 || m2 <= 0 || 
		//�볤���쳣
		Orb.m_SMajAx <= 6371e3 || 
		//ƫ�����쳣
		Orb.m_Ecc < 0 || 
		//�������쳣
		Orb.m_I<0 ||Orb.m_I>AsCPI || 
		//������ྭ�쳣
		Orb.m_RAAN < -AsCPI || Orb.m_RAAN > 2 * AsCPI ||
		//��������쳣
		Orb.m_ArgPeri < 0 || Orb.m_ArgPeri> 2 * AsCPI || 
		//�������쳣
		Orb.m_TrueA < -AsCPI || Orb.m_TrueA> 2 * AsCPI ||
		//�Ƕȹ�ϵ�쳣
	   (Orb.m_Ecc >= 1 && Orb.m_TrueA >= AsCPI - acos(1.0 / Orb.m_Ecc) &&
		Orb.m_TrueA <= AsCPI + acos(1.0 / Orb.m_Ecc))
		)
	{
		cout << "One of the six elements or m1 or m2 is wrong" << endl;
		return false;
	}
	else
	{
		// λ��ʸ���Ͱ�ͨ��
		double  r, m_p;
		//����ecc�жϹ����״,������ͨ��ȡֵ
		if (Orb.m_Ecc == 0 || Orb.m_Ecc == 1) 
			m_p = Orb.m_SMajAx;
		else if (Orb.m_Ecc > 1) 
			m_p = -Orb.m_SMajAx*(1 - Orb.m_Ecc*Orb.m_Ecc);
		else 
			m_p = Orb.m_SMajAx*(1 - Orb.m_Ecc*Orb.m_Ecc);
		//λ��ʸ���Ͱ�ͨ����ϵ
		r = m_p / (1 + Orb.m_Ecc*cos(Orb.m_TrueA));  
		/*
		//�ڶ���������-�����������棬�����3��������ת�任
		//��һ�ΰ�3 ��ת -raan
		CMatrix<double> mtx_1(cos(-Orb.m_RAAN), sin(-Orb.m_RAAN), 0,
		                     -sin(-Orb.m_RAAN), cos(-Orb.m_RAAN), 0,
		                                                  0, 0, 1.0);
		//�ڶ��ΰ�1 ��ת-i
		CMatrix<double> mtx_2(1.0, 0, 0,
		                      0, cos(-Orb.m_I), sin(-Orb.m_I),
		                      0, -sin(-Orb.m_I), cos(-Orb.m_I));
		//��������ת��3 ��ת-argPeri
		CMatrix<double> mtx_3(cos(-Orb.m_ArgPeri), sin(-Orb.m_ArgPeri), 0,
		                     -sin(-Orb.m_ArgPeri), cos(-Orb.m_ArgPeri), 0,
		                                                        0, 0, 1.0);
		CMatrix<double> R(3, 1);//��ʼλ��ʸ��R
		R[0][0] = r*cos(Orb.m_TrueA);
		R[1][0] = r*sin(Orb.m_TrueA);
		R[2][0] = 0;
		*/
		//λ��,ͬʱ����Ϊkm
		cartPos[0] = -1*r*(cos(Orb.m_RAAN)*cos(Orb.m_ArgPeri + Orb.m_TrueA) - sin(Orb.m_RAAN)*sin(Orb.m_ArgPeri + Orb.m_TrueA)*cos(Orb.m_I))/1000;
		cartPos[1] = -1*r*(sin(Orb.m_RAAN)*cos(Orb.m_ArgPeri + Orb.m_TrueA) + cos(Orb.m_RAAN)*sin(Orb.m_ArgPeri + Orb.m_TrueA)*cos(Orb.m_I)) / 1000;
		cartPos[2] = -1*r*sin(Orb.m_RAAN + Orb.m_ArgPeri)*sin(Orb.m_I) / 1000;
		//�ٶ�
		//�����˶��г���trueA�����������������䣬�ҹ����������Ψһȷ��λ��ʸ�������λ��ʸ����trueA�ĺ�����
		//v=(dm_r/dtrueA)*(dtrueA/dt)
		double miu;
		miu = G*(m1 + m2);
		//m/s����Ϊkm/s
		cartVel[0] = (-cos(Orb.m_RAAN)*(sin(Orb.m_ArgPeri + Orb.m_TrueA) + Orb.m_Ecc*sin(Orb.m_ArgPeri)) -
			sin(Orb.m_RAAN)*(cos(Orb.m_ArgPeri + Orb.m_TrueA) +
				Orb.m_Ecc*cos(Orb.m_ArgPeri))*cos(Orb.m_I))*sqrt(miu / m_p)/1000*(-1);
		cartVel[1] = (-sin(Orb.m_RAAN)*(sin(Orb.m_ArgPeri + Orb.m_TrueA) + Orb.m_Ecc*sin(Orb.m_ArgPeri)) +
			cos(Orb.m_RAAN)*(cos(Orb.m_ArgPeri + Orb.m_TrueA) +
				Orb.m_Ecc*cos(Orb.m_ArgPeri))*cos(Orb.m_I))*sqrt(miu / m_p) / 1000 * (-1);
		cartVel[2] = ((cos(Orb.m_ArgPeri + Orb.m_TrueA) +
			Orb.m_Ecc*cos(Orb.m_ArgPeri))*sin(Orb.m_I))*sqrt(miu / m_p) / 1000 * (-1);
		return	true;
	}
}

