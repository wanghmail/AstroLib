////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2006                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsCoordinate.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsCoordinate.h"
#include "AsMath.h"
#include "AsAttitudeParam.h"
#include "AsTimeSystem.h"
#include "AsOrbitParam.h"
#include <cassert>


using namespace std;




//********************************************************************
// CCoord Construction/Destruction
//********************************************************************
//默认构造函数，三维矢量
CCoord::CCoord() : CVector<double>(3)
{
}

CCoord::CCoord(double X,double Y,double Z) : CVector<double>(X,Y,Z)
{
}

CCoord::CCoord(const CVector<double> &v) : CVector<double>(v)
{
	assert(v.GetSize()==3);
}

CCoord::~CCoord()
{
}

//********************************************************************
/// 根据一系列Euler角进行自转移
/// @author	Wang Hua
/// @date	2004.3
/// @Version	1.0
/// @Input
/// @param	angle	转移角数组
/// @param	flag	坐标轴标志数组(1,2,3-X,Y,Z)
//********************************************************************
void CCoord::Rotate(const CVector<double>& angle,const CVector<int>& Flag)
{
	assert(angle.GetSize() == Flag.GetSize());

    int i;
	int n = angle.GetSize();
	for(i=0; i<n; i++)
		assert(Flag[i]<=3 && Flag[i]>0);

	for (i=0; i<n; i++)
		Rotate(angle[i],Flag[i]);
}


void CCoord::Rotate(
			const double angle[], 
			const int seqFlag[], 
			int n)
{
    int i;
	for(i=0; i<n; i++)
		assert(seqFlag[i]<=3 && seqFlag[i]>0);

	for (i=0; i<n; i++)
		Rotate(angle[i],seqFlag[i]);
}


//********************************************************************
/// 给定方式的转移
/// @Author	Wang Hua
/// @Date	2004.3
/// @Input
/// @Param	angle	转移角数组
/// @Param	flag	坐标轴标志数组(1,2,3-X,Y,Z)
/// @Return	转移后的坐标
//********************************************************************
const CCoord CCoord::RotateTo(
			const CVector<double>& angle,const CVector<int>& Flag) const
{
	assert(angle.GetSize() == Flag.GetSize());

    int i;
	int n = angle.GetSize();
	for(i=0; i<n; i++)
		assert(Flag[i]<=3 && Flag[i]>0);

    CCoord Self = *this;
	CMatrix<double> Ts(3,3);

	for (i=0; i<n; i++)
	{
		AsEulerToMtx(angle[i],Flag[i],Ts);
		Self=Ts*Self;
	}

	return Self;
}


//********************************************************************
/// 按四元数自转移
/// @Author	Wang Hua
/// @Date	2004.4
/// @Input
/// @Param	quaternion	旋转四元数
//********************************************************************
void CCoord::Rotate(const CQuaternion& q)
{
	CQuaternion q1;
	q1.m_Qs = 0.0;
	q1.m_Qx = this->operator[](0);
	q1.m_Qy = this->operator[](1);
	q1.m_Qz = this->operator[](2);
	q1 = q.Conjugate()*q1*q;
	this->operator[](0) = q1.m_Qx;
	this->operator[](1) = q1.m_Qy;
	this->operator[](2) = q1.m_Qz;
}


//********************************************************************
/// 按四元数转移
/// @Author	Wang Hua
/// @Date	2004.4
/// @Input
/// @Param	quaternion		旋转四元数
//********************************************************************
const CCoord CCoord::RotateTo(const CQuaternion& q) const
{
	CCoord c = *this;
	c.Rotate(q);
	return c;
}


//********************************************************************
/// 单一角度的自转移
/// @Author	Wang Hua
/// @Date	2004.3
/// @Version	1.0
/// @Input
/// @Param	angle	转移角
/// @Param	flag	坐标轴标志(1,2,3-X,Y,Z)
//********************************************************************
void CCoord::Rotate(double Theta, int Flag)
{
	assert(Flag>=1 &&Flag<=3);

	CMatrix<double> tr(3,3);

	AsEulerToMtx(Theta,Flag,tr);	
	*this = tr*(*this);
}


//********************************************************************
/// 单一角度的转移
/// @Author	Wang Hua
/// @Date	2004.3
/// @Input
/// @Param	angle	转移角
/// @Param	axFlag	坐标轴标志(1,2,3-X,Y,Z)
//********************************************************************
const CCoord CCoord::RotateTo(double Theta,int Flag) const
{
	assert(Flag>=1 &&Flag<=3);

	CCoord Self;
	CMatrix<double> tr(3,3);

	AsEulerToMtx(Theta,Flag,tr);	
	Self = tr*(*this);

	return Self;
}


