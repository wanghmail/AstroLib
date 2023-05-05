////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2006                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsVector.cpp
//
////////////////////


#include "AsVector.h"





//构造函数，默认是三维向量
CVector<double>::CVector(int n)//n=3
{
	Construct(n);
	memset(m_p, 0, m_Size*sizeof(double));
}

//用三个数值来初始化的构造函数，三维向量
CVector<double>::CVector(double x, double y, double z)
{
	Construct(3);
	m_p[0] = x;
	m_p[1] = y;
	m_p[2] = z;
}

//拷贝构造函数
CVector<double>::CVector(const CVector<double>& vec)
{
	Construct(vec.m_Size);
	memcpy(m_p, vec.m_p, sizeof(double)*m_Size);
}

//析构函数
CVector<double>::~CVector()
{
	Destroy();
}


void CVector<double>::Construct(int n)
{
	m_Size = n<0 ? 0 : n;

	if (m_Size<=3)
		m_p = m_Data;
	else {
		m_p = 0;
		m_p = new double[m_Size];
		assert(m_p != 0);
	}
}


void CVector<double>::Destroy()
{
	if (m_Size<=3) {
		m_Size = 0;
		m_p    = 0;
		return;
	}
	else {
		m_Size = 0;
		delete[] m_p;
		m_p = 0;
	}
}


void CVector<double>::Resize(int n)
{
	if (n<0)
		n = 0;

	if (n==m_Size)
		return;

	int minSize = AsMin(n, m_Size);
	CVector<_Type> v = *this;

	Destroy();
	Construct(n);

	memcpy(m_p, v.m_p, minSize*sizeof(double));
}


//设置矢量的值
//Input  parameter:	value	存储矢量元素值的一维数组，长度为m_Size
void CVector<double>::SetData(int n, const double *value)
{
	assert (m_Size==n);

	memcpy(m_p, value, sizeof(double)*n);
}

//得到矢量的值
//Output parameter:	value	返回矢量元素值的一维数组，长度为m_Size
void CVector<double>::GetData(int n, double *value) const
{
	assert (m_Size==n);

	memcpy(value, m_p, sizeof(double)*n);
}

//设置矢量的值
void CVector<double>::SetDataAll(const double& value)
{
	for(int i=0; i<m_Size; i++)
		m_p[i] = value;
}


bool CVector<double>::operator ==(const CVector<double> &v) const
{
	assert(v.m_Size == m_Size);

	for(int i=0; i<m_Size; i++)
	{
		if (m_p[i] != v.m_p[i])
			return false;
	}
	return true;
}

bool CVector<double>::operator !=(const CVector<double> &v) const
{
	assert(v.m_Size == m_Size);

	for(int i=0; i<m_Size; i++)
	{
		if (m_p[i] != v.m_p[i])
			return true;
	}
	return false;
}

bool CVector<double>::operator >(const CVector<double> &v) const
{
	assert(v.m_Size == m_Size);

	for(int i=0; i<m_Size; i++)
	{
		if (m_p[i] <= v.m_p[i])
			return false;
	}
	return true;
}

bool CVector<double>::operator >=(const CVector<double> &v) const
{
	assert(v.m_Size == m_Size);

	for(int i=0; i<m_Size; i++)
	{
		if (m_p[i] < v.m_p[i])
			return false;
	}
	return true;
}

bool CVector<double>::operator <(const CVector<double> &v) const
{
	assert(v.m_Size == m_Size);

	for(int i=0; i<m_Size; i++)
	{
		if (m_p[i] >= v.m_p[i])
			return false;
	}
	return true;
}

bool CVector<double>::operator <=(const CVector<double> &v) const
{
	assert(v.m_Size == m_Size);

	for(int i=0; i<m_Size; i++)
	{
		if (m_p[i] > v.m_p[i])
			return false;
	}
	return true;
}

//用矢量赋值，如果当前矢量维数与输入矢量不同，则改变当前矢量维数
CVector<double> & CVector<double>::operator =(const CVector<double>& vec)
{
	if(this == &vec)
		return *this;

	if (vec.m_Size != m_Size)
	{
		Destroy();
		Construct(vec.m_Size);
	}
	memcpy(m_p, vec.m_p, m_Size*sizeof(double));

	return *this;
}


//得到矢量的模
double CVector<double>::Norm() const 
{
	double s = 0;
	for (int i=0; i<m_Size; i++)
		s += m_p[i]*m_p[i];
	return sqrt(s);
}


//得到矢量的模的平方
double CVector<double>::SqrNorm() const 
{
	double s = 0;
	for (int i=0; i<m_Size; i++)
		s += m_p[i]*m_p[i];
	return s;
}


//矢量单位化
void CVector<double>::Unit()
{
	double norm = Norm();

	assert (norm!=0);

	(*this) /= norm;
}


//点积
double CVector<double>::Dot(const CVector<double>& v2) const
{
	assert(m_Size==v2.m_Size);

	double Rt=0;
	for(int i=0; i<m_Size; i++)
		Rt += m_p[i]*v2.m_p[i];
	return Rt;
}


//三维矢量叉积
const CVector<double> CVector<double>::Cross(const CVector<double>& v2) const
{
	assert(m_Size==3 && 3==v2.Size());
	CVector<double> Out(3);
	Out.m_p[0] = m_p[1]*v2.m_p[2]-m_p[2]*v2.m_p[1];
	Out.m_p[1] = m_p[2]*v2.m_p[0]-m_p[0]*v2.m_p[2];
	Out.m_p[2] = m_p[0]*v2.m_p[1]-m_p[1]*v2.m_p[0];
	return Out;
}


//三维矢量的夹角(0..PI)
double CVector<double>::AngBetween(const CVector<double> &v) const
{
	double w1 = Norm();
	double w2 = v.Norm();

	assert( this->Size() == 3 && w1 != 0  && 
			    v.Size() == 3 && w2 != 0 );

	return acos( Dot(v)/(w1*w2) );
}



//////////////////////////////////////////////////////////////////////
//Global function.

//数乘矢量
const CVector<double>  operator *(double num, const CVector<double>& v)
{
	CVector<double> out(v);
	out *= num;
	return out;
}


//三维矢量的夹角(0..PI)
double  AsAngBetween(const CVector<double> &v1, const CVector<double> &v2)
{
	double w1 = v1.Norm();
	double w2 = v2.Norm();

	assert( v1.Size() == 3  &&  w1 != 0 && 
			v2.Size() == 3  &&  w2 != 0 );

	return acos( v1.Dot(v2)/(w1*w2) );
}



