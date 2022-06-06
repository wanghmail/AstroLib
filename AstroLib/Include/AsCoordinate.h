////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsCoordinate.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ASCOORDINATE_H_)
#define _ASCOORDINATE_H_


#include "AsVector.h"
#include "AsMatrix.h"


AsMBeginNamespace


class	CQuaternion;
class	COrbElem;
class	CCartState;
class	CEuler;

//
//Macro define.
//
#define CCoord				CCoord3


//
//Three dimension coordinate.
//
class CCoord3 : public CVector<double>
{
public:
	CCoord3();
	CCoord3(double x, double y, double z);
	CCoord3(const CVector<double> &vec);
	virtual ~CCoord3();

	//
	//坐标旋转函数
	//

	//********************************************************************
	/// 根据一系列Euler角进行自转移
	/// @author	Wang Hua
	/// @date	2004.3
	/// @Version	1.0
	/// @Input
	/// @param	angle	转移角数组
	/// @param	flag	坐标轴标志数组(1,2,3-X,Y,Z)
	//********************************************************************
	void			Rotate  (const CVector<double>& angle, const CVector<int>& seqFlag);

	//********************************************************************
	/// 根据一系列Euler角进行自转移
	/// @author	Wang Hua
	/// @date	2007.4.20
	/// @Version	1.0
	/// @Input
	/// @param	angle	转移角数组
	/// @param	seqFlag	坐标轴标志数组(1,2,3-X,Y,Z)
	/// @param	n		总的转动次数
	//********************************************************************
	void			Rotate(const double angle[], const int seqFlag[], int n);

	//********************************************************************
	/// 按四元数自转移
	/// @Author	Wang Hua
	/// @Date	2004.4
	/// @Input
	/// @Param	quaternion	旋转四元数
	//********************************************************************
	void			Rotate  (const CQuaternion& quaternion);

	//********************************************************************
	/// 单一角度的自转移
	/// @Author	Wang Hua
	/// @Date	2004.3
	/// @Version	1.0
	/// @Input
	/// @Param	angle	转移角
	/// @Param	flag	坐标轴标志(1,2,3-X,Y,Z)
	//********************************************************************
	void			Rotate  (double angle, int axFlag);

	//********************************************************************
	/// 根据Euler角转移
	/// @Author	Wang Hua
	/// @Date	2004.12.16
	/// @Input
	/// @Param	angle	转移角
	/// @Param	seqFlag	坐标轴标志(1,2,3-X,Y,Z)
	//********************************************************************
	void			Rotate  (const CEuler& angle, int seqFlag);

	//********************************************************************
	/// 根据轴和角转移
	/// @Author	Wang Hua
	/// @Date	2004.10.18
	/// @Input
	/// @Param	axis	the axis of rotation
	/// @Param	angle	the angle of rotation
	//********************************************************************
	void			Rotate  (const CCoord3& axis, double angle);

	//********************************************************************
	/// 根据转移矩阵转移
	/// @Author	Wang Hua
	/// @Date	2004.11.10
	/// @Input
	/// @Param	mtx		旋转矩阵
	//********************************************************************
	void			Rotate  (const CMatrix<double>& mtx);

	//********************************************************************
	/// 给定方式的转移
	/// @Author	Wang Hua
	/// @Date	2004.3
	/// @Input
	/// @Param	angle	转移角数组
	/// @Param	flag	坐标轴标志数组(1,2,3-X,Y,Z)
	/// @Return	转移后的坐标
	//********************************************************************
	const CCoord3	RotateTo(const CVector<double>& angle, const CVector<int>& flag) const;

	//********************************************************************
	/// 按四元数转移
	/// @Author	Wang Hua
	/// @Date	2004.4
	/// @Input
	/// @Param	quaternion		旋转四元数
	//********************************************************************
	const CCoord3	RotateTo(const CQuaternion& quaternion) const;

	//********************************************************************
	/// 单一角度的转移
	/// @Author	Wang Hua
	/// @Date	2004.3
	/// @Input
	/// @Param	angle	转移角
	/// @Param	axFlag	坐标轴标志(1,2,3-X,Y,Z)
	//********************************************************************
	const CCoord3	RotateTo(double angle, int axFlag) const;

	//********************************************************************
	/// 根据Euler角转移
	/// @Author	Wang Hua
	/// @Date	2004.12.16
	/// @Input
	/// @Param	angle	转移角
	/// @Param	seqFlag	坐标轴标志(1,2,3-X,Y,Z)
	//********************************************************************
	const CCoord3	RotateTo(const CEuler& angle, int seqFlag) const;

