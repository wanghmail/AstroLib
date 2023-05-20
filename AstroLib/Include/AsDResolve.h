////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsDResolve.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ASDRESOLVE_H_)
#define _ASDRESOLVE_H_


#include "AsCommon.h"
#include "AsRightFunction.h"
#include "AsVector.h"
#include "AsMatrix.h"





//
//Fixed stepsize integration function of ordinary differential equations.
//


//*********************************************************************
/// Fourth-order Runge-Kutta method.
/// @Author	Wang Hua
/// @Date	2005.1
/// @Input	
/// @Param	func	user-supplied class that computes the right-hand side derivatives.
/// @Param	step	stepsize
/// @Param	t		starting value of the indepentent variable.
/// @In/Out	
/// @Param	x		input dependent variable at the starting. output new value 
//					of dependent variable.
/// @Output	
//*********************************************************************
void	AsODERungeKutta4 (
			const CRightFunc& func, 
			double step, 
			double t, 
			CVector<double>& x);






#endif // !defined(_ASDRESOLVE_H_)