//********************************************************************
/// 根据Euler角转移
/// @Author	Wang Hua
/// @Date	2004.12.16
/// @Input
/// @Param	angle	转移角
/// @Param	seqFlag	坐标轴标志(1,2,3-X,Y,Z)
//********************************************************************
void CCoord::Rotate(const CEuler& Theta, int Flag)
{
	CMatrix<double> tr(3,3);

	AsEulerToMtx(Theta,Flag,tr);	
	*this = tr*(*this);
}


//********************************************************************
/// 根据Euler角转移
/// @Author	Wang Hua
/// @Date	2004.12.16
/// @Input
/// @Param	angle	转移角
/// @Param	seqFlag	坐标轴标志(1,2,3-X,Y,Z)
//********************************************************************
const CCoord CCoord::RotateTo(const CEuler& Theta, int Flag) const
{
	CMatrix<double> tr(3,3);

	AsEulerToMtx(Theta,Flag,tr);	
	return static_cast<CCoord>( tr*(*this) );
}


//********************************************************************
/// 根据轴和角转移
/// @Author	Wang Hua
/// @Date	2004.10.18
/// @Input
/// @Param	axis	the axis of rotation
/// @Param	angle	the angle of rotation
//********************************************************************
void CCoord::Rotate(const CCoord& axis, double angle)
{
	CMatrix<double> tr(3,3);

	AsAxAngToMtx(axis,angle,tr);	
	*this = tr*(*this);
}


//********************************************************************
/// 根据轴和角转移
/// @Author	Wang Hua
/// @Date	2004.11.10
/// @Input
/// @Param	axis	the axis of rotation
/// @Param	angle	the angle of rotation
//********************************************************************
const CCoord CCoord::RotateTo(const CCoord& axis, double angle) const
{
	CMatrix<double> tr(3,3);

	AsAxAngToMtx(axis,angle,tr);	
	return tr*(*this);
}


//********************************************************************
/// 根据转移矩阵转移
/// @Author	Wang Hua
/// @Date	2004.11.10
/// @Input
/// @Param	mtx		旋转矩阵
//********************************************************************
void CCoord::Rotate(const CMatrix<double>& mtx)
{
	*this = mtx*(*this);
}


//********************************************************************
/// 根据转移矩阵转移
/// @Author	Wang Hua
/// @Date	2004.11.10
/// @Input
/// @Param	quaternion		旋转四元数
//********************************************************************
const CCoord CCoord::RotateTo(const CMatrix<double>& mtx) const
{
	return mtx*(*this);
}



/////////////////////////////////////////////////////////////
// CLatLonRad
CLatLonRad::CLatLonRad()
		 : m_Lat(0.0), m_Lon(0.0), m_Rad(0.0)
{
}


CLatLonRad::CLatLonRad(double lat, double lon, double rad)
		 : m_Lat(lat), m_Lon(lon), m_Rad(rad)
{
}


/////////////////////////////////////////////////////////////
// CLatLonAlt
CLatLonAlt::CLatLonAlt()
		 : m_Lat(0.0), m_Lon(0.0), m_Alt(0.0)
{
}


CLatLonAlt::CLatLonAlt(double lat, double lon, double alt)
		 : m_Lat(lat), m_Lon(lon), m_Alt(alt)
{
}



/////////////////////////////////////////////////////////////
// CPolar3
CPolar3::CPolar3()
		 : m_Radius(0.0), m_Theta(0.0), m_Z(0.0)
{	
}


CPolar3::CPolar3(double radius, double theta, double z)
		 : m_Radius(radius), m_Theta(theta), m_Z(z)
{
}


/////////////////////////////////////////////////////////////
// CAziElevRng
CAziElevRng::CAziElevRng()
		 : m_Azi(0.0), m_Elev(0.0), m_Rng(0.0)
{
}


CAziElevRng::CAziElevRng(double azi, double elev, double rng)
		 : m_Azi(azi), m_Elev(elev), m_Rng(rng)
{
}


//***********************************************************
//柱坐标转换为直角坐标
//Author:	Wang Hua
//Date:		2005.7.21
//Input:	polar
//Output:	cart
//***********************************************************
void  AsPolarToCart(const CPolar3& polar, CCoord& cart)
{
	cart[0] = polar.m_Radius*cos(polar.m_Theta);
	cart[1] = polar.m_Radius*sin(polar.m_Theta);
	cart[2] = polar.m_Z;
}


