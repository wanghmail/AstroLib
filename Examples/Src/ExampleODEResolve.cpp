// ExampleODEResolve.cpp: implementation of the CExampleODEResolve class.
//
//////////////////////////////////////////////////////////////////////

#include "AstroLibExamples.h"
#include "ExampleODEResolve.h"



//********************************************************************
/// 常微分方程组初值问题求解例子.
/// @Author	Wang Hua
/// @Date	2009.4.9
//********************************************************************
void ExampleODEResolve()
{
	//定义描述常微分方程组的右函数类
	//这里的常微分方程为描述物体自由下落函数
	// dx/dt=v
	// dv/dt=a
	// a=9.8
	class CMyRightFunc : public CRightFunc 
	{
	public:
		/// @Input
		/// @Param	t		自变量的值
		/// @Param	x		初始函数值,x[0]为位置,x[1]为速度
		/// @Output
		/// @Param	result	计算得到的函数值
		virtual	void operator() (double t, const CVector<double>& x, CVector<double>& result) const
		{
			result[0] = x[1];
			result[1] = -9.8;
		}		
	};
	
	CMyRightFunc func;
	CVector<double> x(2);

	x[0]=1000;	//初始位置1000m
	x[1]=0;		//初始速度0m/s

	//求解常微分方程，积分步长取1s，积分总时间为10s
	for (int i=0; i<10; i++)
		AsODERungeKutta4 (func, 1, 0, x);

    printf("The final position is %f (m)\n", x[0]);
}

