////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2000-2021                         //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsOrbit.h: interface for the COrbit class.
//
//////////////////////////////////////////////////////////////////////
/// @file
/// @brief	������㣬ͷ�ļ�

#if !defined(ASORBIT_H__INCLUDED_)
#define ASORBIT_H__INCLUDED_



#include "AsCoordinate.h"
#include "AsRightFunction.h"


//
//�������ѧ��
//
class COrbit
{
	//
	//Operation.
	//
public:
    COrbit() {};
    ~COrbit() {};

	//********************************************************************
	/// û���������������ʱ������������һ��
	/// @author	Wang Hua
	/// @Date	2011-1-14
	/// @Input
	/// @Param	step			���沽�� (sec), ����̫��һ�㲻�ܳ���20s
	/// @In/Out
	/// @Param	pos				������λ��(m)
	/// @Param	vel				�������ٶ�(m/s)
	/// @Output
	/// @Return	
	//********************************************************************
	void	OrbitStep (
				double step, 
				CCoord& pos,
				CCoord& vel);

    //��������Һ�����
    class CFuncOrbit : public CRightFunc
    {
        //
        //Operation.
        //
    public:
        CFuncOrbit();
        ~CFuncOrbit();

        virtual void operator()(double t, const CVector<double> &x, CVector<double> &result) const;

    }m_FuncOrbit;

};



//�����������㺯��
void	AsBasicGravAccel(double gm, const CCoord& pos, CCoord& accel);
void	AsJ2Accel       (double gm, double J2, double refDist, const CCoord& pos, CCoord& pertAccel);


//*********************************************************************
/// ������ʼ�����յ�״̬������CW���̼�������ʩ�ӵĳ���
///   �ο�����ϵΪVVLH����ϵ
/// @Author	Wang Hua
/// @Date	2003.8 2004.9.6
/// @Input
/// @Param	iniPos		��ʼ���λ��[m]
/// @Param	iniVel		��ʼ����ٶ�[m/s]
/// @Param	endPos		�ն����λ��[m]
/// @Param	endVel		�ն�����ٶ�[m/s]
/// @Param	t			��������֮��ķ���ʱ��[s]
/// @Param	orbAngVel	�ο�������������ٶ�[rad/s]
/// @Output
/// @Param	impulse1	��һ������ֵ[m/s]
/// @Param	impulse2	�ڶ�������ֵ[m/s]
/// @Return	
//*********************************************************************
void	AsCWImpulse (
			const CCoord& iniPos, 
			const CCoord& iniVel, 
			const CCoord& endPos, 
			const CCoord& endVel,
			double t, 
			double orbAngVel, 
			CCoord& impulse1, 
			CCoord& impulse2);



#endif // !defined(_ASORBIT_H__INCLUDED_)