//***********************************************************
//直角坐标转换为柱坐标
//Author:	Wang Hua
//Date:		2005.7.21
//Input:	cart
//Output:	polar	polar.m_Theta [-pi/2,pi/2]
//***********************************************************
void  AsCartToPolar(const CCoord& cart, CPolar3& polar)
{
	polar.m_Theta  = atan2(cart[1], cart[0]); 
	polar.m_Radius = sqrt(cart[0]*cart[0]+cart[1]*cart[1]);
	polar.m_Z      = cart[2];
}


//***********************************************************
//convension of ECF cartisian coordinate to latitude longitude 
//  radius(spherical coordinate)
//Author:	Wang Hua
//Date:		2004.9.10 2005.12.12 2006.8.10
//Input:	cart
//Output:	llr		llr.m_Lat [-pi/2, pi/2]
//					llr.m_Lon [-pi,pi]
//***********************************************************
void AsCartToLLR(const CCoord& cart, CLatLonRad& llr)
{
	llr.m_Rad = cart.Norm();
	llr.m_Lat = asin(cart[2] / llr.m_Rad);
	llr.m_Lon = atan2(cart[1], cart[0]); 
}


//***********************************************************
//convension of latitude longitude radius(spherical coordinate) 
//  to ECF cartisian coordinate
//Author:	Wang Hua
//Date:		2004.9.10
//***********************************************************
void AsLLRToCart(const CLatLonRad& llr, CCoord& cart)
{
	double rcoslat  = llr.m_Rad * cos(llr.m_Lat);
	cart[0] = rcoslat * cos(llr.m_Lon);
	cart[1] = rcoslat * sin(llr.m_Lon);
	cart[2] = llr.m_Rad * sin(llr.m_Lat);
}


//***********************************************************
//determines the "centric" latitude, longitude, and 
//  radius and rates given the cartesian position and velocity.  The input 
//  cartesian vectors can be in any coordinate system and the output will 
//  represent the corresponding spherical coordinates. 
/// @Author	Wang Hua
/// @Date	2004.9.12 10.11 2006.8.10
//			2006.12.26北京修改，好像计算llrRate.m_Lat的公式原来不对！！！
/// @Input	cartPos
//			cartVel
/// @Output	llrPos	llrPos.m_Lat [-pi/2, pi/2]
//					llrPos.m_Lon [-pi,pi]
//			llrRate
//***********************************************************
void AsCartToLLR(
            const CCoord& cartPos,
            const CCoord& cartVel,
            CLatLonRad& llrPos,
            CLatLonRad& llrRate)
{
    double xy2   = cartPos[0]*cartPos[0]+cartPos[1]*cartPos[1];
    llrPos.m_Rad = sqrt(xy2 + cartPos[2]*cartPos[2]);

    if (llrPos.m_Rad==0)
    {
        llrPos.m_Lat = llrPos.m_Lon = 0;
    }
    else
    {
        llrPos.m_Lat = atan(cartPos[2] / sqrt(xy2));
        llrPos.m_Lon = atan2(cartPos[1], cartPos[0]);
    }

    if (llrPos.m_Rad==0)
    {
        llrRate.m_Lat = 0;
        llrRate.m_Lon = 0;
        llrRate.m_Rad = 0;
    }
    else if (xy2==0)
    {
        llrRate.m_Lat = 0;
        llrRate.m_Lon = 0;
        llrRate.m_Rad = (cartPos[2]*cartVel[2]) / llrPos.m_Rad;
    }
    else
    {
        llrRate.m_Lon = (cartVel[1]*cartPos[0]-cartVel[0]*cartPos[1]) / xy2;
        llrRate.m_Rad = (cartPos[0]*cartVel[0]+cartPos[1]*cartVel[1]+cartPos[2]*cartVel[2]) / llrPos.m_Rad;
        llrRate.m_Lat = (cartVel[2]-sin(llrPos.m_Lat)*llrRate.m_Rad) / (cos(llrPos.m_Lat)*llrPos.m_Rad);
    }
}


