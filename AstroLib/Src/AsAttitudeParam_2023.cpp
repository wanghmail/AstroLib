
#include "AsAttitudeParam_2023.h"
#include "AsCoordinate.h"
#include "AsMath.h"
#include "AsAttitudeParam.h"
#include <iostream>


using namespace std;


///***********************************************************************
/// The conversion from matrix to axis and angle.
/// @Author	Xiao Yao
/// @Date	2023.4.4
/// @Input
/// @Param	mtx	    ����ת������
/// @Output
/// @Param	axis	��ת��ʸ��
/// @Param	angle	��ת��[0, pi]
///***********************************************************************
void AsMtxToAxAng(const CMatrix<double>& mtx,
	CCoord& axis, double& angle)
{

	CMatrix<double> mm = mtx;
	/************** �ж��Ƿ�Ϊ�������� ******************/
	double p1 = mm[0][0] * mm[0][1] + mm[1][0] * mm[1][1] + mm[2][0] * mm[2][1];
	double p2 = mm[0][0] * mm[0][2] + mm[1][0] * mm[1][2] + mm[2][0] * mm[2][2];
	double p3 = mm[0][2] * mm[0][1] + mm[1][2] * mm[1][1] + mm[2][2] * mm[2][1];
	double p4 = mm[0][0] * mm[0][0] + mm[1][0] * mm[1][0] + mm[2][0] * mm[2][0];
	double p5 = mm[0][1] * mm[0][1] + mm[1][1] * mm[1][1] + mm[2][1] * mm[2][1];
	double p6 = mm[0][2] * mm[0][2] + mm[1][2] * mm[1][2] + mm[2][2] * mm[2][2];
	if (!(abs(p1)<1e-9 && abs(p2)<1e-9 && abs(p3)<1e-9 && 
		  abs(p4-1)<1e-9 && abs(p5 - 1)<1e-9 && abs(p6 - 1)<1e-9) ){
		printf("warning! not Orthogonal Matrix\n");
		angle = 0;
		axis[0] = 0;
		axis[1] = 0;
		axis[2] = 0;
		return;
	}

	// ����ļ�
	double T = mm[0][0] + mm[1][1] + mm[2][2];

	if (abs(T - 3) > 1e-9 && abs(T + 1) > 1e-9) {
		double cc = 0.5 * (T - 1);   // cos(angle)
		double ss = sin(acos(cc));   // sin(angle)
		angle = atan2(ss, cc);       // (0, pi)
		axis[0] = 0.5 * (mm[1][2] - mm[2][1]) / ss;
		axis[1] = 0.5 * (mm[2][0] - mm[0][2]) / ss;
		axis[2] = 0.5 * (mm[0][1] - mm[1][0]) / ss;
	}
	else if (abs(T + 1) < 1e-9) {
		// ת���������⣬ָ�����180��,�ҽǶȲ�Ӱ��, ����������һ��
		// sin(angle) = 0�� angle=pi+2k*pi,kΪ����
		angle = AsCPI;

		double a12 = mm[0][1] / 2.0;
		double a23 = mm[1][2] / 2.0;
		double a31 = mm[2][0] / 2.0;
		double aa1 = sqrt((1.0 + mm[0][0]) / 2.0);
		double aa2 = sqrt((1.0 + mm[1][1]) / 2.0);
		double aa3 = sqrt((1.0 + mm[2][2]) / 2.0);
		if (a12 > 0 && a23 > 0 && a31 > 0) {
			// a1 a2 a3ͬ��
			axis[0] = aa1;
			axis[1] = aa2;
			axis[2] = aa3;
		}
		else if (a12 > 0) {
			// a1 a2 ͬ��
			axis[0] = aa1;
			axis[1] = aa2;
			axis[2] = -aa3;
		}
		else if (a23 > 0) {
			// a2 a3 ͬ��
			axis[0] = aa1;
			axis[1] = -aa2;
			axis[2] = -aa3;
		}
		else {
			// a1 a3ͬ��
			axis[0] = aa1;
			axis[1] = -aa2;
			axis[2] = aa3;
		}
	}
	else {
		// ת�᲻ȷ��
		angle = 0;
		axis[0] = 1;
		axis[1] = 0;
		axis[2] = 0;
	}
}


//************************************************
//*����Ԫ��ת����ŷ����_123ת��ŷ���ǵ�λΪ�ȣ�*
//************************************************
//���룺
//		��׼����Ԫ����q
//		����ŷ���ǣ�  Ang
//		��������ѡ��cho (cho=1���������ǣ�cho=3������������)
//************************************************
bool AsQuatToEuler_123(const CQuaternion& q, CEuler& Ang, const int cho)
{
	//����Ĭ�ϲ���
	double pi = acos(-1);   //����piֵ
	double deg = 180 / pi;    //����->�ȵ�ת��
	double epsilon = 1e-6;  //��ȷ��
	Ang = { 0, 0, 0 };
	//q0, q1, q2, q3�ֱ��Ӧq.m_Qs, q.m_Qx, q.m_Qy, q.m_Qz
	double q0q0 = q.m_Qs*q.m_Qs;
	double q1q1 = q.m_Qx*q.m_Qx;
	double q2q2 = q.m_Qy*q.m_Qy;
	double q3q3 = q.m_Qz*q.m_Qz;
	double q_norm2 = q0q0 + q1q1 + q2q2 + q3q3;
	if (abs(q_norm2 - 1) > epsilon)   //�ж��������Ԫ���Ƿ��׼��
	{
		cout << "�������׼����Ԫ��������" << endl;
		return false;
	}
	else if (abs(q0q0 - 1) < epsilon) {}   //û����תʱ��ֱ�����
	else
	{
		double q0q1 = q.m_Qs*q.m_Qx;
		double q0q2 = q.m_Qs*q.m_Qy;
		double q0q3 = q.m_Qs*q.m_Qz;
		double q1q2 = q.m_Qx*q.m_Qy;
		double q1q3 = q.m_Qx*q.m_Qz;
		double q2q3 = q.m_Qy*q.m_Qz;

		double a02 = q0q0 - q2q2;
		double a13 = q1q1 - q3q3;
		double sy = 2 * (q1q3 + q0q2);   //������ angle.y ������ֵ

										 //�ж��Ƿ�������죬����������ʱ���� Angle3��Angle1 Ϊ 0��ȷ��������Լ�������
		if (sy - 1 >= 0)   //�жϸ�����Ϊpi/2
		{
			Ang.m_Angle2 = 90;
			Ang.m_Angle1 = (3 - cho)*atan(q.m_Qx / q.m_Qy)*deg;
			Ang.m_Angle3 = (cho - 1)*atan(q.m_Qx / q.m_Qy)*deg;
		}
		else if (sy + 1 <= 0)   //�жϸ�����Ϊ-pi/2
		{
			Ang.m_Angle2 = -90;
			Ang.m_Angle1 = -(3 - cho)*atan(q.m_Qx / q.m_Qy)*deg;
			Ang.m_Angle3 = -(cho - 1)*atan(q.m_Qx / q.m_Qy)*deg;
		}
		else
		{
			Ang.m_Angle2 = asin(2 * (q1q3 + q0q2))*deg;
			Ang.m_Angle1 = atan2(2 * (q0q1 - q2q3), a02 - a13)*deg;
			Ang.m_Angle3 = atan2(2 * (q0q3 - q1q2), a02 + a13)*deg;
		}
	}
	return true;
};