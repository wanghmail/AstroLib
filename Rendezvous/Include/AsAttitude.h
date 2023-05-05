////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2000-2021                         //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsAttitude.h
//
//////////////////////////////////////////////////////////////////////
/// @file
/// @brief	姿态函数，头文件

#if !defined(_AsAttitude_H__INCLUDED_)
#define _AsAttitude_H__INCLUDED_


#include "AsCoordinate.h"
#include "AsAttitudeParam.h"
#include "AsRightFunction.h"


class CAttitude
{
	//
	//Operation.
	//
public:
    CAttitude();
	virtual ~CAttitude();

	void	AttitudeStep(double step, 
			const CMatrix<double>& inertia, const CCoord& torque, 
			CQuaternion& quat, CCoord& angVel);


	//
	//Attribute.
	//
private:
	//轨道姿态计算右函数类
	class CFuncAtt : public CRightFunc  
	{		
		//
		//Operation.
		//
	public:
        CFuncAtt();
		virtual ~CFuncAtt();

		void		operator()(double t, const CVector<double> &x, CVector<double> &result) const;

		//
		//Attribute
		//
	public:
		CCoord			    m_Torque;
		CMatrix<double>		m_Inertia;

	}m_FuncAtt;
};

#endif // !defined(_AsAttitude_H__INCLUDED_)
