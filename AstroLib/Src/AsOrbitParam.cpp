////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2006                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsOrbitParam.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsOrbitParam.h"
#include "AsInterpolation.h"





//////////////////////////////////////////////////////////////////////
//CCartState
CCartState::CCartState() : m_Pos(0.0, 0.0, 0.0), m_Vel(0.0, 0.0, 0.0)
{
}


CCartState::CCartState(const CCoord& pos, const CCoord& vel) : m_Pos(pos), m_Vel(vel)
{
}


CCartState::CCartState(double xPos, double yPos, double zPos, double xVel, double yVel, double zVel)
	: m_Pos(xPos, yPos, zPos), m_Vel(xVel, yVel, zVel)
{
}

//重载求负运算
const CCartState CCartState::operator -() const
{
	return CCartState(-m_Pos, -m_Vel);
}

//重载相减运算
const CCartState& CCartState::operator -=(const CCartState& state)
{
	m_Pos -= state.m_Pos;
	m_Vel -= state.m_Vel;
	return *this;
}


const CCartState& CCartState::operator +=(const CCartState& state)
{
	m_Pos += state.m_Pos;
	m_Vel += state.m_Vel;
	return *this;
}


const CCartState CCartState::operator -(const CCartState& state) const
{
	CCartState s=*this;
	s -= state;
	return s;
}


const CCartState CCartState::operator +(const CCartState& state) const
{
	CCartState s=*this;
	s += state;
	return s;
}


//////////////////////////////////////////////////////////////////////
//CModOrbElem
CModOrbElem::CModOrbElem() 
			: m_PeriRad(0.0), m_Ecc(0.0), m_I(0.0), 
			  m_RAAN(0.0), m_ArgPeri(0.0), m_TrueA(0.0)

{
}


CModOrbElem::CModOrbElem(double periRad, double ecc, double i, 
						 double raan, double argPeri, double trueA)
	: m_PeriRad(periRad), m_Ecc(ecc), m_I(i), 
	  m_RAAN(raan), m_ArgPeri(argPeri), m_TrueA(trueA)
{
}


//////////////////////////////////////////////////////////////////////
//COrbElem
COrbElem::COrbElem() : m_SMajAx(0.0), m_Ecc(0.0), m_I(0.0), m_RAAN(0.0), m_ArgPeri(0.0), m_TrueA(0.0)
{
}


COrbElem::COrbElem(double sMajAx, double ecc, double i, 
				   double raan, double argPeri, double trueA)
	: m_SMajAx(sMajAx), m_Ecc(ecc), m_I(i), 
	  m_RAAN(raan), m_ArgPeri(argPeri), m_TrueA(trueA)
{
}


//********************************************************************
/// 从位置和速度矢量得到飞行器的轨道角速度
/// @Author	Wang Hua
/// @Date	2005.3.4
/// @Input
/// @Param	pos			飞行器位置
/// @Param	vel			飞行器速度
/// @Output
/// @Param	orbAngVel	orbit angular velocity			
/// @Return			
//********************************************************************
bool AsCartStateToOrbAngVel(
            const CCoord& pos,
            const CCoord& vel, 
            CCoord& orbAngVel)
{
    orbAngVel = pos*vel / pos.SqrNorm();
    return true;
}


//********************************************************************
/// 由惯性系中的绝对速度和位置计算目标VVLH坐标系(x指向前，z指向地心，
///   y垂直轨道面)中的相对位置坐标和速度矢量
/// @author	Wang Hua
/// @Date	2005.12.22
/// @Input
/// @Param	assocPos	绝对位置，即追踪飞行器位置
/// @Param	assocVel	绝对速度，即追踪飞行器速度
/// @Param	basePos		参考飞行器位置，即目标飞行器位置
/// @Param	baseVel		参考飞行器速度，即目标飞行器速度
/// @Output
/// @Param	relPos		目标轨道坐标系中的相对位置
/// @Param	relVel		目标轨道坐标系中的相对速度
//********************************************************************
void AsStateToVVLHRelState(
            const CCoord& assocPos, const CCoord& assocVel,
            const CCoord& basePos, const CCoord& baseVel,
            CCoord& relPos, CCoord& relVel)
{
    CCoord		angVel;
    CMatrix<double> mtxICSToVVLH(3, 3);

    AsCartStateToOrbAngVel(basePos, baseVel, angVel);
    relPos = assocPos-basePos;
    relVel = assocVel-baseVel-angVel*relPos;

    AsICSToVVLHMtx(basePos, baseVel, mtxICSToVVLH);
    relPos = mtxICSToVVLH*relPos;
    relVel = mtxICSToVVLH*relVel;
}


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
            const CCoord& relPos, const CCoord& relVel,
            const CCoord& basePos, const CCoord& baseVel,
            CCoord& assocPos, CCoord& assocVel)
{
    CCoord		angVel;
    CMatrix<double> mat_relOrb_iner(3, 3);

    AsVVLHToICSMtx(basePos, baseVel, mat_relOrb_iner);
    assocPos = mat_relOrb_iner*relPos;
    assocVel = mat_relOrb_iner*relVel;

    AsCartStateToOrbAngVel(basePos, baseVel, angVel);
    assocVel += baseVel+angVel*assocPos;
    assocPos += basePos;
}