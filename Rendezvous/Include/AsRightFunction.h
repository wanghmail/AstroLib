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
//�Һ����࣬���������Һ���������Ļ���
//
class CRightFunc
{
public:
	CRightFunc(){};
	virtual ~CRightFunc(){};

	/// @Input
	/// @Param	t		�Ա�����ֵ
	/// @Param	x		��ʼ����ֵ
	/// @Output
	/// @Param	result	����õ��ĺ���ֵ
	virtual	void operator() (double t, const CVector<double>& x, CVector<double>& result) const = 0;

};


//
//�Һ����࣬���������Һ���������Ļ���
//double * version
//
class AsTRightFunc
{
public:
	AsTRightFunc(){};
	virtual ~AsTRightFunc(){};

	/// @Input
	/// @Param	t		�Ա�����ֵ
	/// @Param	x		��ʼ����ֵ
	/// @Param	n		��ʼ����ֵ�ĸ���
	/// @Output
	/// @Param	result	����õ��ĺ���ֵ
	virtual	void operator() (double t, const double* x, int n, double* result) const = 0;

};





#endif // !defined(_ASRIGHTFUNCTION_H_)
