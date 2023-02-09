////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsAttitudeParam.h
//
//////////////////////////////////////////////////////////////////////


#if !defined(_ASATTITUDEPARAM_H_)
#define _ASATTITUDEPARAM_H_


#include "AsCommon.h"
#include "AsMatrix.h"
#include "AsVector.h"





#define AsCAxisX 1;
#define AsCAxisY 2;
#define AsCAxisZ 3;


class CCoord;
class CQuaternion;


//
//Euler angle class.
//
class CEuler
{
public:
	CEuler();
	CEuler(double ang1, double ang2, double ang3);
	virtual ~CEuler();

	///***********************************************************************
	/// ������ת�ǶȺ���ת���������ת�ƾ���
	/// @Author	Wang Hua
	/// @Date	2004.11.24
	/// @Input
	/// @Param	seqFlag	��תsequence
	/// @Output
	/// @Param	mtx		���ص�ת�ƾ���
	///***********************************************************************
	void	ToMtx      (int seqFlag, CMatrix<double>& mtx) const;

	///***********************************************************************
	/// ������ת�ǶȺ���ת�����quaternion
	/// @Author	Wang Hua
	/// @Date	2004.11.24
	/// @Input
	/// @Param	seqFlag	��תsequence
	/// @Output
	/// @Param	quat	��Ԫ��
	///***********************************************************************
	void	ToQuat     (int seqFlag, CQuaternion& quat) const;

public:
	double	m_Angle1;	///< ��һ����
	double	m_Angle2;	///< �ڶ�����
	double	m_Angle3;	///< ��������
};


//
//Quaternion class.
//
class CQuaternion
{
public:
	CQuaternion();
	CQuaternion(double e, double x, double y, double z);
	virtual ~CQuaternion();

	inline	double		operator [](int i) const;
	inline	double&		operator [](int i);

	const CQuaternion	operator +() const;
	const CQuaternion	operator -() const;
	const CQuaternion&	operator+=(const CQuaternion& q);
	const CQuaternion	operator +(const CQuaternion& q) const;
	const CQuaternion&	operator-=(const CQuaternion& q);
	const CQuaternion	operator -(const CQuaternion& q) const;
	const CQuaternion&	operator*=(const CQuaternion& q);
	const CQuaternion	operator *(const CQuaternion& q) const;
	const CQuaternion&	operator*=(double d);
	const CQuaternion	operator *(double d) const;
	const CQuaternion&	operator/=(double d);
	const CQuaternion	operator /(double d) const;

	bool	operator==(const CQuaternion& q) const;
	bool	operator!=(const CQuaternion& q) const;

	///�õ���Ԫ���Ĺ�����Ԫ��
	const CQuaternion	Conjugate() const;

	///�õ���Ԫ������Ԫ��
	const CQuaternion	Invert   () const;

	///�õ���Ԫ���ķ���
	double				Norm     () const;

	///��λ��
	void				Unit     ();

	///***********************************************************************
	/// ������ת�ǶȺ���ת�����������Ԫ��
	/// @Author	Wang Hua
	/// @Date	2004.3
	/// @Input
	/// @Param	angle	��ת�Ƕ�
	/// @Param	axFlag	��ת��(1,2,3-X,Y,Z)
	/// @Output	
	///***********************************************************************
	void	FromEuler  (double angle, int axFlag);

	///***********************************************************************
	/// ����Euler�Ǻ���ת�����������Ԫ��
	/// @Author	Wang Hua
	/// @Date	2004.11.24
	/// @Input
	/// @Param	euler	��תEuler��
	/// @Param	seqFlag	��ת��(1,2,3-X,Y,Z)
	/// @Output	
	///***********************************************************************
	void	FromEuler  (const CEuler& euler, int seqFlag);

