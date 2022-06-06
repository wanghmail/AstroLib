////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2006                          //
//                              Wang Hua                                  //
//             College of Aerospace and Material Engineering              //
//               National University of Defense Technology                //
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
AsMUsingNamespace



//********************************************************************
// CCoord3 Construction/Destruction
//********************************************************************
//默认构造函数，三维矢量
CCoord3::CCoord3() : CVector<double>(3)
{
}

CCoord3::CCoord3(double X,double Y,double Z) : CVector<double>(X,Y,Z)
{
}

CCoord3::CCoord3(const CVector<double> &v) : CVector<double>(v)
{
	assert(v.GetSize()==3);
}

CCoord3::~CCoord3()
{
}

//********************************************************************
//给定方式的自转移
//Author:	Wang Hua
//Date:		2004.3
//Version:	1.0
//Input:	angle	转移角数组
//			Flag	坐标轴标志数组(1,2,3-X,Y,Z)
//********************************************************************
void CCoord3::Rotate(const CVector<double>& angle,const CVector<int>& Flag)
{
	assert(angle.GetSize() == Flag.GetSize());

	int n = angle.GetSize();
	for(int i=0; i<n; i++)
		assert(Flag[i]<=3 && Flag[i]>0);

	for (int i=0; i<n; i++)
		Rotate(angle[i],Flag[i]);
}


void CCoord3::Rotate(
			const double angle[], 
			const int seqFlag[], 
			int n)
{
	for(int i=0; i<n; i++)
		assert(seqFlag[i]<=3 && seqFlag[i]>0);

	for (int i=0; i<n; i++)
		Rotate(angle[i],seqFlag[i]);
}


//********************************************************************
//给定方式的转移
//Author:	Wang Hua
//Date:		2004.3
//Input:	angle	转移角数组
//			Flag	坐标轴标志数组(1,2,3-X,Y,Z)
//Return:	转移后的坐标
//********************************************************************
const CCoord3 CCoord3::RotateTo(
			const CVector<double>& angle,const CVector<int>& Flag) const
{
	assert(angle.GetSize() == Flag.GetSize());

	int n = angle.GetSize();
	for(int i=0; i<n; i++)
		assert(Flag[i]<=3 && Flag[i]>0);

    CCoord3 Self = *this;
	CMatrix<double> Ts(3,3);

	for (int i=0; i<n; i++)
	{
		AsEulerToMtx(angle[i],Flag[i],Ts);
		Self=Ts*Self;
	}

	return Self;
}


//********************************************************************
//按四元数自转移
//Author:	Wang Hua
//Date:		2004.4
//Input:	q		旋转四元数
//********************************************************************
void CCoord3::Rotate(const CQuaternion& q)
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
//按四元数转移
//Author:	Wang Hua
//Date:		2004.4
//Input:	q		旋转四元数
//********************************************************************
const CCoord3 CCoord3::RotateTo(const CQuaternion& q) const
{
	CCoord3 c = *this;
	c.Rotate(q);
	return c;
}


//********************************************************************
//单一角度的自转移
//Author:	Wang Hua
//Date:		2004.3
//Version:	1.0
//Input:	Theta	转移角
//			Flag	坐标轴标志(1,2,3-X,Y,Z)
//********************************************************************
void CCoord3::Rotate(double Theta, int Flag)
{
	assert(Flag>=1 &&Flag<=3);

	CMatrix<double> tr(3,3);

	AsEulerToMtx(Theta,Flag,tr);	
	*this = tr*(*this);
}


//********************************************************************
//单一角度的转移
//Author:	Wang Hua
//Date:		2004.3
//Version:	1.0
//Input:	Theta	转移角
//			Flag	坐标轴标志(1,2,3-X,Y,Z)
//********************************************************************
const CCoord3 CCoord3::RotateTo(double Theta,int Flag) const
{
	assert(Flag>=1 &&Flag<=3);

	CCoord3 Self;
	CMatrix<double> tr(3,3);

	AsEulerToMtx(Theta,Flag,tr);	
	Self = tr*(*this);

	return Self;
}


