////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

//AsVector.h
//
//////////////////////////////////////////////////////////////////////
/// @file
/// @brief	ʸ����CVectorͷ�ļ�


#if !defined(_ASVECTOR_H_)
#define _ASVECTOR_H_


#include <sstream>
#include "AsCommon.h"
#include "AsMath.h"





//////////////////////////////////////////////////////////////////////
//TEMPLATE CLASS CVector<_Type>
template <class _Type>
class CVector
{
	typedef CVector<_Type> _MyT;

public:
	//���캯����Ĭ������ά����
	explicit CVector(int n=3);
	CVector(const _Type& x, const _Type& y, const _Type& z);
	CVector(const _MyT& vec);
	virtual ~CVector();

	inline int			GetSize () const;					///< ��ͬ��Size()
	inline int			Size () const;						///< �õ�ʸ����ά��
	void				Resize  (int nDim);					///< ��������ά��

	void				SetData (int n, const _Type *value);///< ��һά��������ʸ��������
	void				GetData (int n, _Type *value) const;///< ��ʸ������д��һά����
	void				SetDataAll(const _Type& value);		///< ��������ʸ��Ԫ��ֵ

	inline _Type*		Data  ();							///< �õ�����ָ��
	inline const _Type*	Data  () const;					///< �õ�����ָ��
	inline   operator _Type*();								///< ת������Ϊָ����
	inline   operator const _Type*() const;					///< ת������Ϊָ����

	inline _Type&		operator[](int i);					///< ����[]�����
	inline const _Type&	operator[](int i) const;			///< ����[]�����

	bool	operator==(const _MyT &vec) const;				///< ��ʸ���Ƿ����
	bool	operator!=(const _MyT &vec) const;				///< ��ʸ���Ƿ����
	bool	operator >(const _MyT &vec) const;				///< ��һ��ʸ���Ƿ���ڵڶ���ʸ��
	bool	operator>=(const _MyT &vec) const;				///< ��һ��ʸ���Ƿ���ڵ��ڵڶ���ʸ��
	bool	operator <(const _MyT &vec) const;				///< ��һ��ʸ���Ƿ�С���ڵڶ���ʸ��
	bool	operator<=(const _MyT &vec) const;				///< ��һ��ʸ���Ƿ�С�ڵ��ڵڶ���ʸ��

	_MyT&	operator =(const _MyT& vec);					///< ʸ����ֵ

	inline const _MyT	operator +() const;					///< ʸ��ǰΪ����
	inline const _MyT	operator -() const;					///< ʸ��ǰΪ����
	inline const _MyT&	operator+=(const _MyT& vec);		///< ʸ�����
	inline const _MyT	operator +(const _MyT& vec) const;	///< ʸ�����
	inline const _MyT&	operator-=(const _MyT& vec);		///< ʸ�����
	inline const _MyT	operator -(const _MyT& vec) const;	///< ʸ�����
	inline const _MyT&	operator*=(const _MyT& vec);		///< ʸ�����
	inline const _MyT	operator *(const _MyT& vec) const;	///< ʸ�����
	inline const _MyT	operator *(const _Type& num) const;	///< ʸ������
	inline const _MyT&	operator*=(const _Type&  num);		///< ʸ������
	inline const _MyT&	operator/=(const _Type&  num);		///< ʸ������
	inline const _MyT	operator /(const _Type&  num) const;///< ʸ������

protected:
	void	Construct(int n);
	void	Destroy  ();

protected:
	_Type	m_Data[3];

protected:
	int		m_Size;
	_Type  *m_p;
};



//////////////////////////////////////////////////////////////////////
//TEMPLATE CLASS CVector<double>
template<>
class CVector<double>
{
	typedef double _Type;
	typedef CVector<double> _MyT;

public:

	explicit CVector(int n=3);
	CVector(_Type x, _Type y, _Type z);
	CVector(const _MyT& vec);
	virtual ~CVector();

	inline int			GetSize () const;					///< ��ͬ��Size()
	inline int			Size () const;						///< �õ�ʸ����ά��
	void				Resize(int n);						///< ��������ά��

	void				SetData (int n, const _Type *value);///< ��һά��������ʸ��������
	void				GetData (int n, _Type *value) const;///< ��ʸ������д��һά����
	void				SetDataAll(const _Type& value);		///< ��������ʸ��Ԫ��ֵ

