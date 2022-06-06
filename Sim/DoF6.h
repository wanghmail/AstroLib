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
			const CMatrix<DP>& inertia, const CCoord3& force, const CCoord3& torque, 
			CCoord3& pos, CCoord3& vel, CQuaternion& quat, CCoord3& angVel);


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

		void		operator()(DP t, const CVector<DP> &x, CVector<DP> &result) const;

		//
		//Attribute
		//
	public:
		CCoord3			m_Force;
		CCoord3			m_Torque;
		CMatrix<DP>		m_Inertia;
		double			m_Mass;

	}m_FuncOrbAtt;
};

#endif // !defined(AFX_DOF6_H__8599D4D4_C0F7_428F_9D8E_1CE91D76D292__INCLUDED_)
