////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2006                          //
//                              Wang Hua                                  //
//             College of Aerospace and Material Engineering              //
//               National University of Defense Technology                //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsAttitudeParam.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsAttitudeParam.h"
#include "AsCoordinate.h"


using namespace std;
AsMUsingNamespace



//////////////////////////////////////////////////////////////////////
// CEuler Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEuler::CEuler() : m_Angle1(0.0), m_Angle2(0.0), m_Angle3(0.0)
{
}

CEuler::CEuler(double ang1, double ang2, double ang3)
			 : m_Angle1(ang1), m_Angle2(ang2), m_Angle3(ang3)
{
}

CEuler::~CEuler()
{
}


//********************************************************************
//������ת�ǶȺ���ת���������ת�ƾ���
//Author:	Wang Hua
//Date:		2004.11.24
//Input:	euler	Euler��
//			seqFlag	��תsequence
//Output:	mtx		���ص�ת�ƾ���
//********************************************************************
void CEuler::ToMtx (int seqFlag, CMatrix<double>& mtx) const
{
	assert(seqFlag==123 || seqFlag==132 || seqFlag==121 || seqFlag==131 
		|| seqFlag==213 || seqFlag==231 || seqFlag==212 || seqFlag==232
		|| seqFlag==321 || seqFlag==312 || seqFlag==313 || seqFlag==323);

	int m;
	CMatrix<double> mat(3,3);

	m       = seqFlag/100;
	AsEulerToMtx(m_Angle1, m, mtx);

	seqFlag = seqFlag-m*100;
	m       = seqFlag/10;
	AsEulerToMtx(m_Angle2, m, mat);
	mtx     = mat*mtx;

	seqFlag = seqFlag-m*10;
	AsEulerToMtx(m_Angle3, seqFlag, mat);
	mtx     = mat*mtx;
}


//********************************************************************
//������ת�ǶȺ���ת�����quaternion
//Author:	Wang Hua
//Date:		2004.11.24
//Input:	euler	Euler��
//			seqFlag	��תsequence
//Output:	quat	
//********************************************************************
void CEuler::ToQuat (int seqFlag, CQuaternion& quat) const
{
	quat.FromEuler(*this, seqFlag);
}



//////////////////////////////////////////////////////////////////////
// CQuaternion Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuaternion::CQuaternion()
			 : m_Qs(1.0), m_Qx(0.0), m_Qy(0.0), m_Qz(0.0)
{
}

CQuaternion::CQuaternion(double e,double x,double y,double z)
			 : m_Qs(e), m_Qx(x), m_Qy(y), m_Qz(z)
{
}

CQuaternion::~CQuaternion()
{

}


const CQuaternion CQuaternion::operator +() const
{
	return *this;
}


const CQuaternion CQuaternion::operator -() const
{
	CQuaternion q;
	q.m_Qs = -m_Qs;
	q.m_Qx = -m_Qx;
	q.m_Qy = -m_Qy;
	q.m_Qz = -m_Qz;
	return q;
}


const CQuaternion& CQuaternion::operator +=(const CQuaternion& q)
{
	m_Qs += q.m_Qs;
	m_Qx += q.m_Qx;
	m_Qy += q.m_Qy;
	m_Qz += q.m_Qz;
	return *this;
}


const CQuaternion CQuaternion::operator +(const CQuaternion& q) const
{
	CQuaternion qu = *this;
	qu += q;
	return qu;
}


const CQuaternion& CQuaternion::operator -=(const CQuaternion& q)
{
	m_Qs -= q.m_Qs;
	m_Qx -= q.m_Qx;
	m_Qy -= q.m_Qy;
	m_Qz -= q.m_Qz;
	return *this;
}


const CQuaternion CQuaternion::operator -(const CQuaternion& q) const
{
	CQuaternion qu = *this;
	qu -= q;
	return qu;
}


