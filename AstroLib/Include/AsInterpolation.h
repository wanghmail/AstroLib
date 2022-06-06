////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsInterpolation.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ASINTERPOLATION_H_)
#define _ASINTERPOLATION_H_


#include "AsCommon.h"


AsMBeginNamespace


template<class>
class CVector;
template<class>
class CMatrix;


//
//Interpolation and extrapolation function.
//

//*********************************************************************
/// Hermite interpolation.
/// <<计算方法引论>>
/// @Author	Wang Hua
/// @Input
/// @Param	x		the point of interpolation
/// @Param	x0
/// @Param	y0		x0处的函数值
/// @Param	x1
/// @Param	y1		x1处的函数值
/// @Param	p0		x0处的导数
/// @Param	p1		x1处的导数
/// @Output
/// @Param	y		the result of interpolation		
//*********************************************************************
void	AsInterpHermite (
			double x0, 
			double x1, 
			double y0, 
			double y1, 
			double dy0, 
			double dy1, 
			double x, 
			double& y);

//*********************************************************************
/// linearly interpolates between one dimensional points (x0,y0) 
///   and (x1,y1). The input x is the point for which to 
///   interpolate, and the interpolated value is returned in the 
///   pointer to y.
///   一阶线性插值
/// @Author	Wang Hua
/// @Date	2006.1.22
/// @Input
/// @Param	x		the point of interpolation
/// @Param	x0
/// @Param	y0		x0处的函数值
/// @Param	x1
/// @Param	y1		x1处的函数值
/// @Output
/// @Param	y		the result of interpolation		
//*********************************************************************
void	AsInterp1Ord (
			double x0, 
			double x1, 
			double y0, 
			double y1, 
			double x, 
			double& y);

//*********************************************************************
///quadratically interpolates between three one dimension input 
///   points ((x0,y0),(x1,y1),(x2,y2)). The input x is the point 
///   for which to interpolate, and the output y is a pointer to 
///   the interpolated value.
///   二阶插值
/// @Author	Wang Hua
/// @Date	2006.1.22
/// @Output	
/// @Param	y		the result of interpolation		
//*********************************************************************
void	AsInterp2Ord (
			double x0, 
			double x1, 
			double x2, 
			double y0, 
			double y1, 
			double y2, 
			double x, 
			double& y);


AsMEndNamespace


#endif // !defined(_ASINTERPOLATION_H_)
