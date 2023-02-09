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
	/// 根据旋转角度和旋转轴计算坐标转移矩阵
	/// @Author	Wang Hua
	/// @Date	2004.11.24
	/// @Input
	/// @Param	seqFlag	旋转sequence
	/// @Output
	/// @Param	mtx		返回的转移矩阵
	///***********************************************************************
	void	ToMtx      (int seqFlag, CMatrix<double>& mtx) const;

	///***********************************************************************
	/// 根据旋转角度和旋转轴计算quaternion
	/// @Author	Wang Hua
	/// @Date	2004.11.24
	/// @Input
	/// @Param	seqFlag	旋转sequence
	/// @Output
	/// @Param	quat	四元数
	///***********************************************************************
	void	ToQuat     (int seqFlag, CQuaternion& quat) const;

public:
	double	m_Angle1;	///< 第一个角
	double	m_Angle2;	///< 第二个角
	double	m_Angle3;	///< 第三个角
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

	///得到四元数的共轭四元数
	const CQuaternion	Conjugate() const;

	///得到四元数的逆元数
	const CQuaternion	Invert   () const;

	///得到四元数的范数
	double				Norm     () const;

	///单位化
	void				Unit     ();

	///***********************************************************************
	/// 根据旋转角度和旋转坐标轴计算四元数
	/// @Author	Wang Hua
	/// @Date	2004.3
	/// @Input
	/// @Param	angle	旋转角度
	/// @Param	axFlag	旋转轴(1,2,3-X,Y,Z)
	/// @Output	
	///***********************************************************************
	void	FromEuler  (double angle, int axFlag);

	///***********************************************************************
	/// 根据Euler角和旋转坐标轴计算四元数
	/// @Author	Wang Hua
	/// @Date	2004.11.24
	/// @Input
	/// @Param	euler	旋转Euler角
	/// @Param	seqFlag	旋转轴(1,2,3-X,Y,Z)
	/// @Output	
	///***********************************************************************
	void	FromEuler  (const CEuler& euler, int seqFlag);

	///***********************************************************************
	/// 根据旋转角度和旋转坐标轴计算四元数
	/// @Author	Wang Hua
	/// @Date	2004.10.22
	/// @Input
	/// @Param	angle	旋转角
	/// @Param	seqFlag	旋转轴(1,2,3-X,Y,Z)
	///***********************************************************************
	void	FromEuler  (const CVector<double>& angle, const CVector<int>& seqFlag);

	///***********************************************************************
	/// 根据旋转角度和旋转轴计算四元数
	/// @Author	Wang Hua
	/// @Date	2004.10.12
	/// @Input
	/// @Param	axis	旋转轴矢量
	/// @Param	angle	旋转角
	///***********************************************************************
	void	FromAxAng  (const CCoord& axis, double angle);

	///***********************************************************************
	/// 方向余弦矩阵转换成四元数
	/// @Author	Wang Hua
	/// @Date	2004.4
	/// @Input
	/// @Param	mtx		旋转矩阵
	///***********************************************************************
	void	FromMtx    (const CMatrix<double>& mtx);

	///***********************************************************************
	/// 根据四元数计算旋转角度和旋转轴
	/// @Author	Wang Hua
	/// @Date	2004.10.26
	/// @Output
	/// @Param	angle	旋转角度
	/// @Param	axFlag	旋转轴
	///***********************************************************************
	void	ToAxAng    (CCoord& axis, double& angle) const;

	///***********************************************************************
	/// 四元数转换成方向余弦矩阵
	/// @Author	Wang Hua
	/// @Date	2004.4
	/// @Output
	/// @Param	axis	旋转轴矢量
	/// @Param	angle	旋转角
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
/// 根据旋转角度和旋转轴计算坐标转移矩阵
/// @Author	Wang Hua
/// @Date	2004.3
/// @Input
/// @Param	angle	旋转角度
/// @Param	axFlag	旋转轴
/// @Output
/// @Param	mtx		返回的转移矩阵
///***********************************************************************
void	AsEulerToMtx  (
			double angle, 
			int axFlag, 
			CMatrix<double>& mtx);

///***********************************************************************
/// 根据旋转角度和旋转轴计算坐标转移矩阵
/// @Author	Wang Hua
/// @Date	2004.9.7
/// @Input
/// @Param	angle	旋转角度(弧度)
/// @Param	seqFlag	旋转轴标志:1=x,2=y,3=z
/// @Param	n		输入的角的个数
/// @Output	
/// @Param	mtx		转移矩阵
///***********************************************************************
void	AsEulerToMtx  (
			const double angle[], 
			const int seqFlag[], 
			int n, 
			CMatrix<double>& mtx);

///***********************************************************************
/// 根据旋转角度和旋转轴计算坐标转移矩阵
/// @Author	Wang Hua
/// @Date	2004.9.7
/// @Input
/// @Param	angle	旋转角度(弧度)
/// @Param	seqFlag	旋转轴标志:1=x,2=y,3=z
/// @Output
/// @Param	mtx		转移矩阵
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
/// @Param	axis	旋转轴矢量
/// @Param	angle	旋转角
/// @Output
/// @Param	mtx		转移矩阵
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
/// @Param	axis	旋转轴矢量
/// @Param	angle	旋转角
/// @Output
/// @Param	quat	四元数
///***********************************************************************
void	AsAxAngToQuat (
			const CCoord& axis, 
			double angle, 
			CQuaternion& quat);

///***********************************************************************
/// 根据旋转角度和旋转轴计算坐标转移矩阵
/// @Author	Wang Hua
/// @Date	2004.11.24
/// @Input
/// @Param	euler	Euler角
/// @Param	seqFlag	旋转sequence
/// @Output	
/// @Param	mtx		返回的转移矩阵
///***********************************************************************
void	AsEulerToMtx  (
			const CEuler& euler, 
			int seqFlag, 
			CMatrix<double>& mtx);

///***********************************************************************
/// 根据Euler角计算quaternion
/// @Author	Wang Hua
/// @Date	2004.11.24
/// @Input
/// @Param	euler	Euler角
/// @Param	seqFlag	旋转sequence
/// @Output
/// @Param	quat	四元数
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
/// @Param	mtx		转移矩阵
/// @Output
/// @Param	quat	四元数
///***********************************************************************
void	AsMtxToQuat   (
			const CMatrix<double>& mtx, 
			CQuaternion& quat);

///***********************************************************************
/// The conversion from quaternion to axis and angle.
/// @Author	Wang Hua
/// @Date	2004.11.24
/// @Input
/// @Param	quat	四元数
/// @Output
/// @Param	axis	旋转轴矢量
/// @Param	angle	旋转角[0, pi]
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
/// @Param	quat	四元数
/// @Output
/// @Param	mtx		转移矩阵
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
