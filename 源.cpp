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
///***********************************************************************
#include <iostream>
#include <stdlib.h>
#include "AsVector.h"
#include "AsMatrix.h"
#include <cmath>
#define G 6.67e-11
using namespace std;
void AsOrb2RV(double sMajAx, double ecc, double i, double raan, double argPeri, double trueA,
	double m1,double m2)
{
	double m_r,r, m_p;// λ��ʸ���Ͱ�ͨ��
	if (ecc == 0 || ecc == 1) m_p = sMajAx;
	else if (ecc > 1) m_p = -sMajAx*(1 - ecc*ecc);
	else m_p = sMajAx*(1 - ecc*ecc);
	r = m_p / (1 + ecc*cos(trueA));  //λ��ʸ���Ͱ�ͨ����ϵ
	//�ڶ���������----�����������棬�����3��������ת�任
	//��һ�ΰ�3 ��ת -raan
	double pi = 3.1415926;
	CMatrix<double> mtx_1(cos(-raan), sin(-raan), 0,
			             -sin(-raan), cos(-raan), 0,
			                                0, 0, 1.0);
	//�ڶ��ΰ�1 ��ת-i
	CMatrix<double> mtx_2(1.0, 0, 0,
		                  0, cos(-i), sin(-i),
		                  0, -sin(-i), cos(-i));
	//��������ת��3 ��ת-argPeri
	CMatrix<double> mtx_3(cos(-argPeri), sin(-argPeri), 0,
		                 -sin(-argPeri), cos(-argPeri), 0,
		                                          0, 0, 1.0);
	//��ʼλ��ʸ��R
	//CVector<double> R(r*cos(trueA), r*sin(trueA),0); ��֪�����ת��-��-��
	CMatrix<double> R(3, 1);
	R[0] = r*cos(trueA);
	R[1] = r*sin(trueA);
	R[2] = 0;
	m_r = mtx_3*mtx_2*mtx_1*R;//��������ϵ�µ�λ��ʸ��
	//�ٶ�
	//�����˶��г���trueA�����������������䣬
	//�ҹ����������Ψһȷ��λ��ʸ�������λ��ʸ����trueA�ĺ�����
	//v=(dm_r/dtrueA)*(dtrueA/dt)
	double miu,m_v;
	miu = G*m1*m2;
	CMatrix<double> mtx_4(3, 1);
	mtx_4[0] = -cos(raan)*(sin(argPeri + trueA) + ecc*sin(argPeri)) - 
		sin(raan)*(cos(argPeri + trueA) +
		ecc*cos(argPeri))*cos(i);
	mtx_4[1] = -sin(raan)*(sin(argPeri + trueA) + ecc*sin(argPeri)) + 
		cos(raan)*(cos(argPeri + trueA) +
		ecc*cos(argPeri))*cos(i);
	mtx_4[2] = (cos(argPeri + trueA) + ecc*cos(argPeri))*sin(i);
	m_v = sqrt(miu / m_p)*mtx_4;
	cout << "λ��Ϊ��" << m_r << endl;
	cout << "�ٶ�Ϊ��" << m_p << endl;
}