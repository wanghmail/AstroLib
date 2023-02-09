////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2006                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsDResolve.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsDResolve.h"
#include "AsMath.h"
#include <iostream>


using namespace std;



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
void  AsODERungeKutta4(
			const CRightFunc& func, 
			double step, 
			double t, 
			CVector<double> &x)
{
	int n=x.GetSize();
	CVector<double> dxm(n),dxdt(n);

	//First step.
	func(t,x,dxm);

	//Second step.
	double hh = step*0.5;
	double th = t+hh;
	func(th,x+hh*dxm,dxdt);

	//Third step.
	dxm += dxdt+dxdt;
	func(th,x+hh*dxdt,dxdt);

	//Fourth step.
	dxm += dxdt+dxdt;
	func(t+step,x+step*dxdt,dxdt);

	//Accumulate increments with proper weights.
	x += (dxdt+dxm)*step/6.0;
}


