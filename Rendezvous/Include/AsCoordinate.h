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





class	CQuaternion;
class	COrbElem;
class	CCartState;
class	CEuler;



//
//Three dimension coordinate.
//
class CCoord : public CVector<double>
{
public:
	CCoord();
	CCoord(double x, double y, double z);
	CCoord(const CVector<double> &vec);
	virtual ~CCoord();

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
	void			Rotate  (const CCoord& axis, double angle);

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
	const CCoord	RotateTo(const CVector<double>& angle, const CVector<int>& flag) const;

	//********************************************************************
	/// 按四元数转移
	/// @Author	Wang Hua
	/// @Date	2004.4
	/// @Input
	/// @Param	quaternion		旋转四元数
	//********************************************************************
	const CCoord	RotateTo(const CQuaternion& quaternion) const;

	//********************************************************************
	/// 单一角度的转移
	/// @Author	Wang Hua
	/// @Date	2004.3
	/// @Input
	/// @Param	angle	转移角
	/// @Param	axFlag	坐标轴标志(1,2,3-X,Y,Z)
	//********************************************************************
	const CCoord	RotateTo(double angle, int axFlag) const;

	//********************************************************************
	/// 根据Euler角转移
	/// @Author	Wang Hua
	/// @Date	2004.12.16
	/// @Input
	/// @Param	angle	转移角
	/// @Param	seqFlag	坐标轴标志(1,2,3-X,Y,Z)
	//********************************************************************
	const CCoord	RotateTo(const CEuler& angle, int seqFlag) const;

	//********************************************************************
	/// 根据轴和角转移
	/// @Author	Wang Hua
	/// @Date	2004.11.10
	/// @Input
	/// @Param	axis	the axis of rotation
	/// @Param	angle	the angle of rotation
	//********************************************************************
	const CCoord	RotateTo(const CCoord& axis, double angle) const;

	//********************************************************************
	/// 根据转移矩阵转移
	/// @Author	Wang Hua
	/// @Date	2004.11.10
	/// @Input
	/// @Param	quaternion		旋转四元数
	//********************************************************************
	const CCoord	RotateTo(const CMatrix<double>& mtx) const;
	
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
			const CCoord& cart, 
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
			CCoord& cart);


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
void	AsCartToLLR (const CCoord& cart, CLatLonRad& llr);

//***********************************************************
/// convension of latitude longitude radius(spherical coordinate) 
///   to ECF cartisian coordinate
/// @Author	Wang Hua
/// @Date	2004.9.10
//***********************************************************
void	AsLLRToCart (
			const CLatLonRad& llr, 
			CCoord& cart);

//***********************************************************
/// 直角坐标转换为经度、地心纬度和半径
/// determines the "centric" latitude, longitude, and 
///   radius and rates given the cartesian position and velocity.  The input 
///   cartesian vectors can be in any coordinate system and the output will 
///   represent the corresponding spherical coordinates. 
/// @Author	Wang Hua
/// @Date	2004.9.12 10.11 2006.8.10
//			2006.12.26北京修改，好像计算llrRate.m_Lat的公式原来不对！！！
/// @Input	
/// @Param	cartPos
/// @Param	cartVel
/// @Output	
/// @Param	llrPos	llr.m_Lat从xy平面转到z轴为正,[-pi/2, pi/2]
///					llr.m_Lon从x轴转到y轴为正,[-pi,pi]
/// @Param	llrRate	
//***********************************************************
void	AsCartToLLR (
			const CCoord& cartPos, 
			const CCoord& cartVel, 
			CLatLonRad& llrPos, 
			CLatLonRad& llrRate);

//***********************************************************
/// converts the "centric" latitude, longitude, and 
///   radius and rates to the cartesian position and velocity.  The  
///   cartesian vectors will be in the same coordinate system as the input  
///   spherical coordinates. 
/// @Author	Wang Hua
/// @Date	2004.9.12 10.11
/// @Input	
/// @Param	llrPos	llr.m_Lat从xy平面转到z轴为正
///					llr.m_Lon从x轴转到y轴为正
/// @Param	llrRate	
/// @Output	
/// @Param	cartPos
/// @Param	cartVel
//***********************************************************
void	AsLLRToCart (
			const CLatLonRad& llrPos, 
			const CLatLonRad& llrRate, 
			CCoord& cartPos, 
			CCoord& cartVel);


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
			const CCoord& cart, 
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
			CCoord& cart);

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
			const CCoord& cart,  
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
			CCoord& cart);

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

//********************************************************************
/// 计算从输入直角坐标系（地心惯性系或地固系）到VVLH坐标系的转换矩阵
///  VVLH定义为：z轴指向地心，x轴与z轴垂直指向速度方向，y轴与其它两轴成右手坐标系.
/// @Author	Wang Hua
/// @Date	2004.10.19
/// @Input	
/// @Param	pos		the position of vehicle 地心惯性系或地固系中的飞行器位置[m]
/// @Param	vel		velocity of vehicle 地心惯性系或地固系中的飞行器速度[m/s]
/// @Output	
/// @Param	mtx		ICS至VVLH转移矩阵
///					pos为0，vel为0，pos与vel方向相同时，均无法计算转换矩阵，此时mtx输出单位矩阵
/// @Return			true=计算正确; false=输入数据异常
//********************************************************************
bool	AsICSToVVLHMtx (
			const CCoord& pos, 
			const CCoord& vel, 
			CMatrix<double>& mtx);

//********************************************************************
/// 计算从VVLH坐标系到输入直角坐标系（地心惯性系或地固系）的转换矩阵
///  VVLH定义为：z轴指向地心，x轴与z轴垂直指向速度方向，y轴与其它两轴成右手坐标系.
/// Return transformation matrix from VVLH(Vehicle Velocity Local Horizontal) to ICS.
/// @Author	Wang Hua
/// @Date	2004.10.19
/// @Input	
/// @Param	pos		the position of vehicle 地心惯性系或地固系中的飞行器位置[m]
/// @Param	vel		velocity of vehicle 地心惯性系或地固系中的飞行器速度[m/s]
/// @Output	
/// @Param	mtx		VVLH到ICS的转移矩阵
///					pos为0，vel为0，pos与vel方向相同时，均无法计算转换矩阵，此时mtx输出单位矩阵
/// @Return			true=计算正确; false=输入数据异常
//********************************************************************
bool	AsVVLHToICSMtx (
			const CCoord& pos, 
			const CCoord& vel, 
			CMatrix<double>& mtx);





#endif // !defined(_ASCOORDINATE_H_)