const CQuaternion& CQuaternion::operator *=(const CQuaternion& q)
{
	CQuaternion q0 = *this;
	m_Qs = q0.m_Qs*q.m_Qs-q0.m_Qx*q.m_Qx-q0.m_Qy*q.m_Qy-q0.m_Qz*q.m_Qz;
	m_Qx = q0.m_Qs*q.m_Qx+q0.m_Qx*q.m_Qs+q0.m_Qy*q.m_Qz-q0.m_Qz*q.m_Qy;
	m_Qy = q0.m_Qs*q.m_Qy-q0.m_Qx*q.m_Qz+q0.m_Qy*q.m_Qs+q0.m_Qz*q.m_Qx;
	m_Qz = q0.m_Qs*q.m_Qz+q0.m_Qx*q.m_Qy-q0.m_Qy*q.m_Qx+q0.m_Qz*q.m_Qs;
	return *this;
}


const CQuaternion CQuaternion::operator *(const CQuaternion& q) const
{
	CQuaternion qu = *this;
	qu *= q;
	return qu;
}


const CQuaternion& CQuaternion::operator*=(double d)
{
	m_Qs *= d;
	m_Qx *= d;
	m_Qy *= d;
	m_Qz *= d;
	return *this;
}


const CQuaternion CQuaternion::operator *(double d) const
{
	CQuaternion q = *this;
	q *= d;
	return q;
}

	
const CQuaternion operator *(double d, const CQuaternion& q)
{
	return q*d;
}


const CQuaternion& CQuaternion::operator/=(double d)
{
	m_Qs /= d;
	m_Qx /= d;
	m_Qy /= d;
	m_Qz /= d;
	return *this;
}


const CQuaternion CQuaternion::operator /(double d) const
{
	CQuaternion q = *this;
	q /= d;
	return q;
}

// Equalities, inequalities.
bool CQuaternion::operator==(const CQuaternion& q) const
{
    return (m_Qs == q.m_Qs) && (m_Qx == q.m_Qx) && (m_Qy == q.m_Qy) && (m_Qz == q.m_Qz);
}

bool CQuaternion::operator!=(const CQuaternion& q) const
{
    return !(*this == q); 
}


//�õ���Ԫ���Ĺ�����Ԫ��
const CQuaternion CQuaternion::Conjugate() const
{
	CQuaternion q;
	q.m_Qs =  m_Qs;
	q.m_Qx = -m_Qx;
	q.m_Qy = -m_Qy;
	q.m_Qz = -m_Qz;
	return q;
}

//�õ���Ԫ������Ԫ��
const CQuaternion CQuaternion::Invert() const
{
	return Conjugate()/Norm();
}


//�õ���Ԫ���ķ���
double CQuaternion::Norm() const
{
	return m_Qs*m_Qs+m_Qx*m_Qx+m_Qy*m_Qy+m_Qz*m_Qz;
}


void CQuaternion::Unit()
{
	assert(Norm()!=0);

	*this /= sqrt(m_Qs*m_Qs+m_Qx*m_Qx+m_Qy*m_Qy+m_Qz*m_Qz);
}


//********************************************************************
//������ת�ǶȺ���ת�����������Ԫ��
//Author:	Wang Hua
//Date:		2004.3
//Input:	angle	��ת�Ƕ�
//			axFlag	��ת��(1,2,3-X,Y,Z)
//Output:	
//********************************************************************
void CQuaternion::FromEuler(double angle, int axFlag)
{
	assert(axFlag>=1 && axFlag<=3);

	double a = angle*0.5;
	m_Qs = cos(a);
	switch(axFlag)
	{
	case 1:
		m_Qx = sin(a);
		m_Qy = 0.0;
		m_Qz = 0.0;
		break;
	case 2:
		m_Qx = 0.0;
		m_Qy = sin(a);
		m_Qz = 0.0;
		break;
	case 3:
		m_Qx = 0.0;
		m_Qy = 0.0;
		m_Qz = sin(a);
		break;
	}
}


