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
/// @brief	矢量类CVector头文件


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
	//构造函数，默认是三维向量
	explicit CVector(int n=3);
	CVector(const _Type& x, const _Type& y, const _Type& z);
	CVector(const _MyT& vec);
	virtual ~CVector();

	inline int			GetSize () const;					///< 等同于Size()
	inline int			Size () const;						///< 得到矢量的维数
	void				Resize  (int nDim);					///< 重新设置维数

	void				SetData (int n, const _Type *value);///< 用一维数组设置矢量的数据
	void				GetData (int n, _Type *value) const;///< 将矢量数据写入一维数组
	void				SetDataAll(const _Type& value);		///< 设置所有矢量元素值

	inline _Type*		Data  ();							///< 得到数据指针
	inline const _Type*	Data  () const;					///< 得到数据指针
	inline   operator _Type*();								///< 转换类型为指针型
	inline   operator const _Type*() const;					///< 转换类型为指针型

	inline _Type&		operator[](int i);					///< 重载[]运算符
	inline const _Type&	operator[](int i) const;			///< 重载[]运算符

	bool	operator==(const _MyT &vec) const;				///< 两矢量是否相等
	bool	operator!=(const _MyT &vec) const;				///< 两矢量是否不相等
	bool	operator >(const _MyT &vec) const;				///< 第一个矢量是否大于第二个矢量
	bool	operator>=(const _MyT &vec) const;				///< 第一个矢量是否大于等于第二个矢量
	bool	operator <(const _MyT &vec) const;				///< 第一个矢量是否小于于第二个矢量
	bool	operator<=(const _MyT &vec) const;				///< 第一个矢量是否小于等于第二个矢量

	_MyT&	operator =(const _MyT& vec);					///< 矢量赋值

	inline const _MyT	operator +() const;					///< 矢量前为正号
	inline const _MyT	operator -() const;					///< 矢量前为负号
	inline const _MyT&	operator+=(const _MyT& vec);		///< 矢量相加
	inline const _MyT	operator +(const _MyT& vec) const;	///< 矢量相加
	inline const _MyT&	operator-=(const _MyT& vec);		///< 矢量相减
	inline const _MyT	operator -(const _MyT& vec) const;	///< 矢量相减
	inline const _MyT&	operator*=(const _MyT& vec);		///< 矢量叉乘
	inline const _MyT	operator *(const _MyT& vec) const;	///< 矢量叉乘
	inline const _MyT	operator *(const _Type& num) const;	///< 矢量乘数
	inline const _MyT&	operator*=(const _Type&  num);		///< 矢量乘数
	inline const _MyT&	operator/=(const _Type&  num);		///< 矢量除数
	inline const _MyT	operator /(const _Type&  num) const;///< 矢量除数

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

	inline int			GetSize () const;					///< 等同于Size()
	inline int			Size () const;						///< 得到矢量的维数
	void				Resize(int n);						///< 重新设置维数

	void				SetData (int n, const _Type *value);///< 用一维数组设置矢量的数据
	void				GetData (int n, _Type *value) const;///< 将矢量数据写入一维数组
	void				SetDataAll(const _Type& value);		///< 设置所有矢量元素值

	inline _Type*		Data  ();							///< 得到数据指针
	inline const _Type*	Data  () const;					    ///< 得到数据指针103645259
	inline   operator _Type*();								///< 转换类型为指针型
	inline   operator const _Type*() const;					///< 转换类型为指针型

	inline _Type&		operator[](int i);					///< 重载[]运算符
	inline const _Type&	operator[](int i) const;			///< 重载[]运算符

	bool	operator==(const _MyT &vec) const;				///< 两矢量是否相等
	bool	operator!=(const _MyT &vec) const;				///< 两矢量是否不相等
	bool	operator >(const _MyT &vec) const;				///< 第一个矢量是否大于第二个矢量
	bool	operator>=(const _MyT &vec) const;				///< 第一个矢量是否大于等于第二个矢量
	bool	operator <(const _MyT &vec) const;				///< 第一个矢量是否小于于第二个矢量
	bool	operator<=(const _MyT &vec) const;				///< 第一个矢量是否小于等于第二个矢量

	_MyT&	operator =(const _MyT& vec);					///< 矢量赋值

	inline const _MyT	operator +() const;					///< 矢量前为正号
	inline const _MyT	operator -() const;					///< 矢量前为负号
	inline const _MyT&	operator+=(const _MyT& vec);		///< 矢量相加
	inline const _MyT	operator +(const _MyT& vec) const;	///< 矢量相加
	inline const _MyT&	operator-=(const _MyT& vec);		///< 矢量相减
	inline const _MyT	operator -(const _MyT& vec) const;	///< 矢量相减
	inline const _MyT&	operator*=(const _MyT& vec);		///< 矢量叉乘
	inline const _MyT	operator *(const _MyT& vec) const;	///< 矢量叉乘
	inline const _MyT	operator *(_Type num) const;		///< 矢量乘数
	inline const _MyT&	operator*=(_Type num);				///< 矢量乘数
	inline const _MyT&	operator/=(_Type num);				///< 矢量除数
	inline const _MyT	operator /(_Type num) const;		///< 矢量除数

	double		Norm() const;								///< 矢量范数，也就是矢量模
	double		SqrNorm() const;							///< 矢量范数的平方
	void		Unit();										///< 单位化矢量
	double		Dot(const CVector<double>& vec) const;			///< 点乘
	const _MyT	Cross(const CVector<double>& vec) const;		///< 叉乘
	double		AngBetween(const CVector<double>& vec) const;	///< 两个矢量的夹角[0,PI]

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
const CVector<double> operator *  (double num, const CVector<double>& vec);			///< 数乘矢量
double	AsAngBetween(const CVector<double>& vec1, const CVector<double>& vec2);	///< 两个矢量的夹角[0,PI]
template <class _Type>
std::ostream& operator<<(std::ostream &s, const CVector<_Type> &v);		///< 输出
template <class _Type>
std::istream & operator>>(std::istream &s, CVector<_Type> &v);			///< 输入