//***********************************************************
//converts the "centric" latitude, longitude, and 
//  radius and rates to the cartesian position and velocity.  The  
//  cartesian vectors will be in the same coordinate system as the input  
//  spherical coordinates. 
/// @Author	Wang Hua
/// @Date	2004.9.12 10.11
//***********************************************************
void AsLLRToCart(const CLatLonRad& llrPos, const CLatLonRad& llrRate,
            CCoord& cartPos, CCoord& cartVel)
{
    double coslat      = cos(llrPos.m_Lat);
    double coslon      = cos(llrPos.m_Lon);
    double sinlat      = sin(llrPos.m_Lat);
    double sinlon      = sin(llrPos.m_Lon);
    double rcoslat     = llrPos.m_Rad*coslat;
    double rsinlat     = llrPos.m_Rad*sinlat;
    double dr_coslat   = llrRate.m_Rad*coslat;
    double rdlatsinlat = rsinlat*llrRate.m_Lat;
    double rdloncoslat = rcoslat*llrRate.m_Lon;

    cartPos[0] = rcoslat*coslon;
    cartPos[1] = rcoslat*sinlon;
    cartPos[2] = rsinlat;
    cartVel[0] = dr_coslat*coslon - rdlatsinlat*coslon - rdloncoslat*sinlon;
    cartVel[1] = dr_coslat*sinlon - rdlatsinlat*sinlon + rdloncoslat*coslon;
    cartVel[2] = llrRate.m_Rad*sinlat + rcoslat*llrRate.m_Lat;
}


//********************************************************************
//Convert central body fixed Cartesian coordinate to planetodetic latitude,
//	longitude and altitude (sphere central body).
//Author:	Wang Hua
//Date:		2004.9.7
//Input:	cart	cartesian coordinate
//			radius	radius of central body
//Output:	lla		m_Lon[-PI,PI], m_Lat[-PI/2,PI/2]
//Return:	
//********************************************************************
void  AsSphCartToLLA(const CCoord& cart, double radius, CLatLonAlt& lla)
{
	CLatLonRad llr;
	AsCartToLLR(cart,llr);
	lla.m_Alt = llr.m_Rad-radius;
	lla.m_Lat = llr.m_Lat;
	lla.m_Lon = llr.m_Lon;
}


//********************************************************************
//Convert planetodetic latitude, longitude and altitude to central body 
//	fixed Cartesian coordinate (sphere central body).
//Author:	Wang Hua
//Date:		2004.10.12
//Input:	lla		latitude, longitude and altitude
//			radius	radius of central body
//Output:	cart	cartesian coordinate
//********************************************************************
void  AsSphLLAToCart(const CLatLonAlt& lla, double radius, CCoord& cart)
{
	CLatLonRad llr;
	llr.m_Lat = lla.m_Lat;
	llr.m_Lon = lla.m_Lon;
	llr.m_Rad = lla.m_Alt+radius;
	AsLLRToCart(llr, cart);
}


//********************************************************************
//Convert latitude longitude and radius to planetodetic latitude,
//	longitude and altitude (sphere central body).
//Author:	Wang Hua
//Date:		2004.10.12
//********************************************************************
void  AsSphLLRToLLA(const CLatLonRad& llr, double radius, CLatLonAlt& lla)
{
	lla.m_Lat = llr.m_Lat;
	lla.m_Lon = llr.m_Lon;
	lla.m_Alt = llr.m_Rad-radius;
}


//********************************************************************
//Convert planetodetic latitude, longitude and altitude to  latitude
//	longitude and radius  (sphere central body).
//Author:	Wang Hua
//Date:		2004.10.12
//********************************************************************
void  AsSphLLAToLLR(const CLatLonAlt& lla, double radius, CLatLonRad& llr)
{
	llr.m_Lat = lla.m_Lat;
	llr.m_Lon = lla.m_Lon;
	llr.m_Rad = lla.m_Alt+radius;
}


//********************************************************************
//Convert central body fixed Cartesian coordinate to planetodetic latitude,
//	longitude and altitude (oblate central body).
//Author:	Wang Hua
//Date:		2004.10.13
//Input:	radius		the radius of central body.
//			flatFact	the flattening factor of the central body (1-b/a).
//						note the eccentricity of central body (c/a).
//Output:	lla			m_Lon[-PI,PI], m_Lat[-PI/2,PI/2]
//********************************************************************
void AsOblCartToLLA(const CCoord& cart, double radius, double flatFact, CLatLonAlt& lla)
{
	const double EPS = 1.0e-13;

	double B,H,xy,newN,newB,ee,sinB;

	lla.m_Lon = atan2(cart[1], cart[0]); //longitude

	//initial value
	ee   = 1.0-AsSqr(1.0-flatFact);
	xy   = sqrt(cart[0]*cart[0]+cart[1]*cart[1]);
	H    = cart.Norm()-radius*radius*(1-flatFact);
	newB = atan( cart[2] / (xy*(1-ee*radius/(radius+H))) );

	do
	{
		B    = newB;
		sinB = sin(B);
		newN = radius/sqrt(1-ee*AsSqr(sinB));
		newB = atan( (cart[2]+ee*newN*sinB) / xy );
	}while ( fabs(newB-B) > EPS );

	lla.m_Lat = newB;
	lla.m_Alt = xy/cos(newB)-newN;
}


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
void  AsOblLLAToCart(const CLatLonAlt& lla, double radius, double flatFact, CCoord& cart)
{
	double slat = sin(lla.m_Lat);
	double ee   = 1.0-AsSqr(1.0-flatFact);
	double N    = radius/sqrt(1.0-ee*AsSqr(slat));
	double temp = (N+lla.m_Alt)*cos(lla.m_Lat);
	cart[0]     = temp*cos(lla.m_Lon);
	cart[1]     = temp*sin(lla.m_Lon);
	cart[2]     = (N*(1.0-ee)+lla.m_Alt)*slat;
}


