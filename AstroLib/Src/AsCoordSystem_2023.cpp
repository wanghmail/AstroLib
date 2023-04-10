////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2006                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsCoordSystem_2023.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsCoordSystem_2023.h"


using namespace std;


//********************************************************************
/// �ع�ϵ����������ϵ��ת������
/// ��������ϵ��x��ƽ�е���ָ���䷽��,y��ֱ����Ǧ������,z������ֱ������ϵ
/// @author	fangxuankun
/// @Date	2023.4.5
/// @Input
/// @Param	A0			���䷽λ��A0������Ϊ���䷽���뵱�������ļнǣ�˳ʱ�����Ϊ����rad��
/// @Param	Lon	        �����ľ��ȣ�rad��
/// @Param	Lan	        ������γ�ȣ�rad��
/// @Output
/// @Param	mtx			�ع�ϵ����������ϵ��ת������
//********************************************************************
void	AsCBFToLCMtx(double A0,double Lon,double Lat,CMatrix<double>& mtx)
{
	double a, b, c;
	a = Lon - AsCHalfPI;
	b = Lat;
	c = -A0 - AsCHalfPI;

	CMatrix<double>Mx(1, 0, 0, 0, cos(b), sin(b), 0, -sin(b), cos(b));
	CMatrix<double>My(cos(c), 0, -sin(c), 0, 1, 0, sin(c), 0, cos(c));
	CMatrix<double>Mz(cos(a), sin(a), 0, -sin(a), cos(a), 0, 0, 0, 1);
	CMatrix<double>M = My*Mx*Mz;
	mtx = M;
}