//
//下面为具体实现代码
//

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//TEMPLATE CLASS CVector<_Type>

//构造函数，默认是三维向量
template <class _Type>
CVector<_Type>::CVector(int n)
{
	Construct(n);
}

//用三个数值来初始化的构造函数，三维向量
template <class _Type>
CVector<_Type>::CVector(const _Type& x, const _Type& y, const _Type& z)
{
	Construct(3);
	m_p[0] = x;
	m_p[1] = y;
	m_p[2] = z;
}


//拷贝构造函数
template <class _Type>
CVector<_Type>::CVector(const CVector<_Type>& vec)
{
	Construct(vec.m_Size);
	for(int i=0; i<m_Size; i++)
		m_p[i] = vec.m_p[i];
}

//析构函数
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

//调整矢量尺寸
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

//得到矢量的维数
template <class _Type>
inline int CVector<_Type>::GetSize() const
{
	return m_Size;
};


//得到矢量的维数
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


//设置矢量的值
//Input  parameter:	value	存储矢量元素值的一维数组，长度为m_Size
template <class _Type>
void CVector<_Type>::SetData(int n, const _Type *value)
{
	assert (m_Size==n);

	for(int i=0; i<m_Size; i++)
		m_p[i] = value[i];
}

//得到矢量的值
//Output parameter:	value	返回矢量元素值的一维数组，长度为m_Size
template <class _Type>
void CVector<_Type>::GetData(int n, _Type *value) const
{
	assert (m_Size==n);

	for(int i=0; i<m_Size; i++)
		value[i] = m_p[i];
}

//设置矢量的值
template <class _Type>
void CVector<_Type>::SetDataAll(const _Type& value)
{
	for(int i=0; i<m_Size; i++)
		m_p[i] = value;
}

//引用矢量的元素，从0开始
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

//用矢量赋值
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


//重载矢量求正运算
template<class _Type>
inline const CVector<_Type> CVector<_Type>::operator +() const
{
	return *this;
}


//矢量累加
template<class _Type>
inline const CVector<_Type>& CVector<_Type>::operator +=(const CVector<_Type> &v)
{
	assert(v.m_Size==m_Size);
	for(int i=0; i<m_Size; i++)
		m_p[i] += v.m_p[i];
	return *this;
}

//矢量加
template<class _Type>
inline const CVector<_Type> CVector<_Type>::operator +(const CVector<_Type>& v2) const
{
	assert(m_Size==v2.m_Size);
	CVector<_Type> v = *this;
	v += v2;
	return v;
}

//矢量累差
template<class _Type>
inline const CVector<_Type>& CVector<_Type>::operator-=(const CVector<_Type>& v)
{
	assert(v.m_Size==m_Size);
	for(int i=0; i<m_Size; i++)
		m_p[i] -= v.m_p[i];
	return *this;
}

//矢量差
template<class _Type>
inline const CVector<_Type> CVector<_Type>::operator -(const CVector<_Type>& v2) const
{
	assert(m_Size==v2.m_Size);
	CVector<_Type> v = *this;
	v -= v2;
	return v;
}

//重载矢量求负运算
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

//三维矢量叉积
template<class _Type>
inline const CVector<_Type> CVector<_Type>::operator *(const CVector<_Type>& v2) const
{
	assert(m_Size==3 && 3==v2.m_Size);
	CVector<_Type> Out = *this;
	Out *= v2;
	return Out;
}

//矢量数乘
template<class _Type>
inline const CVector<_Type> CVector<_Type>::operator *(const _Type& Num) const
{
	CVector<_Type> Out = *this;
	Out *= Num;
	return Out;
}

//矢量累乘数
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

//得到矢量的维数
inline int CVector<double>::GetSize() const
{
	return m_Size;
};


//得到矢量的维数
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

//引用矢量的元素，从0开始
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

//重载矢量求正运算
inline const CVector<double> CVector<double>::operator +() const
{
	return *this;
}

//矢量累加
inline const CVector<double>& CVector<double>::operator +=(const CVector<double> &v)
{
	assert(v.m_Size==m_Size);
	for(int i=0; i<m_Size; i++)
		m_p[i] += v.m_p[i];
	return *this;
}

//矢量加
inline const CVector<double> CVector<double>::operator +(const CVector<double>& v2) const
{
	assert(m_Size==v2.m_Size);
	CVector<double> v = *this;
	v += v2;
	return v;
}

//矢量累差
inline const CVector<double>& CVector<double>::operator-=(const CVector<double>& v)
{
	assert(v.m_Size==m_Size);
	for(int i=0; i<m_Size; i++)
		m_p[i] -= v.m_p[i];
	return *this;
}

//矢量差
inline const CVector<double> CVector<double>::operator -(const CVector<double>& v2) const
{
	assert(m_Size==v2.m_Size);
	CVector<double> v = *this;
	v -= v2;
	return v;
}

//重载矢量求负运算
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

//三维矢量叉积
inline const CVector<double> CVector<double>::operator *(const CVector<double>& v2) const
{
	assert(m_Size==3 && 3==v2.m_Size);
	CVector<double> Out = *this;
	Out *= v2;
	return Out;
}

//矢量数乘
inline const CVector<double> CVector<double>::operator *(double Num) const
{
	CVector<double> Out = *this;
	Out *= Num;
	return Out;
}

//矢量累乘数
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