//********************************************************************
//����Euler�Ǻ���ת�����������Ԫ��
//Author:	Wang Hua
//Date:		2004.11.24
//Input:	angle	��תEuler��
//			seqFlag	��ת��(1,2,3-X,Y,Z)
//Output:	
//********************************************************************
void CQuaternion::FromEuler(const CEuler& angle, int seqFlag)
{
	assert(seqFlag==123 || seqFlag==132 || seqFlag==121 || seqFlag==131 
		|| seqFlag==213 || seqFlag==231 || seqFlag==212 || seqFlag==232
		|| seqFlag==321 || seqFlag==312 || seqFlag==313 || seqFlag==323);

	int m;
	CQuaternion q1;

	m       = seqFlag/100;
	FromEuler(angle.m_Angle1, m);

	seqFlag = seqFlag-m*100;
	m       = seqFlag/10;
	q1.FromEuler(angle.m_Angle2, m);
	*this  *= q1;

	seqFlag = seqFlag-m*10;
	q1.FromEuler(angle.m_Angle3, seqFlag);
	*this  *= q1;

	Unit();
}


//********************************************************************
//������ת�ǶȺ���ת�����������Ԫ��
//Author:	Wang Hua
//Date:		2004.10.22
//********************************************************************
void CQuaternion::FromEuler(const CVector<double>& angle, const CVector<int>& seqFlag)
{
	assert(angle.GetSize()==seqFlag.GetSize());

	CQuaternion q1;
	int n = angle.GetSize();
	if (n>0)
	{
		FromEuler(angle[0],seqFlag[0]);
	}
	for (int i=1; i<n; i++)
	{
		q1.FromEuler(angle[i],seqFlag[i]);
		*this *= q1;
	}

	Unit();
}


//********************************************************************
//������ת�ǶȺ���ת�������Ԫ��
//Author:	Wang Hua
//Date:		2004.10.12
//********************************************************************
void CQuaternion::FromAxAng(const CCoord3& axis, double angle)
{
	CCoord3 ax(axis);
	ax.Unit();
	double a = angle*0.5;
	double b = sin(a);
	m_Qs = cos(a);
	m_Qx = axis[0]*b;
	m_Qy = axis[1]*b;
	m_Qz = axis[2]*b;
}


//********************************************************************
//������Ԫ��������ת�ǶȺ���ת��
//Author:	Wang Hua
//Date:		2004.10.26
//********************************************************************
void CQuaternion::ToAxAng(CCoord3& axis, double& angle) const
{
	angle = 2.0*acos(m_Qs);
	double b  = sin(angle*0.5);
	if (b!=0)
	{
		axis[0] = m_Qx/b;
		axis[1] = m_Qy/b;
		axis[2] = m_Qz/b;
	}
	else
	{
		axis[0] = 1.0;
		axis[1] = 0.0;
		axis[2] = 0.0;
	}
}


//********************************************************************
//�������Ҿ���ת������Ԫ��
//Author:	Wang Hua
//Date:		2004.4
//********************************************************************
void CQuaternion::FromMtx(const CMatrix<double>& m)
{
	const double TINY = 1.0e-15;

	double w = 1.0+m[0][0]+m[1][1]+m[2][2];
	if (w <= TINY)
	{
		m_Qs = 0;
		m_Qx = sqrt(0.5*(m[0][0]+1));
		m_Qy = sqrt(0.5*(m[1][1]+1));
		m_Qz = sqrt(0.5*(m[2][2]+1));
		if (m_Qx != 0)
		{
			if (m_Qx*m[0][1]<0)
				m_Qy = -m_Qy;
			if (m_Qx*m[0][2]<0)
				m_Qz = -m_Qz;
		}
		else if (m_Qy != 0)
		{
			if (m_Qy*m[1][2]<0)
				m_Qz = -m_Qz;
		}
	}
	else
	{
		m_Qs = sqrt(w)*0.5;
		double tempdbl = 4.0*m_Qs;
		m_Qx = (m[1][2]-m[2][1])/tempdbl;
		m_Qy = (m[2][0]-m[0][2])/tempdbl;
		m_Qz = (m[0][1]-m[1][0])/tempdbl;
	}

	Unit();
}


