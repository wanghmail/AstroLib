////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsRightFunction.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ASRIGHTFUNCTION_H_)
#define _ASRIGHTFUNCTION_H_


#include "AsCommon.h"
#include "AsVector.h"





//
//右函数类，用于其它右函数计算类的基类
//
class CRightFunc
{
public:
	CRightFunc(){};
	virtual ~CRightFunc(){};

	/// @Input
	/// @Param	t		自变量的值
	/// @Param	x		初始函数值
	/// @Output
	/// @Param	result	计算得到的函数值
	virtual	void operator() (double t, const CVector<double>& x, CVector<double>& result) const = 0;

};


//
//右函数类，用于其它右函数计算类的基类
//double * version
//
class AsTRightFunc
{
public:
	AsTRightFunc(){};
	virtual ~AsTRightFunc(){};

	/// @Input
	/// @Param	t		自变量的值
	/// @Param	x		初始函数值
	/// @Param	n		初始函数值的个数
	/// @Output
	/// @Param	result	计算得到的函数值
	virtual	void operator() (double t, const double* x, int n, double* result) const = 0;

};





#endif // !defined(_ASRIGHTFUNCTION_H_)