	inline _Type*		Data  ();							///< �õ�����ָ��
	inline const _Type*	Data  () const;					    ///< �õ�����ָ��103645259
	inline   operator _Type*();								///< ת������Ϊָ����
	inline   operator const _Type*() const;					///< ת������Ϊָ����

	inline _Type&		operator[](int i);					///< ����[]�����
	inline const _Type&	operator[](int i) const;			///< ����[]�����

	bool	operator==(const _MyT &vec) const;				///< ��ʸ���Ƿ����
	bool	operator!=(const _MyT &vec) const;				///< ��ʸ���Ƿ����
	bool	operator >(const _MyT &vec) const;				///< ��һ��ʸ���Ƿ���ڵڶ���ʸ��
	bool	operator>=(const _MyT &vec) const;				///< ��һ��ʸ���Ƿ���ڵ��ڵڶ���ʸ��
	bool	operator <(const _MyT &vec) const;				///< ��һ��ʸ���Ƿ�С���ڵڶ���ʸ��
	bool	operator<=(const _MyT &vec) const;				///< ��һ��ʸ���Ƿ�С�ڵ��ڵڶ���ʸ��

	_MyT&	operator =(const _MyT& vec);					///< ʸ����ֵ

	inline const _MyT	operator +() const;					///< ʸ��ǰΪ����
	inline const _MyT	operator -() const;					///< ʸ��ǰΪ����
	inline const _MyT&	operator+=(const _MyT& vec);		///< ʸ�����
	inline const _MyT	operator +(const _MyT& vec) const;	///< ʸ�����
	inline const _MyT&	operator-=(const _MyT& vec);		///< ʸ�����
	inline const _MyT	operator -(const _MyT& vec) const;	///< ʸ�����
	inline const _MyT&	operator*=(const _MyT& vec);		///< ʸ�����
	inline const _MyT	operator *(const _MyT& vec) const;	///< ʸ�����
	inline const _MyT	operator *(_Type num) const;		///< ʸ������
	inline const _MyT&	operator*=(_Type num);				///< ʸ������
	inline const _MyT&	operator/=(_Type num);				///< ʸ������
	inline const _MyT	operator /(_Type num) const;		///< ʸ������

	double		Norm() const;								///< ʸ��������Ҳ����ʸ��ģ
	double		SqrNorm() const;							///< ʸ��������ƽ��
	void		Unit();										///< ��λ��ʸ��
	double		Dot(const CVector<double>& vec) const;			///< ���
	const _MyT	Cross(const CVector<double>& vec) const;		///< ���
	double		AngBetween(const CVector<double>& vec) const;	///< ����ʸ���ļн�[0,PI]

protected:
	void	Construct(int n);
	void	Destroy  ();

protected:
	_Type	m_Data[3];

protected:
	int		m_Size;
	_Type  *m_p;
};



//
//Global function.
//
const CVector<double> operator *  (double num, const CVector<double>& vec);			///< ����ʸ��
double	AsAngBetween(const CVector<double>& vec1, const CVector<double>& vec2);	///< ����ʸ���ļн�[0,PI]
template <class _Type>
std::ostream& operator<<(std::ostream &s, const CVector<_Type> &v);		///< ���
template <class _Type>
std::istream & operator>>(std::istream &s, CVector<_Type> &v);			///< ����






//
//����Ϊ����ʵ�ִ���
//

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//TEMPLATE CLASS CVector<_Type>

//���캯����Ĭ������ά����
template <class _Type>
CVector<_Type>::CVector(int n)
{
	Construct(n);
}

//��������ֵ����ʼ���Ĺ��캯������ά����
template <class _Type>
CVector<_Type>::CVector(const _Type& x, const _Type& y, const _Type& z)
{
	Construct(3);
	m_p[0] = x;
	m_p[1] = y;
	m_p[2] = z;
}


//�������캯��
template <class _Type>
CVector<_Type>::CVector(const CVector<_Type>& vec)
{
	Construct(vec.m_Size);
	for(int i=0; i<m_Size; i++)
		m_p[i] = vec.m_p[i];
}