//********************************************************************
//��Ԫ��ת���ɷ������Ҿ���
//Author:	Wang Hua
//Date:		2004.4
//********************************************************************
void CQuaternion::ToMtx(CMatrix<double>& m) const
{
	assert(m.GetSizeCol()==3 && m.GetSizeRow()==3);

	double xy = m_Qx*m_Qy;
	double yz = m_Qy*m_Qz;
	double zx = m_Qz*m_Qx;
	double sx = m_Qs*m_Qx;
	double sy = m_Qs*m_Qy;
	double sz = m_Qs*m_Qz;
	double xx = m_Qx*m_Qx;
	double yy = m_Qy*m_Qy;
	double zz = m_Qz*m_Qz;

	m[0][0] = 1.0-2.0*(yy+zz);	m[0][1] = 2.0*(xy+sz);		m[0][2] = 2.0*(zx-sy);
	m[1][0] = 2.0*(xy-sz);		m[1][1] = 1.0-2.0*(xx+zz);	m[1][2] = 2.0*(yz+sx);
	m[2][0] = 2.0*(zx+sy);		m[2][1] = 2.0*(yz-sx);		m[2][2] = 1.0-2.0*(xx+yy);

	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
		{
			if (m[i][j] > 1.0)
				m[i][j] = 1;
			else if (m[i][j] < -1.0)
				m[i][j] = -1;
		}
	}
}


///***********************************************************************
/// ������ת�ǶȺ���ת���������ת�ƾ���
///   <<���պ������˶��Ľ�ģ>>P10
///   <<������ѧ�̳�>>P101
/// @Author	Wang Hua
/// @Date	2004.3
/// @Input
/// @Param	angle	��ת�Ƕ�
/// @Param	axFlag	��ת��
/// @Output
/// @Param	mtx		���ص�ת�ƾ���
///***********************************************************************
void AsMNamespace AsEulerToMtx(double angle, int axFlag, CMatrix<double>& mtx)
{
	assert(axFlag<=3 && axFlag>0);

	double c = cos(angle);
	double s = sin(angle);
	switch(axFlag)
	{
	case 1:
		mtx[0][0]=1.0;	mtx[0][1]=0.0;	mtx[0][2]=0.0;
		mtx[1][0]=0.0;	mtx[1][1]=c;	mtx[1][2]=s;
		mtx[2][0]=0.0;	mtx[2][1]=-s;	mtx[2][2]=c;
		break;
	case 2:
		mtx[0][0]=c;	mtx[0][1]=0.0;	mtx[0][2]=-s;
		mtx[1][0]=0.0;	mtx[1][1]=1.0;	mtx[1][2]=0.0;
		mtx[2][0]=s;	mtx[2][1]=0.0;	mtx[2][2]=c;
		break;
	case 3:
		mtx[0][0]=c;	mtx[0][1]=s;	mtx[0][2]=0.0;
		mtx[1][0]=-s;	mtx[1][1]=c;	mtx[1][2]=0.0;
		mtx[2][0]=0.0;	mtx[2][1]=0.0;	mtx[2][2]=1.0;
		break;
	}
}


//********************************************************************
//������ת�ǶȺ���ת���������ת�ƾ���
//Author:	Wang Hua
//Date:		2004.11.24
//Input:	euler	Euler��
//			seqFlag	��תsequence
//Output:	mtx		���ص�ת�ƾ���
//********************************************************************
void AsMNamespace AsEulerToMtx (const CEuler& euler, int seqFlag, 
								CMatrix<double>& mtx)
{
	assert(seqFlag==123 || seqFlag==132 || seqFlag==121 || seqFlag==131 
		|| seqFlag==213 || seqFlag==231 || seqFlag==212 || seqFlag==232
		|| seqFlag==321 || seqFlag==312 || seqFlag==313 || seqFlag==323);

	euler.ToMtx(seqFlag, mtx);
}


