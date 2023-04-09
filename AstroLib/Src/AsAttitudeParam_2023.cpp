
#include "AsAttitudeParam_2023.h"
#include "AsCoordinate.h"
#include "AsMath.h"
#include "AsAttitudeParam.h"


///***********************************************************************
/// The conversion from matrix to axis and angle.
/// @Author	Xiao Yao
/// @Date	2023.4.4
/// @Input
/// @Param	mtx	    坐标转换矩阵
/// @Output
/// @Param	axis	旋转轴矢量
/// @Param	angle	旋转角[0, pi]
///***********************************************************************
void AsMtxToAxAng(const CMatrix<double>& mtx,
	CCoord& axis, double& angle)
{

	CMatrix<double> mm = mtx;
	/************** 判断是否为正交矩阵 ******************/
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

	// 矩阵的迹
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
		// 转轴有两个解，指向相差180°,且角度不影响, 本程序仅输出一个
		// sin(angle) = 0， angle=pi+2k*pi,k为整数
		angle = AsCPI;

		double a12 = mm[0][1] / 2.0;
		double a23 = mm[1][2] / 2.0;
		double a31 = mm[2][0] / 2.0;
		double aa1 = sqrt((1.0 + mm[0][0]) / 2.0);
		double aa2 = sqrt((1.0 + mm[1][1]) / 2.0);
		double aa3 = sqrt((1.0 + mm[2][2]) / 2.0);
		if (a12 > 0 && a23 > 0 && a31 > 0) {
			// a1 a2 a3同号
			axis[0] = aa1;
			axis[1] = aa2;
			axis[2] = aa3;
		}
		else if (a12 > 0) {
			// a1 a2 同号
			axis[0] = aa1;
			axis[1] = aa2;
			axis[2] = -aa3;
		}
		else if (a23 > 0) {
			// a2 a3 同号
			axis[0] = aa1;
			axis[1] = -aa2;
			axis[2] = -aa3;
		}
		else {
			// a1 a3同号
			axis[0] = aa1;
			axis[1] = -aa2;
			axis[2] = aa3;
		}
	}
	else {
		// 转轴不确定
		angle = 0;
		axis[0] = 1;
		axis[1] = 0;
		axis[2] = 0;
	}
}