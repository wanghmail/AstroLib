////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2006                          //
//                              Wang Hua                                  //
//             College of Aerospace and Material Engineering              //
//               National University of Defense Technology                //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsInterpolation.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsInterpolation.h"
#include "AsVector.h"
#include "AsMatrix.h"
#include <iostream>


using namespace std;
AsMUsingNamespace


//*********************************************************************
//Hermite interpolation.
//Author:	Wang Hua
//Input:	x		the point of interpolation
//			x0
//			y0		x0处的函数值
//			x1
//			y1		x1处的函数值
//			p0		x0处的导数
//			p1		x1处的导数
//Output:	y		the result of interpolation		
//*********************************************************************
void AsMNamespace AsInterpHermite(double x0, double x1, double y0, double y1, 
								  double p0, double p1, double x, double& y)
{
	//<<计算方法引论>>

	assert(x0 != x1);

	double h0, h1, H0, H1, m0, m1, m02, m12;

	m0  = (x-x0) / (x1-x0);
	m1  = (x-x1) / (x0-x1);
	m02 = m0*m0;
	m12 = m1*m1;
	h0  = (1.0+m0+m0) * m12;
	h1  = (1.0+m1+m1) * m02;
	H0  = (x-x0) * m12;
	H1  = (x-x1) * m02;
	y   = y0*h0 + y1*h1 + p0*H0 + p1*H1;
}


//*********************************************************************
//linearly interpolates between one dimensional points (x0,y0) 
//   and (x1,y1). The input x is the point for which to 
//   interpolate, and the interpolated value is returned in the 
//   pointer to y.
//Author:	Wang Hua
//Date:		2006.1.22
//Input:	x		the point of interpolation
//			x0
//			y0		x0处的函数值
//			x1
//			y1		x1处的函数值
//Output:	y		the result of interpolation		
//*********************************************************************
void AsMNamespace AsInterp1Ord (
			double x0, double x1, 
			double y0, double y1, 
			double x, double& y)
{
	assert(x0!=x1);

	y = (x-x0)*(y1-y0)/(x1-x0)+y0;
}


//*********************************************************************
//quadratically interpolates between three one dimension input 
//   points ((x0,y0),(x1,y1),(x2,y2)). The input x is the point 
//   for which to interpolate, and the output y is a pointer to 
//   the interpolated value.
//Author:	Wang Hua
//Date:		2006.1.22
//Output:	y		the result of interpolation		
//*********************************************************************
void AsMNamespace AsInterp2Ord (
			double x0, double x1, double x2, 
			double y0, double y1, double y2, 
			double x, double& y)
{
	assert(x0!=x1 || x0!=x2 || x1!=x2);

	double x12 = x1-x2;
	double x01 = x0-x1;
	double x02 = x0-x2;
	double xx1 = x-x1;
	double xx2 = x-x2;
	double xx0 = x-x0;
	y = y0*xx1*xx2/(x01*x02) + y1*xx0*xx2/(-x01*x12) + y2*xx0*xx1/(x02*x12);
}


