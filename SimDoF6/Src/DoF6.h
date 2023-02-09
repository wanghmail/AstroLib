// DoF6.h: interface for the CDoF6 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOF6_H__8599D4D4_C0F7_428F_9D8E_1CE91D76D292__INCLUDED_)
#define AFX_DOF6_H__8599D4D4_C0F7_428F_9D8E_1CE91D76D292__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "AstroLib.h"


class CDoF6  
{
	//
	//Operation.
	//
public:
	CDoF6();
	virtual ~CDoF6();

	void	DoF6 (double step, double mass, 
			const CMatrix<double>& inertia, const CCoord& force, const CCoord& torque, 
			CCoord& pos, CCoord& vel, CQuaternion& quat, CCoord& angVel);


	//
	//Attribute.
	//
private:
	//轨道姿态计算右函数类
	class CFuncOrbAtt : public CRightFunc  
	{		
		//
		//Operation.
		//
	public:
		CFuncOrbAtt();
		virtual ~CFuncOrbAtt();

		void		operator()(double t, const CVector<double> &x, CVector<double> &result) const;

		//
		//Attribute
		//
	public:
		CCoord			    m_Force;
		CCoord			    m_Torque;
		CMatrix<double>		m_Inertia;
		double			    m_Mass;

	}m_FuncOrbAtt;
};

#endif // !defined(AFX_DOF6_H__8599D4D4_C0F7_428F_9D8E_1CE91D76D292__INCLUDED_)
