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
	CCoord	m_Pos;		///< λ�� position(m)
	CCoord	m_Vel;		///< �ٶ� velocity(m/s)
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
	double	m_PeriRad;	///<������뾶 periapsis radius in m
	double	m_Ecc;		///<ƫ���� eccentricity
	double	m_I;		///<������ inclination in radians
	double	m_RAAN;		///<������ྭ right ascension of ascending node
	double	m_ArgPeri;	///<������� arg of periapsis in rad
	double	m_TrueA;	///<������ true anomaly in radians
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
	double	m_SMajAx;	///< ������ Semimajor axis
	double	m_Ecc;		///< ƫ���� Eccentricity
	double	m_I;		///< ������ Inclination
	double	m_RAAN;		///< ������ྭ Right Ascension of the ascending node (RAAN)
	double	m_ArgPeri;	///< ������� Argument of Perigee
	double	m_TrueA;	///< ������ True Anomaly
};


//********************************************************************
/// ��λ�ú��ٶ�ʸ���õ��������Ĺ�����ٶ�
/// @Author	Wang Hua
/// @Date	2005.3.4
/// @Input
/// @Param	pos			������λ��(m)
/// @Param	vel			�������ٶ�(m/s)
/// @Output
/// @Param	orbAngVel	orbit angular velocity			
/// @Return			
//********************************************************************
bool	AsCartStateToOrbAngVel(
			const CCoord& pos,
			const CCoord& vel, 
			CCoord& orbAngVel);

//********************************************************************
/// �ɹ���ϵ�еľ����ٶȺ�λ�ü���Ŀ��VVLH����ϵ(zָ����ģ�x��ֱzָ��ǰ��
///   y��ֱ�����ָ�򸺷���)�е����λ��������ٶ�ʸ��
/// @author	Wang Hua
/// @Date	2005.12.22
/// @Input
/// @Param	assocPos	����λ�ã���׷�ٷ�����λ��(m)
/// @Param	assocVel	�����ٶȣ���׷�ٷ������ٶ�(m/s)
/// @Param	basePos		�ο�������λ�ã���Ŀ�������λ��(m)
/// @Param	baseVel		�ο��������ٶȣ���Ŀ��������ٶ�(m/s)
/// @Output
/// @Param	relPos		Ŀ��������ϵ�е����λ��(m)
/// @Param	relVel		Ŀ��������ϵ�е�����ٶ�(m/s)
//********************************************************************
void	AsStateToVVLHRelState(
			const CCoord& assocPos, 
			const CCoord& assocVel, 
			const CCoord& basePos, 
			const CCoord& baseVel, 
			CCoord& relPos, 
			CCoord& relVel);

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
            const CCoord& relPos, 
            const CCoord& relVel,
            const CCoord& basePos, 
            const CCoord& baseVel,
            CCoord& assocPos, 
            CCoord& assocVel);




#endif //_ASORBITPARAM_H_