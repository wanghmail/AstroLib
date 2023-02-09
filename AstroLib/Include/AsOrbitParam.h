////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsOrbitParam.h
//////////////////////////

#if! defined (_ASORBITPARAM_H_)
#define  _ASORBITPARAM_H_


#include "AsCoordinate.h"



//
//Cartesian state elements
//  Position and velocity in cartesian coordinates.
//
class CCartState
{
public:
	CCartState();
	CCartState(const CCoord& pos, const CCoord& vel);
	CCartState(double xPos, double yPos, double zPos, double xVel, double yVel, double zVel);

	const CCartState	operator -() const;
	const CCartState&	operator+=(const CCartState& state);
	const CCartState	operator +(const CCartState& state) const;
	const CCartState&	operator-=(const CCartState& state);
	const CCartState	operator -(const CCartState& state) const;

public:
	CCoord	m_Pos;		///< 位置 position(m)
	CCoord	m_Vel;		///< 速度 velocity(m/s)
};


//
//Modified orbital elements
//
class CModOrbElem
{
public:
	CModOrbElem();
	CModOrbElem(double periRad, double ecc, double i, double raan, double argPeri, double trueA);

public:
	double	m_PeriRad;	///<近拱点半径 periapsis radius in m
	double	m_Ecc;		///<偏心率 eccentricity
	double	m_I;		///<轨道倾角 inclination in radians
	double	m_RAAN;		///<升交点赤经 right ascension of ascending node
	double	m_ArgPeri;	///<近拱点角 arg of periapsis in rad
	double	m_TrueA;	///<真近点角 true anomaly in radians
};


//
//classic orbit element.
//
class COrbElem
{
public:
	COrbElem();
	COrbElem(double sMajAx, double ecc, double i, double raan, double argPeri, double trueA);

public:
	double	m_SMajAx;	///< 长半轴 Semimajor axis
	double	m_Ecc;		///< 偏心率 Eccentricity
	double	m_I;		///< 轨道倾角 Inclination
	double	m_RAAN;		///< 升交点赤经 Right Ascension of the ascending node (RAAN)
	double	m_ArgPeri;	///< 近拱点角 Argument of Perigee
	double	m_TrueA;	///< 真近点角 True Anomaly
};


//********************************************************************
/// 从位置和速度矢量得到飞行器的轨道角速度
/// @Author	Wang Hua
/// @Date	2005.3.4
/// @Input
/// @Param	pos			飞行器位置(m)
/// @Param	vel			飞行器速度(m/s)
/// @Output
/// @Param	orbAngVel	orbit angular velocity			
/// @Return			
//********************************************************************
bool	AsCartStateToOrbAngVel(
			const CCoord& pos,
			const CCoord& vel, 
			CCoord& orbAngVel);

//********************************************************************
/// 由惯性系中的绝对速度和位置计算目标VVLH坐标系(z指向地心，x垂直z指向前，
///   y垂直轨道面指向负法向)中的相对位置坐标和速度矢量
/// @author	Wang Hua
/// @Date	2005.12.22
/// @Input
/// @Param	assocPos	绝对位置，即追踪飞行器位置(m)
/// @Param	assocVel	绝对速度，即追踪飞行器速度(m/s)
/// @Param	basePos		参考飞行器位置，即目标飞行器位置(m)
/// @Param	baseVel		参考飞行器速度，即目标飞行器速度(m/s)
/// @Output
/// @Param	relPos		目标轨道坐标系中的相对位置(m)
/// @Param	relVel		目标轨道坐标系中的相对速度(m/s)
//********************************************************************
void	AsStateToVVLHRelState(
			const CCoord& assocPos, 
			const CCoord& assocVel, 
			const CCoord& basePos, 
			const CCoord& baseVel, 
			CCoord& relPos, 
			CCoord& relVel);

//********************************************************************
/// 由目标VVLH坐标系(x指向前，z指向地心，y垂直轨道面)中的相对位置坐标和
///    速度矢量计算惯性系中的绝对速度和位置
/// @author	Wang Hua
/// @Date	2005.12.22
/// @Input
/// @Param	relPos		目标轨道坐标系中的相对位置
/// @Param	relVel		目标轨道坐标系中的相对速度
/// @Param	basePos		参考飞行器位置，即目标飞行器位置
/// @Param	baseVel		参考飞行器速度，即目标飞行器速度
/// @Output
/// @Param	assocPos	绝对位置，即追踪飞行器位置
/// @Param	assocVel	绝对速度，即追踪飞行器速度
//********************************************************************
void AsVVLHRelStateToState(
            const CCoord& relPos, 
            const CCoord& relVel,
            const CCoord& basePos, 
            const CCoord& baseVel,
            CCoord& assocPos, 
            CCoord& assocVel);




#endif //_ASORBITPARAM_H_