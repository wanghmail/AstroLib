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
	//������ת����
	//

	//********************************************************************
	/// ����һϵ��Euler�ǽ�����ת��
	/// @author	Wang Hua
	/// @date	2004.3
	/// @Version	1.0
	/// @Input
	/// @param	angle	ת�ƽ�����
	/// @param	flag	�������־����(1,2,3-X,Y,Z)
	//********************************************************************
	void			Rotate  (const CVector<double>& angle, const CVector<int>& seqFlag);

	//********************************************************************
	/// ����һϵ��Euler�ǽ�����ת��
	/// @author	Wang Hua
	/// @date	2007.4.20
	/// @Version	1.0
	/// @Input
	/// @param	angle	ת�ƽ�����
	/// @param	seqFlag	�������־����(1,2,3-X,Y,Z)
	/// @param	n		�ܵ�ת������
	//********************************************************************
	void			Rotate(const double angle[], const int seqFlag[], int n);

	//********************************************************************
	/// ����Ԫ����ת��
	/// @Author	Wang Hua
	/// @Date	2004.4
	/// @Input
	/// @Param	quaternion	��ת��Ԫ��
	//********************************************************************
	void			Rotate  (const CQuaternion& quaternion);

	//********************************************************************
	/// ��һ�Ƕȵ���ת��
	/// @Author	Wang Hua
	/// @Date	2004.3
	/// @Version	1.0
	/// @Input
	/// @Param	angle	ת�ƽ�
	/// @Param	flag	�������־(1,2,3-X,Y,Z)
	//********************************************************************
	void			Rotate  (double angle, int axFlag);

	//********************************************************************
	/// ����Euler��ת��
	/// @Author	Wang Hua
	/// @Date	2004.12.16
	/// @Input
	/// @Param	angle	ת�ƽ�
	/// @Param	seqFlag	�������־(1,2,3-X,Y,Z)
	//********************************************************************
	void			Rotate  (const CEuler& angle, int seqFlag);

	//********************************************************************
	/// ������ͽ�ת��
	/// @Author	Wang Hua
	/// @Date	2004.10.18
	/// @Input
	/// @Param	axis	the axis of rotation
	/// @Param	angle	the angle of rotation
	//********************************************************************
	void			Rotate  (const CCoord& axis, double angle);

	//********************************************************************
	/// ����ת�ƾ���ת��
	/// @Author	Wang Hua
	/// @Date	2004.11.10
	/// @Input
	/// @Param	mtx		��ת����
	//********************************************************************
	void			Rotate  (const CMatrix<double>& mtx);

	//********************************************************************
	/// ������ʽ��ת��
	/// @Author	Wang Hua
	/// @Date	2004.3
	/// @Input
	/// @Param	angle	ת�ƽ�����
	/// @Param	flag	�������־����(1,2,3-X,Y,Z)
	/// @Return	ת�ƺ������
	//********************************************************************
	const CCoord	RotateTo(const CVector<double>& angle, const CVector<int>& flag) const;

	//********************************************************************
	/// ����Ԫ��ת��
	/// @Author	Wang Hua
	/// @Date	2004.4
	/// @Input
	/// @Param	quaternion		��ת��Ԫ��
	//********************************************************************
	const CCoord	RotateTo(const CQuaternion& quaternion) const;

	//********************************************************************
	/// ��һ�Ƕȵ�ת��
	/// @Author	Wang Hua
	/// @Date	2004.3
	/// @Input
	/// @Param	angle	ת�ƽ�
	/// @Param	axFlag	�������־(1,2,3-X,Y,Z)
	//********************************************************************
	const CCoord	RotateTo(double angle, int axFlag) const;

	//********************************************************************
	/// ����Euler��ת��
	/// @Author	Wang Hua
	/// @Date	2004.12.16
	/// @Input
	/// @Param	angle	ת�ƽ�
	/// @Param	seqFlag	�������־(1,2,3-X,Y,Z)
	//********************************************************************
	const CCoord	RotateTo(const CEuler& angle, int seqFlag) const;

	//********************************************************************
	/// ������ͽ�ת��
	/// @Author	Wang Hua
	/// @Date	2004.11.10
	/// @Input
	/// @Param	axis	the axis of rotation
	/// @Param	angle	the angle of rotation
	//********************************************************************
	const CCoord	RotateTo(const CCoord& axis, double angle) const;

	//********************************************************************
	/// ����ת�ƾ���ת��
	/// @Author	Wang Hua
	/// @Date	2004.11.10
	/// @Input
	/// @Param	quaternion		��ת��Ԫ��
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
	double	m_Lat;		///< declination(inertial)/latitude(fixed), ��γ
	double	m_Lon;		///< right ascension/longitude(fixed), �ྭ
	double	m_Rad;		///< radius, ��뾶
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
	double	m_Lat;		///< ���γ��planetodetic latitude(rad)
	double	m_Lon;		///< ��ؾ���planetodetic longitude(rad)
	double	m_Alt;		///< ��ظ�planetodetic altitude
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
	double	m_Radius;	///< �뾶
	double	m_Theta;	///< ����, angle in x-y plane from x towards y (rad)
	double	m_Z;		///< Z����
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
    double	m_Azi;		///< ��λ��
    double  m_Elev;		///< ����
    double  m_Rng;		///< ����
};




