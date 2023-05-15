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
/// @brief	轨道计算，头文件

#if !defined(ASORBIT_H__INCLUDED_)
#define ASORBIT_H__INCLUDED_



#include "AsCoordinate.h"
#include "AsRightFunction.h"


//
//轨道动力学类
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
	/// 没有外力或冲量作用时计算轨道，积分一步
	/// @author	Wang Hua
	/// @Date	2011-1-14
	/// @Input
	/// @Param	step			仿真步长 (sec), 不能太大，一般不能超过20s
	/// @In/Out
	/// @Param	pos				飞行器位置(m)
	/// @Param	vel				飞行器速度(m/s)
	/// @Output
	/// @Return	
	//********************************************************************
	void	OrbitStep (
				double step, 
				CCoord& pos,
				CCoord& vel);

    //轨道计算右函数类
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



//基础引力计算函数
void	AsBasicGravAccel(double gm, const CCoord& pos, CCoord& accel);
void	AsJ2Accel       (double gm, double J2, double refDist, const CCoord& pos, CCoord& pertAccel);


//*********************************************************************
/// 给定初始和最终的状态，根据CW方程计算所需施加的冲量
///   参考坐标系为VVLH坐标系
/// @Author	Wang Hua
/// @Date	2003.8 2004.9.6
/// @Input
/// @Param	iniPos		初始相对位置[m]
/// @Param	iniVel		初始相对速度[m/s]
/// @Param	endPos		终端相对位置[m]
/// @Param	endVel		终端相对速度[m/s]
/// @Param	t			两个冲量之间的飞行时间[s]
/// @Param	orbAngVel	参考飞行器轨道角速度[rad/s]
/// @Output
/// @Param	impulse1	第一个冲量值[m/s]
/// @Param	impulse2	第二个冲量值[m/s]
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