//��������
template <class _Type>
CVector<_Type>::~CVector()
{
	Destroy();
}

template <class _Type>
void CVector<_Type>::Construct(int n)
{
	assert(n>=0);

	m_Size = n;

	if (m_Size<=3)
		m_p = m_Data;
	else {
		m_p = 0;
		m_p = new _Type[m_Size];
	}
}

template <class _Type>
void CVector<_Type>::Destroy()
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

//����ʸ���ߴ�
template <class _Type>
void CVector<_Type>::Resize(int nDim)
{
	assert(nDim>=0);

	if (nDim==m_Size)
		return;

	int minSize = AsMin(nDim, m_Size);
	CVector<_Type> v = *this;

	Destroy();
	Construct(nDim);

	for (int i=0; i<minSize; i++)
		m_p[i] = v.m_p[i];
}

//�õ�ʸ����ά��
template <class _Type>
inline int CVector<_Type>::GetSize() const
{
	return m_Size;
};


//�õ�ʸ����ά��
template <class _Type>
inline int CVector<_Type>::Size() const
{
	return m_Size;
};


//get the buffer of CVector
template <class _Type>
inline _Type* CVector<_Type>::Data()
{
	return m_p;
}


//get the buffer of CVector
template <class _Type>
inline const _Type* CVector<_Type>::Data() const 
{
	return m_p;
}


template <class _Type>
inline CVector<_Type>::operator _Type*()
{
	return m_p;
}


template <class _Type>
inline CVector<_Type>::operator const _Type*() const
{
	return m_p;
}


//����ʸ����ֵ
//Input  parameter:	value	�洢ʸ��Ԫ��ֵ��һά���飬����Ϊm_Size
template <class _Type>
void CVector<_Type>::SetData(int n, const _Type *value)
{
	assert (m_Size==n);

	for(int i=0; i<m_Size; i++)
		m_p[i] = value[i];
}

//�õ�ʸ����ֵ
//Output parameter:	value	����ʸ��Ԫ��ֵ��һά���飬����Ϊm_Size
template <class _Type>
void CVector<_Type>::GetData(int n, _Type *value) const
{
	assert (m_Size==n);

	for(int i=0; i<m_Size; i++)
		value[i] = m_p[i];
}

//����ʸ����ֵ
template <class _Type>
void CVector<_Type>::SetDataAll(const _Type& value)
{
	for(int i=0; i<m_Size; i++)
		m_p[i] = value;
}

//����ʸ����Ԫ�أ���0��ʼ
template <class _Type>
inline _Type & CVector<_Type>::operator[](int i)
{
	assert(i>=0 && i<m_Size); //Exceed the size of vector.
	return m_p[i];
}

template <class _Type>
inline const _Type& CVector<_Type>::operator[](int i) const
{
	assert((i>=0) && (i<m_Size));
	return m_p[i];
}

template <class _Type>
bool CVector<_Type>::operator ==(const CVector<_Type> &v) const
{
	assert(v.m_Size == m_Size);

	for(int i=0; i<m_Size; i++)
	{
		if (m_p[i] != v.m_p[i])
			return false;
	}
	return true;
}

template <class _Type>
bool CVector<_Type>::operator !=(const CVector<_Type> &v) const
{
	assert(v.m_Size == m_Size);

	for(int i=0; i<m_Size; i++)
	{
		if (m_p[i] != v.m_p[i])
			return true;
	}
	return false;
}

template <class _Type>
bool CVector<_Type>::operator >(const CVector<_Type> &v) const
{
	assert(v.m_Size == m_Size);

	for(int i=0; i<m_Size; i++)
	{
		if (m_p[i] <= v.m_p[i])
			return false;
	}
	return true;
}

template <class _Type>
bool CVector<_Type>::operator >=(const CVector<_Type> &v) const
{
	assert(v.m_Size == m_Size);

	for(int i=0; i<m_Size; i++)
	{
		if (m_p[i] < v.m_p[i])
			return false;
	}
	return true;
}

template <class _Type>
bool CVector<_Type>::operator <(const CVector<_Type> &v) const
{
	assert(v.m_Size == m_Size);

	for(int i=0; i<m_Size; i++)
	{
		if (m_p[i] >= v.m_p[i])
			return false;
	}
	return true;
}