	//********************************************************************
	/// 根据轴和角转移
	/// @Author	Wang Hua
	/// @Date	2004.11.10
	/// @Input
	/// @Param	axis	the axis of rotation
	/// @Param	angle	the angle of rotation
	//********************************************************************
	const CCoord3	RotateTo(const CCoord3& axis, double angle) const;

	//********************************************************************
	/// 根据转移矩阵转移
	/// @Author	Wang Hua
	/// @Date	2004.11.10
	/// @Input
	/// @Param	quaternion		旋转四元数
	//********************************************************************
	const CCoord3	RotateTo(const CMatrix<double>& mtx) const;
	
};


//
//Latitude, longitude and radius class.
//  which are equivalent to spherical coordinates
//  m_Lat: angle above the x-y plane (rad)
//  m_Lon: angle in x-y plane from x towards y (rad)
//
class CLatLonRad
{
public:
	CLatLonRad();
	CLatLonRad(double lat, double lon, double rad);

public:
	double	m_Lat;		///< declination(inertial)/latitude(fixed), 赤纬
	double	m_Lon;		///< right ascension/longitude(fixed), 赤经
	double	m_Rad;		///< radius, 球半径
};


//
//Latitude, longitude and altitude class.
//
class CLatLonAlt
{
public:
	CLatLonAlt();
	CLatLonAlt(double lat, double lon, double alt);

public:
	double	m_Lat;		///< 大地纬度planetodetic latitude(rad)
	double	m_Lon;		///< 大地经度planetodetic longitude(rad)
	double	m_Alt;		///< 大地高planetodetic altitude
};


//
//Polar coordinate class.
//
class CPolar3
{
public:
	CPolar3();
	CPolar3(double radius, double theta, double z);

public:
	double	m_Radius;	///< 半径
	double	m_Theta;	///< 幅角, angle in x-y plane from x towards y (rad)
	double	m_Z;		///< Z坐标
};



//
//Azimuth/Elevation/Range.
//  The AER data are calculated using the generic LH/VVLH definition 
//  of azimuth and elevation, with azimuth as longitude measured from 
//  the x to y axes, and elevation positive towards the negative z axis.
//  
class CAziElevRng
{
public:
	CAziElevRng();
	CAziElevRng(double azi, double elev, double rng);

public:
    double	m_Azi;		///< 方位角
    double  m_Elev;		///< 仰角
    double  m_Rng;		///< 距离
};




//////////////////////////////////////////////////////////////////////
//
//Coordinate Conversion Functions.
//
//////////////////////////////////////////////////////////////////////

//***********************************************************
/// 直角坐标转换为柱坐标
/// @Author	Wang Hua
/// @Date	2005.7.21
/// @Input	
/// @Param	cart
/// @Output	
/// @Param	polar	polar.m_Theta [-pi/2,pi/2]
//***********************************************************
void	AsCartToPolar(
			const CCoord3& cart, 
			CPolar3& polar);

//***********************************************************
/// 柱坐标转换为直角坐标
/// @Author	Wang Hua
/// @Date	2005.7.21
/// @Input	
/// @Param	polar
/// @Output	
/// @Param	cart
//***********************************************************
void	AsPolarToCart(
			const CPolar3& polar, 
			CCoord3& cart);


//////////////////////////////////////////////////////////////////////
//
//Single-point conversion functions.
//
//////////////////////////////////////////////////////////////////////


//***********************************************************
/// convension of ECF cartisian coordinate to latitude longitude 
///   radius(spherical coordinate)
/// @Author	Wang Hua
/// @Date	2004.9.10 2005.12.12 2006.8.10
/// @Input	
/// @Param	cart
/// @Output	
/// @Param	llr		llr.m_Lat [-pi/2, pi/2]
///					llr.m_Lon [-pi,pi]
//***********************************************************
void	AsCartToLLR (const CCoord3& cart, CLatLonRad& llr);

//***********************************************************
/// convension of latitude longitude radius(spherical coordinate) 
///   to ECF cartisian coordinate
/// @Author	Wang Hua
/// @Date	2004.9.10
//***********************************************************
void	AsLLRToCart (
			const CLatLonRad& llr, 
			CCoord3& cart);