//////////////////////////////////////////////////////////////////////
//
//Coordinate Conversion Functions.
//
//////////////////////////////////////////////////////////////////////

//***********************************************************
/// ֱ������ת��Ϊ������
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
/// ������ת��Ϊֱ������
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
/// ֱ������ת��Ϊ���ȡ�����γ�ȺͰ뾶
/// determines the "centric" latitude, longitude, and 
///   radius and rates given the cartesian position and velocity.  The input 
///   cartesian vectors can be in any coordinate system and the output will 
///   represent the corresponding spherical coordinates. 
/// @Author	Wang Hua
/// @Date	2004.9.12 10.11 2006.8.10
//			2006.12.26�����޸ģ��������llrRate.m_Lat�Ĺ�ʽԭ�����ԣ�����
/// @Input	
/// @Param	cartPos
/// @Param	cartVel
/// @Output	
/// @Param	llrPos	llr.m_Lat��xyƽ��ת��z��Ϊ��,[-pi/2, pi/2]
///					llr.m_Lon��x��ת��y��Ϊ��,[-pi,pi]
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
/// @Param	llrPos	llr.m_Lat��xyƽ��ת��z��Ϊ��
///					llr.m_Lon��x��ת��y��Ϊ��
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
/// @Param	flatFact	����
/// @Output	
/// @Param	cart		cartesian coordinate
//********************************************************************
void	AsOblLLAToLLR  (
			const CLatLonAlt& lla,  
			double radius, 
			double flatFact, 
			CLatLonRad& llr);

//********************************************************************
/// ���������ֱ������ϵ�����Ĺ���ϵ��ع�ϵ����VVLH����ϵ��ת������
///  VVLH����Ϊ��z��ָ����ģ�x����z�ᴹֱָ���ٶȷ���y���������������������ϵ.
/// @Author	Wang Hua
/// @Date	2004.10.19
/// @Input	
/// @Param	pos		the position of vehicle ���Ĺ���ϵ��ع�ϵ�еķ�����λ��[m]
/// @Param	vel		velocity of vehicle ���Ĺ���ϵ��ع�ϵ�еķ������ٶ�[m/s]
/// @Output	
/// @Param	mtx		ICS��VVLHת�ƾ���
///					posΪ0��velΪ0��pos��vel������ͬʱ�����޷�����ת�����󣬴�ʱmtx�����λ����
/// @Return			true=������ȷ; false=���������쳣
//********************************************************************
bool	AsICSToVVLHMtx (
			const CCoord& pos, 
			const CCoord& vel, 
			CMatrix<double>& mtx);

//********************************************************************
/// �����VVLH����ϵ������ֱ������ϵ�����Ĺ���ϵ��ع�ϵ����ת������
///  VVLH����Ϊ��z��ָ����ģ�x����z�ᴹֱָ���ٶȷ���y���������������������ϵ.
/// Return transformation matrix from VVLH(Vehicle Velocity Local Horizontal) to ICS.
/// @Author	Wang Hua
/// @Date	2004.10.19
/// @Input	
/// @Param	pos		the position of vehicle ���Ĺ���ϵ��ع�ϵ�еķ�����λ��[m]
/// @Param	vel		velocity of vehicle ���Ĺ���ϵ��ع�ϵ�еķ������ٶ�[m/s]
/// @Output	
/// @Param	mtx		VVLH��ICS��ת�ƾ���
///					posΪ0��velΪ0��pos��vel������ͬʱ�����޷�����ת�����󣬴�ʱmtx�����λ����
/// @Return			true=������ȷ; false=���������쳣
//********************************************************************
bool	AsVVLHToICSMtx (
			const CCoord& pos, 
			const CCoord& vel, 
			CMatrix<double>& mtx);





#endif // !defined(_ASCOORDINATE_H_)