template <class _Type>
bool CVector<_Type>::operator <=(const CVector<_Type> &v) const
{
	assert(v.m_Size == m_Size);

	for(int i=0; i<m_Size; i++)
	{
		if (m_p[i] > v.m_p[i])
			return false;
	}
	return true;
}

//��ʸ����ֵ
template <class _Type>
CVector<_Type> & CVector<_Type>::operator =(const CVector<_Type>& vec)
{
	if(this == &vec)
		return *this;

	if (vec.m_Size == m_Size)
	{
		for(int i=0; i<m_Size; i++)
			m_p[i] = vec.m_p[i];
	}
	else
	{
		Destroy();
		Construct(vec.m_Size);
		for(int i=0; i<m_Size; i++)
			m_p[i] = vec.m_p[i];
	}

	return *this;
}


//����ʸ����������
template<class _Type>
inline const CVector<_Type> CVector<_Type>::operator +() const
{
	return *this;
}


//ʸ���ۼ�
template<class _Type>
inline const CVector<_Type>& CVector<_Type>::operator +=(const CVector<_Type> &v)
{
	assert(v.m_Size==m_Size);
	for(int i=0; i<m_Size; i++)
		m_p[i] += v.m_p[i];
	return *this;
}

//ʸ����
template<class _Type>
inline const CVector<_Type> CVector<_Type>::operator +(const CVector<_Type>& v2) const
{
	assert(m_Size==v2.m_Size);
	CVector<_Type> v = *this;
	v += v2;
	return v;
}

//ʸ���۲�
template<class _Type>
inline const CVector<_Type>& CVector<_Type>::operator-=(const CVector<_Type>& v)
{
	assert(v.m_Size==m_Size);
	for(int i=0; i<m_Size; i++)
		m_p[i] -= v.m_p[i];
	return *this;
}

//ʸ����
template<class _Type>
inline const CVector<_Type> CVector<_Type>::operator -(const CVector<_Type>& v2) const
{
	assert(m_Size==v2.m_Size);
	CVector<_Type> v = *this;
	v -= v2;
	return v;
}

//����ʸ��������
template<class _Type>
inline const CVector<_Type> CVector<_Type>::operator -() const
{
	CVector<_Type> v = *this;
	for(int i=0; i<m_Size; i++)
		v.m_p[i] = -m_p[i];
	return v;
}


template<class _Type>
inline const CVector<_Type>& CVector<_Type>::operator *=(const CVector<_Type>& v2)
{
	assert(m_Size==3 && 3==v2.m_Size);
	_Type v[3];
	for (int i=0; i<3; i++)
		v[i] = m_p[i];
	m_p[0] = v[1]*v2.m_p[2]-v[2]*v2.m_p[1];
	m_p[1] = v[2]*v2.m_p[0]-v[0]*v2.m_p[2];
	m_p[2] = v[0]*v2.m_p[1]-v[1]*v2.m_p[0];
	return *this;
}

//��άʸ�����
template<class _Type>
inline const CVector<_Type> CVector<_Type>::operator *(const CVector<_Type>& v2) const
{
	assert(m_Size==3 && 3==v2.m_Size);
	CVector<_Type> Out = *this;
	Out *= v2;
	return Out;
}

//ʸ������
template<class _Type>
inline const CVector<_Type> CVector<_Type>::operator *(const _Type& Num) const
{
	CVector<_Type> Out = *this;
	Out *= Num;
	return Out;
}

//ʸ���۳���
template<class _Type>
inline const CVector<_Type>& CVector<_Type>::operator*=(const _Type& Num)
{
	for(int i=0; i<m_Size; i++)
		m_p[i] *= Num;
	return *this;
}

template<class _Type>
inline const CVector<_Type>& CVector<_Type>::operator/=(const _Type& Num)
{
	for(int i=0; i<m_Size; i++)
		m_p[i] /= Num;
	return *this;
}

template<class _Type>
inline const CVector<_Type>  CVector<_Type>::operator /(const _Type& dNum) const
{
	CVector<_Type> Out = *this;
	Out /= dNum;
	return Out;
}



//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//TEMPLATE CLASS CVector<double>

//�õ�ʸ����ά��
inline int CVector<double>::GetSize() const
{
	return m_Size;
};


