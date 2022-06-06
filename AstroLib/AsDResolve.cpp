////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2006                          //
//                              Wang Hua                                  //
//             College of Aerospace and Material Engineering              //
//               National University of Defense Technology                //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsDResolve.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsDResolve.h"
#include "AsMath.h"
#include <iostream>


using namespace std;
AsMUsingNamespace


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif






//*********************************************************************
//Fourth-order Runge-Kutta method.
//Author:	Wang Hua
//Data:		2005.1
//Input:	func	user-supplied class that computes the right-hand side derivatives.
//			step	stepsize
//			t		starting value of the indepentent variable.
//In/Out:	x		input dependent variable at the starting. output new value 
//					of dependent variable.
//Output:	
//*********************************************************************
void AsMNamespace AsODERungeKutta4(
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