//////////////////////////////////////////////////////////////////////
//
//Sphere functions.
//
//////////////////////////////////////////////////////////////////////

//********************************************************************
/// Convert central body fixed Cartesian coordinate to planetodetic latitude,
///   longitude and altitude (sphere central body).
/// @Author	Wang Hua
/// @Date	2004.9.7
/// @Input	
/// @Param	cart	cartesian coordinate
/// @Param	radius	radius of central body
/// @Output	
/// @Param	lla		m_Lon[-PI,PI], m_Lat[-PI/2,PI/2]
/// @Return	
//********************************************************************
void	AsSphCartToLLA (
			const CCoord3& cart, 
			double radius, 
			CLatLonAlt& lla);

//********************************************************************
/// Convert planetodetic latitude, longitude and altitude to central body 
///   fixed Cartesian coordinate (sphere central body).
/// @Author	Wang Hua
/// @Date	2004.10.12
/// @Input	
/// @Param	lla		latitude, longitude and altitude
/// @Param	radius	radius of central body
/// @Output	
/// @Param	cart	cartesian coordinate
//********************************************************************
void	AsSphLLAToCart (
			const CLatLonAlt& lla, 
			double radius, 
			CCoord3& cart);

//********************************************************************
/// Convert latitude longitude and radius to planetodetic latitude,
///   longitude and altitude (sphere central body).
/// @Author	Wang Hua
/// @Date	2004.10.12
//********************************************************************
void	AsSphLLRToLLA  (
			const CLatLonRad& llr, 
			double radius, 
			CLatLonAlt& lla);

//********************************************************************
/// Convert planetodetic latitude, longitude and altitude to  latitude
///	  longitude and radius  (sphere central body).
/// @Author	Wang Hua
/// @Date	2004.10.12
//********************************************************************
void	AsSphLLAToLLR  (
			const CLatLonAlt& lla, 
			double radius, 
			CLatLonRad& llr);


//////////////////////////////////////////////////////////////////////
//
//Grouping: Oblate spheroid functions.
//
//////////////////////////////////////////////////////////////////////

//********************************************************************
/// Convert central body fixed Cartesian coordinate to planetodetic latitude,
///   longitude and altitude (oblate central body).
/// @Author	Wang Hua
/// @Date	2004.10.13
/// @Input	
/// @Param	radius		the radius of central body.
/// @Param	flatFact	the flattening factor of the central body (1-b/a).
///						note the eccentricity of central body (c/a).
/// @Output	
/// @Param	lla			m_Lon[-PI,PI], m_Lat[-PI/2,PI/2]
//********************************************************************
void	AsOblCartToLLA (
			const CCoord3& cart,  
			double radius, 
			double flatFact, 
			CLatLonAlt& lla);

//********************************************************************
/// Convert planetodetic latitude, longitude and altitude to central body 
///	  fixed Cartesian coordinate (oblate central body).
/// @Author	Wang Hua
/// @Date	2004.10.13
/// @Input	
/// @Param	lla			latitude, longitude and altitude
/// @Param	radius		long radius of central body
/// @Param	flatFact	the flattening factor of the central body (1-b/a).
///						note the eccentricity of central body (c/a).
/// @Output	
/// @Param	cart		cartesian coordinate
//********************************************************************
void	AsOblLLAToCart (
			const CLatLonAlt& lla,  
			double radius, 
			double flatFact, 
			CCoord3& cart);

//********************************************************************
/// Convert latitude, longitude and altitude to planetodetic latitude,
///	  longitude and altitude (oblate central body).
/// @Author	Wang Hua
/// @Date	2004.10.14
//********************************************************************
void	AsOblLLRToLLA  (
			const CLatLonRad& llr,  
			double radius, 
			double flatFact, 
			CLatLonAlt& lla);

//********************************************************************
/// Convert planetodetic latitude, longitude and altitude to latitude,
///	  longitude and altitude (oblate central body).
/// @Author	Wang Hua
/// @Date	2004.10.14
/// @Input	
/// @Param	lla			latitude, longitude and altitude
/// @Param	radius		long radius of central body
/// @Param	flatFact	扁率
/// @Output	
/// @Param	cart		cartesian coordinate
//********************************************************************
void	AsOblLLAToLLR  (
			const CLatLonAlt& lla,  
			double radius, 
			double flatFact, 
			CLatLonRad& llr);





AsMEndNamespace


#endif // !defined(_ASCOORDINATE_H_)