//�õ�ʸ����ά��
inline int CVector<double>::Size() const
{
	return m_Size;
};


//get the buffer of CVector
inline double* CVector<double>::Data()
{
	return m_p;
}


//get the buffer of CVector
inline const double* CVector<double>::Data() const 
{
	return m_p;
}


inline CVector<double>::operator double*()
{
	return m_p;
}


inline CVector<double>::operator const double*() const
{
	return m_p;
}

//����ʸ����Ԫ�أ���0��ʼ
inline double & CVector<double>::operator[](int i)
{
	assert(i>=0 && i<m_Size); //Exceed the size of vector.
	return m_p[i];
}

inline const double& CVector<double>::operator[](int i) const
{
	assert((i>=0) && (i<m_Size));
	return m_p[i];
}

//����ʸ����������
inline const CVector<double> CVector<double>::operator +() const
{
	return *this;
}

//ʸ���ۼ�
inline const CVector<double>& CVector<double>::operator +=(const CVector<double> &v)
{
	assert(v.m_Size==m_Size);
	for(int i=0; i<m_Size; i++)
		m_p[i] += v.m_p[i];
	return *this;
}

//ʸ����
inline const CVector<double> CVector<double>::operator +(const CVector<double>& v2) const
{
	assert(m_Size==v2.m_Size);
	CVector<double> v = *this;
	v += v2;
	return v;
}

//ʸ���۲�
inline const CVector<double>& CVector<double>::operator-=(const CVector<double>& v)
{
	assert(v.m_Size==m_Size);
	for(int i=0; i<m_Size; i++)
		m_p[i] -= v.m_p[i];
	return *this;
}

//ʸ����
inline const CVector<double> CVector<double>::operator -(const CVector<double>& v2) const
{
	assert(m_Size==v2.m_Size);
	CVector<double> v = *this;
	v -= v2;
	return v;
}

//����ʸ��������
inline const CVector<double> CVector<double>::operator -() const
{
	CVector<double> v = *this;
	for(int i=0; i<m_Size; i++)
		v.m_p[i] = -m_p[i];
	return v;
}

inline const CVector<double>& CVector<double>::operator *=(const CVector<double>& v2)
{
	assert(m_Size==3 && 3==v2.m_Size);
	double v[3];
	memcpy(v, m_p, 3*sizeof(double));
	m_p[0] = v[1]*v2.m_p[2]-v[2]*v2.m_p[1];
	m_p[1] = v[2]*v2.m_p[0]-v[0]*v2.m_p[2];
	m_p[2] = v[0]*v2.m_p[1]-v[1]*v2.m_p[0];
	return *this;
}

//��άʸ�����
inline const CVector<double> CVector<double>::operator *(const CVector<double>& v2) const
{
	assert(m_Size==3 && 3==v2.m_Size);
	CVector<double> Out = *this;
	Out *= v2;
	return Out;
}

//ʸ������
inline const CVector<double> CVector<double>::operator *(double Num) const
{
	CVector<double> Out = *this;
	Out *= Num;
	return Out;
}

//ʸ���۳���
inline const CVector<double>& CVector<double>::operator*=(double Num)
{
	for(int i=0; i<m_Size; i++)
		m_p[i] *= Num;
	return *this;
}

inline const CVector<double>& CVector<double>::operator/=(double Num)
{
	for(int i=0; i<m_Size; i++)
		m_p[i] /= Num;
	return *this;
}

inline const CVector<double>  CVector<double>::operator /(_Type dNum) const
{
	CVector<double> Out = *this;
	Out /= dNum;
	return Out;
}


// TEMPLATE FUNCTION operator<<
template <class _Type>
std::ostream& operator<<(std::ostream &s, const CVector<_Type> &v)
{
    int N=v.Size();

    s <<  N << endl;

    for (int i=0; i<N; i++)
        s   << v[i] << " ";

    return s;
}


// TEMPLATE FUNCTION operator>>
template <class _Type>
std::istream & operator>>(std::istream &s, CVector<_Type> &v)
{

    int N;

    s >> N;

    if ( !(N == v.Size() ))
        v.Resize(N);

    for (int i=0; i<N; i++)
        s >>  v[i];

    return s;
}






#endif	//_ASVECTOR_H_