	///***********************************************************************
	/// ������ת�ǶȺ���ת�����������Ԫ��
	/// @Author	Wang Hua
	/// @Date	2004.10.22
	/// @Input
	/// @Param	angle	��ת��
	/// @Param	seqFlag	��ת��(1,2,3-X,Y,Z)
	///***********************************************************************
	void	FromEuler  (const CVector<double>& angle, const CVector<int>& seqFlag);

	///***********************************************************************
	/// ������ת�ǶȺ���ת�������Ԫ��
	/// @Author	Wang Hua
	/// @Date	2004.10.12
	/// @Input
	/// @Param	axis	��ת��ʸ��
	/// @Param	angle	��ת��
	///***********************************************************************
	void	FromAxAng  (const CCoord& axis, double angle);

	///***********************************************************************
	/// �������Ҿ���ת������Ԫ��
	/// @Author	Wang Hua
	/// @Date	2004.4
	/// @Input
	/// @Param	mtx		��ת����
	///***********************************************************************
	void	FromMtx    (const CMatrix<double>& mtx);

	///***********************************************************************
	/// ������Ԫ��������ת�ǶȺ���ת��
	/// @Author	Wang Hua
	/// @Date	2004.10.26
	/// @Output
	/// @Param	angle	��ת�Ƕ�
	/// @Param	axFlag	��ת��
	///***********************************************************************
	void	ToAxAng    (CCoord& axis, double& angle) const;

	///***********************************************************************
	/// ��Ԫ��ת���ɷ������Ҿ���
	/// @Author	Wang Hua
	/// @Date	2004.4
	/// @Output
	/// @Param	axis	��ת��ʸ��
	/// @Param	angle	��ת��
	///***********************************************************************
	void	ToMtx      (CMatrix<double>& mtx) const;

public:
	double	m_Qx;	///< x
	double	m_Qy;	///< y
	double	m_Qz;	///< z
	double	m_Qs;	///< s
};


//
//global function.
//
const CQuaternion	operator *(double num, const CQuaternion& q);


inline double CQuaternion::operator [](int i) const
{
	assert(i>=0 && i<4);
	switch(i) {
	case 0:
		return m_Qs;
	case 1:
		return m_Qx;
	case 2:
		return m_Qy;
	default:	//case 3:
		return m_Qz;
	}
}


inline double& CQuaternion::operator [](int i)
{
	assert(i>=0 && i<4);
	switch(i) {
	case 0:
		return m_Qs;
	case 1:
		return m_Qx;
	case 2:
		return m_Qy;
	default:	//case 3:
		return m_Qz;
	}
}


//
//Rotation function.
//
///***********************************************************************
/// ������ת�ǶȺ���ת���������ת�ƾ���
/// @Author	Wang Hua
/// @Date	2004.3
/// @Input
/// @Param	angle	��ת�Ƕ�
/// @Param	axFlag	��ת��
/// @Output
/// @Param	mtx		���ص�ת�ƾ���
///***********************************************************************
void	AsEulerToMtx  (
			double angle, 
			int axFlag, 
			CMatrix<double>& mtx);

///***********************************************************************
/// ������ת�ǶȺ���ת���������ת�ƾ���
/// @Author	Wang Hua
/// @Date	2004.9.7
/// @Input
/// @Param	angle	��ת�Ƕ�(����)
/// @Param	seqFlag	��ת���־:1=x,2=y,3=z
/// @Param	n		����Ľǵĸ���
/// @Output	
/// @Param	mtx		ת�ƾ���
///***********************************************************************
void	AsEulerToMtx  (
			const double angle[], 
			const int seqFlag[], 
			int n, 
			CMatrix<double>& mtx);

///***********************************************************************
/// ������ת�ǶȺ���ת���������ת�ƾ���
/// @Author	Wang Hua
/// @Date	2004.9.7
/// @Input
/// @Param	angle	��ת�Ƕ�(����)
/// @Param	seqFlag	��ת���־:1=x,2=y,3=z
/// @Output
/// @Param	mtx		ת�ƾ���
///***********************************************************************
void	AsEulerToMtx  (
			const CVector<double>& angle, 
			const CVector<int>& seqFlag, 
			CMatrix<double>& mtx);