//***********************************************************
//������ת�ǶȺ���ת���������ת�ƾ���
//Author:	Wang Hua
//Date:		2004.9.7
//Input:	angle	��ת�Ƕ�(����)
//			seqFlag	��ת���־:1=x,2=y,3=z
//			n		����Ľǵĸ���
//Output:	mtx		ת�ƾ���
//***********************************************************
void AsMNamespace AsEulerToMtx(const double angle[], 
			const int seqFlag[], int n, CMatrix<double>& mtx)
{
    if (n<=0)
    {
        mtx.MakeUnit();
        return;
    }

	CMatrix<double> m(3,3);
	AsEulerToMtx(angle[0],seqFlag[0],mtx);
	for(int i=1; i<n; i++)
	{
		AsEulerToMtx(angle[i],seqFlag[i],m);
		mtx = m*mtx;
	}
}


//***********************************************************
//������ת�ǶȺ���ת���������ת�ƾ���
//Author:	Wang Hua
//Date:		2004.9.7
//Input:	angle	��ת�Ƕ�(����)
//			seqFlag	��ת���־:1=x,2=y,3=z
//Output:	mtx		ת�ƾ���
//***********************************************************
void AsMNamespace AsEulerToMtx(const CVector<double>& angle, 
			const CVector<int>& seqFlag, CMatrix<double>& mtx)
{
	assert(angle.GetSize()==seqFlag.GetSize());
    AsEulerToMtx(angle.Buffer(), seqFlag.Buffer(), angle.GetSize(), mtx);
}


//***********************************************************
//����Euler�Ǽ���quaternion
//Author:	Wang Hua
//Date:		2004.11.24
//***********************************************************
void AsMNamespace AsEulerToQuat (const CEuler& euler, int seqFlag, 
			CQuaternion& quat)
{
	quat.FromEuler(euler, seqFlag);
}


//***********************************************************
//The cnoversion from Euler axis and angle to matrix.
//Author:	Wang Hua
//Date:		2004.10.13
//***********************************************************
void AsMNamespace AsAxAngToMtx(const CCoord3& axis, double angle, 
			CMatrix<double>& mtx)
{
	CCoord3 ax(axis);
	ax.Unit();
	double cc    = cos(angle);
	double ss    = sin(angle);
	double sigma = 1.0-cc;
	mtx[0][0] = ax[0]*ax[0]*sigma+cc;
	mtx[0][1] = ax[0]*ax[1]*sigma+ax[2]*ss;
	mtx[0][2] = ax[0]*ax[2]*sigma-ax[1]*ss;
	mtx[1][0] = ax[0]*ax[1]*sigma-ax[2]*ss;
	mtx[1][1] = ax[1]*ax[1]*sigma+cc;
	mtx[1][2] = ax[1]*ax[2]*sigma+ax[0]*ss;
	mtx[2][0] = ax[0]*ax[2]*sigma+ax[1]*ss;
	mtx[2][1] = ax[1]*ax[2]*sigma-ax[0]*ss;
	mtx[2][2] = ax[2]*ax[2]*sigma+cc;
}


//***********************************************************
//The cnoversion from axis and angle to quaternion.
//Author:	Wang Hua
//Date:		2004.11.24
//***********************************************************
void AsMNamespace AsAxAngToQuat(const CCoord3& axis, double angle, 
			CQuaternion& quat)
{
	quat.FromAxAng(axis, angle);
}


//***********************************************************
//The cnoversion from matrix to quaternion.
//Author:	Wang Hua
//Date:		2004.11.24
//***********************************************************
void AsMNamespace AsMtxToQuat (const CMatrix<double>& mtx, CQuaternion& quat)
{
	quat.FromMtx(mtx);
}


//***********************************************************
//The cnoversion from quaternion to axis and angle.
//Author:	Wang Hua
//Date:		2004.11.24
//***********************************************************
void AsMNamespace AsQuatToAxAng(const CQuaternion& quat, CCoord3& axis, 
			double& angle)
{
	quat.ToAxAng(axis, angle);
}


//***********************************************************
//The cnoversion from quaternion to matrix.
//Author:	Wang Hua
//Date:		2004.11.24
//***********************************************************
void AsMNamespace AsQuatToMtx(const CQuaternion& quat, CMatrix<double>& mtx)
{
	quat.ToMtx(mtx);
}


