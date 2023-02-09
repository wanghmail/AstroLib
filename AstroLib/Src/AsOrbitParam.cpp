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

//����������
const CCartState CCartState::operator -() const
{
	return CCartState(-m_Pos, -m_Vel);
}

//�����������
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
/// ��λ�ú��ٶ�ʸ���õ��������Ĺ�����ٶ�
/// @Author	Wang Hua
/// @Date	2005.3.4
/// @Input
/// @Param	pos			������λ��
/// @Param	vel			�������ٶ�
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
/// �ɹ���ϵ�еľ����ٶȺ�λ�ü���Ŀ��VVLH����ϵ(xָ��ǰ��zָ����ģ�
///   y��ֱ�����)�е����λ��������ٶ�ʸ��
/// @author	Wang Hua
/// @Date	2005.12.22
/// @Input
/// @Param	assocPos	����λ�ã���׷�ٷ�����λ��
/// @Param	assocVel	�����ٶȣ���׷�ٷ������ٶ�
/// @Param	basePos		�ο�������λ�ã���Ŀ�������λ��
/// @Param	baseVel		�ο��������ٶȣ���Ŀ��������ٶ�
/// @Output
/// @Param	relPos		Ŀ��������ϵ�е����λ��
/// @Param	relVel		Ŀ��������ϵ�е�����ٶ�
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
/// ��Ŀ��VVLH����ϵ(xָ��ǰ��zָ����ģ�y��ֱ�����)�е����λ�������
///    �ٶ�ʸ���������ϵ�еľ����ٶȺ�λ��
/// @author	Wang Hua
/// @Date	2005.12.22
/// @Input
/// @Param	relPos		Ŀ��������ϵ�е����λ��
/// @Param	relVel		Ŀ��������ϵ�е�����ٶ�
/// @Param	basePos		�ο�������λ�ã���Ŀ�������λ��
/// @Param	baseVel		�ο��������ٶȣ���Ŀ��������ٶ�
/// @Output
/// @Param	assocPos	����λ�ã���׷�ٷ�����λ��
/// @Param	assocVel	�����ٶȣ���׷�ٷ������ٶ�
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