///***********************************************************************
/// The conversion from Euler axis and angle to matrix.
/// @Author	Wang Hua
/// @Date	2004.10.13
/// @Input
/// @Param	axis	��ת��ʸ��
/// @Param	angle	��ת��
/// @Output
/// @Param	mtx		ת�ƾ���
///***********************************************************************
void	AsAxAngToMtx  (
			const CCoord& axis, 
			double angle, 
			CMatrix<double>& mtx);

///***********************************************************************
/// The conversion from axis and angle to quaternion.
/// @Author	Wang Hua
/// @Date	2004.11.24
/// @Input
/// @Param	axis	��ת��ʸ��
/// @Param	angle	��ת��
/// @Output
/// @Param	quat	��Ԫ��
///***********************************************************************
void	AsAxAngToQuat (
			const CCoord& axis, 
			double angle, 
			CQuaternion& quat);

///***********************************************************************
/// ������ת�ǶȺ���ת���������ת�ƾ���
/// @Author	Wang Hua
/// @Date	2004.11.24
/// @Input
/// @Param	euler	Euler��
/// @Param	seqFlag	��תsequence
/// @Output	
/// @Param	mtx		���ص�ת�ƾ���
///***********************************************************************
void	AsEulerToMtx  (
			const CEuler& euler, 
			int seqFlag, 
			CMatrix<double>& mtx);

///***********************************************************************
/// ����Euler�Ǽ���quaternion
/// @Author	Wang Hua
/// @Date	2004.11.24
/// @Input
/// @Param	euler	Euler��
/// @Param	seqFlag	��תsequence
/// @Output
/// @Param	quat	��Ԫ��
///***********************************************************************
void	AsEulerToQuat (
			const CEuler& euler, 
			int seqFlag, 
			CQuaternion& quat);

///***********************************************************************
/// The conversion from matrix to quaternion.
/// @Author	Wang Hua
/// @Date	2004.11.24
/// @Input
/// @Param	mtx		ת�ƾ���
/// @Output
/// @Param	quat	��Ԫ��
///***********************************************************************
void	AsMtxToQuat   (
			const CMatrix<double>& mtx, 
			CQuaternion& quat);

///***********************************************************************
/// The conversion from quaternion to axis and angle.
/// @Author	Wang Hua
/// @Date	2004.11.24
/// @Input
/// @Param	quat	��Ԫ��
/// @Output
/// @Param	axis	��ת��ʸ��
/// @Param	angle	��ת��[0, pi]
///***********************************************************************
void	AsQuatToAxAng (
			const CQuaternion& quat, 
			CCoord& axis, 
			double& angle);

///***********************************************************************
/// The conversion from quaternion to matrix.
/// @Author	Wang Hua
/// @Date	2004.11.24
/// @Input
/// @Param	quat	��Ԫ��
/// @Output
/// @Param	mtx		ת�ƾ���
///***********************************************************************
void	AsQuatToMtx   (
			const CQuaternion& quat, 
			CMatrix<double>& mtx);


//Degree to radian.
inline const CEuler AsDegToRad(CEuler deg)
{
	CEuler rad;
	rad.m_Angle1 = deg.m_Angle1*AsCDegToRad;
	rad.m_Angle2 = deg.m_Angle2*AsCDegToRad;
	rad.m_Angle3 = deg.m_Angle3*AsCDegToRad;
	return rad;
}

//Radian to degree.
inline const CEuler AsRadToDeg(CEuler rad)
{
	CEuler deg;
	deg.m_Angle1 = rad.m_Angle1*AsCRadToDeg;
	deg.m_Angle2 = rad.m_Angle2*AsCRadToDeg;
	deg.m_Angle3 = rad.m_Angle3*AsCRadToDeg;
	return deg;
}






#endif // !defined(_ASATTITUDEPARAM_H_)