//********************************************************************
//根据Euler角转移
//Author:	Wang Hua
//Date:		2004.12.16
//********************************************************************
void CCoord3::Rotate(const CEuler& Theta, int Flag)
{
	CMatrix<double> tr(3,3);

	AsEulerToMtx(Theta,Flag,tr);	
	*this = tr*(*this);
}


//********************************************************************
//根据Euler角转移
//Author:	Wang Hua
//Date:		2004.12.16
//********************************************************************
const CCoord3 CCoord3::RotateTo(const CEuler& Theta, int Flag) const
{
	CMatrix<double> tr(3,3);

	AsEulerToMtx(Theta,Flag,tr);	
	return static_cast<CCoord3>( tr*(*this) );
}


//********************************************************************
//根据轴和角转移
//Author:	Wang Hua
//Date:		2004.10.18
//Input:	axis	the axis of rotation
//			angle	the angle of rotation
//********************************************************************
void CCoord3::Rotate(const CCoord3& axis, double angle)
{
	CMatrix<double> tr(3,3);

	AsAxAngToMtx(axis,angle,tr);	
	*this = tr*(*this);
}


//********************************************************************
//根据轴和角转移
//Author:	Wang Hua
//Date:		2004.11.10
//********************************************************************
const CCoord3 CCoord3::RotateTo(const CCoord3& axis, double angle) const
{
	CMatrix<double> tr(3,3);

	AsAxAngToMtx(axis,angle,tr);	
	return tr*(*this);
}


//********************************************************************
//根据转移矩阵转移
//Author:	Wang Hua
//Date:		2004.11.10
//********************************************************************
void CCoord3::Rotate(const CMatrix<double>& mtx)
{
	*this = mtx*(*this);
}


//********************************************************************
//Author:	Wang Hua
//Date:		2004.11.10
//********************************************************************
const CCoord3 CCoord3::RotateTo(const CMatrix<double>& mtx) const
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
void AsMNamespace AsPolarToCart(const CPolar3& polar, CCoord3& cart)
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
void AsMNamespace AsCartToPolar(const CCoord3& cart, CPolar3& polar)
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
void AsMNamespace AsCartToLLR(const CCoord3& cart, CLatLonRad& llr)
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
void AsMNamespace AsLLRToCart(const CLatLonRad& llr, CCoord3& cart)
{
	double rcoslat  = llr.m_Rad * cos(llr.m_Lat);
	cart[0] = rcoslat * cos(llr.m_Lon);
	cart[1] = rcoslat * sin(llr.m_Lon);
	cart[2] = llr.m_Rad * sin(llr.m_Lat);
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
void AsMNamespace AsSphCartToLLA(const CCoord3& cart, double radius, CLatLonAlt& lla)
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
void AsMNamespace AsSphLLAToCart(const CLatLonAlt& lla, double radius, CCoord3& cart)
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
void AsMNamespace AsSphLLRToLLA(const CLatLonRad& llr, double radius, CLatLonAlt& lla)
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
void AsMNamespace AsSphLLAToLLR(const CLatLonAlt& lla, double radius, CLatLonRad& llr)
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
void AsMNamespace AsOblCartToLLA(const CCoord3& cart, double radius, double flatFact, CLatLonAlt& lla)
{
	const double EPS = 1.0e-8;

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
//Convert planetodetic latitude, longitude and altitude to central body 
//	fixed Cartesian coordinate (oblate central body).
//Author:	Wang Hua
//Date:		2004.10.13
//Input:	lla			latitude, longitude and altitude
//			radius		long radius of central body
//			flatFact	the flattening factor of the central body (1-b/a).
//						note the eccentricity of central body (c/a).
//Output:	cart		cartesian coordinate
//********************************************************************
void AsMNamespace AsOblLLAToCart(const CLatLonAlt& lla, double radius, double flatFact, CCoord3& cart)
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
void AsMNamespace AsOblLLRToLLA(const CLatLonRad& llr, double radius, double flatFact, CLatLonAlt& lla)
{
	CCoord3 cart;
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
void AsMNamespace AsOblLLAToLLR(const CLatLonAlt& lla, double radius, double flatFact, CLatLonRad& llr)
{
	CCoord3 cart;
	AsOblLLAToCart(lla,radius,flatFact,cart);
	AsCartToLLR(cart,llr);
}