//********************************************************************
//Convert latitude, longitude and altitude to planetodetic latitude,
//	longitude and altitude (oblate central body).
//Author:	Wang Hua
//Date:		2004.10.14
//********************************************************************
void  AsOblLLRToLLA(const CLatLonRad& llr, double radius, double flatFact, CLatLonAlt& lla)
{
	CCoord cart;
	AsLLRToCart(llr, cart);
	AsOblCartToLLA(cart, radius, flatFact, lla);
}


//********************************************************************
//Convert planetodetic latitude, longitude and altitude to latitude,
//	longitude and altitude (oblate central body).
//Author:	Wang Hua
//Date:		2004.10.14
//Input:	lla			latitude, longitude and altitude
//			radius		long radius of central body
//			flatFact	扁率
//Output:	cart		cartesian coordinate
//********************************************************************
void  AsOblLLAToLLR(const CLatLonAlt& lla, double radius, double flatFact, CLatLonRad& llr)
{
	CCoord cart;
	AsOblLLAToCart(lla,radius,flatFact,cart);
	AsCartToLLR(cart,llr);
}


//********************************************************************
//Return transformation matrix from ICS(input Cartesian system, either a 
//	central body-fixed or central body-inertial system) to VVLH(Vehicle 
//  Velocity Local Horizontal).
//  The VVLH coordinate system is defines as follows: Positive Z is
//  along the nadir vector, positive Y is along the cross product of positive Z
//  and the vehicle velocity vector, positive X is completed according to
//  the right hand rule. 
//  The VVLH system is undefined if the vehicle
//  position vector is zero, the vehicle velocity is zero, or the vehicle
//  velocity is parallel to the vehicle position (rectilinear orbits).
/// @Author	Wang Hua
/// @Date	2004.10.19
/// @Input	pos		the position of vehicle
//			vel		velocity of vehicle
/// @Output	mtx
//********************************************************************
bool  AsICSToVVLHMtx(const CCoord& pos, const CCoord& vel, 
								 CMatrix<double>& mtx)
{
	CCoord vy = vel.Cross(pos);

	//pos和vel方向不能相同
	if (vy.SqrNorm()!=0)
	{
		CCoord vx = pos.Cross(vy);
		
		vx.Unit();
		vy.Unit();
		double r = pos.Norm();		
		
		mtx[0][0] = vx[0];		mtx[0][1] = vx[1];		mtx[0][2] = vx[2];
		mtx[1][0] = vy[0];		mtx[1][1] = vy[1];		mtx[1][2] = vy[2];
		mtx[2][0] = -pos[0]/r;	mtx[2][1] = -pos[1]/r;	mtx[2][2] = -pos[2]/r;
		return true;
	}
	//pos为0，vel为0，或者pos和vel方向相同
	else
	{
		mtx[0][0] = 1;	mtx[0][1] = 0;	mtx[0][2] = 0;
		mtx[1][0] = 0;	mtx[1][1] = 1;	mtx[1][2] = 0;
		mtx[2][0] = 0;	mtx[2][1] = 0;	mtx[2][2] = 1;
		return false;
	}
}


//********************************************************************
//Return transformation matrix from VVLH(Vehicle Velocity Local Horizontal) to ICS.
/// @Author	Wang Hua
/// @Date	2004.10.19
/// @Input	pos		the position of vehicle
//			vel		velocity of vehicle
//********************************************************************
bool  AsVVLHToICSMtx(const CCoord& pos, const CCoord& vel, 
								 CMatrix<double>& mtx)
{
    if (AsICSToVVLHMtx(pos, vel, mtx))
    {
        mtx = mtx.Transpose();
        return true;
    }
    else
        return false;
}