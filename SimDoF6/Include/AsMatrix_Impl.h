////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

//AsMatrix_Impl.h
//The template and inline function of CMatrix.
//
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//TEMPLATE CLASS _CMatrix_base<_Type>

// 默认的构造函数，三阶矩阵
template <class _Type>
_CMatrix_base<_Type>::_CMatrix_base()
{
	Construct(3, 3);
}


template <class _Type>
_CMatrix_base<_Type>::_CMatrix_base(const _Type& d11, const _Type& d12, const _Type& d13, 
								 const _Type& d21, const _Type& d22, const _Type& d23, 
								 const _Type& d31, const _Type& d32, const _Type& d33)
{
	Construct(3, 3);
	this->operator()(0,0) = d11;
	this->operator()(0,1) = d12;
	this->operator()(0,2) = d13;
	this->operator()(1,0) = d21;
	this->operator()(1,1) = d22;
	this->operator()(1,2) = d23;
	this->operator()(2,0) = d31;
	this->operator()(2,1) = d32;
	this->operator()(2,2) = d33;
}


// 指定行列构造函数
template <class _Type>
_CMatrix_base<_Type>::_CMatrix_base(int nRow, int nCol)
{
	Construct(nRow, nCol);
}


// 拷贝构造函数
template <class _Type>
_CMatrix_base<_Type>::_CMatrix_base(const _CMatrix_base<_Type>& other)
{
	Construct(other.m_Row, other.m_Col);

	for(int i=0; i<m_Row; i++)
	{
		for(int j=0; j<m_Col; j++)
			m_pp[i][j] = other[i][j];
	}
}


template <class _Type>
_CMatrix_base<_Type>::~_CMatrix_base()
{
	Destroy();
}


template <class _Type>
void	_CMatrix_base<_Type>::Construct(int nRow, int nCol)
{
	m_Row = nRow<0 ? 0 : nRow;
	m_Col = nCol<0 ? 0 : nCol;
	if (nRow*nCol <= 9)
	{
		for (int i=0; i<m_Row; i++)
			m_pData[i] = &m_Data[i*m_Col];
		m_pp = m_pData;
	}
	else
	{
		m_pp = 0;
		m_pp = new _Type*[m_Row];
		assert(m_pp != 0);
		m_pp[0] = 0;
		m_pp[0] = new _Type[m_Row*m_Col];
		assert(m_pp[0] != 0);
		for(int i=1; i<m_Row; i++)
			m_pp[i] = m_pp[i-1]+m_Col;
	}
}


template <class _Type>
void	_CMatrix_base<_Type>::Destroy()
{
	if (m_Row*m_Col<=9)
	{
		m_Row = 0;
		m_Col = 0;
		m_pp  = 0;
		return;
	}
	else
	{
		m_Row = 0;
		m_Col = 0;
		delete[] m_pp[0];
		delete[] m_pp;
		m_pp = 0;
	}
}


template <class _Type>
inline int	_CMatrix_base<_Type>::GetSizeCol() const
{
	return m_Col;
}

template <class _Type>
inline int	_CMatrix_base<_Type>::GetSizeRow() const
{
	return m_Row;
}

//重载[]运算符，得到矩阵相应位置的值
template <class _Type>
inline const _Type* _CMatrix_base<_Type>::operator [](int n)const
{
	assert(n>=0 && n<m_Row);
	return m_pp[n];
}

template <class _Type>
inline _Type* _CMatrix_base<_Type>::operator [](int n)
{
	assert(n>=0 && n<m_Row);
	return m_pp[n];
}


template <class _Type>
inline const _Type& _CMatrix_base<_Type>::operator ()(int nRow,int nCol) const
{
	assert(nCol>=0 && nCol<m_Col && nRow>=0 && nRow<m_Row);

	return m_pp[nRow][nCol];
}

template <class _Type>
inline _Type& _CMatrix_base<_Type>::operator ()(int nRow,int nCol)
{
	assert(nCol>=0 && nCol<m_Col && nRow>=0 && nRow<m_Row);

	return m_pp[nRow][nCol];
}

//////////////////////////////////////////////////////////////////////
// 重载运算符==，判断矩阵是否相等
//
// 参数：
// 1. const CMatrix& other - 用于比较的矩阵
//
// 返回值：BOOL 型，两个矩阵相等则为TRUE，否则为FALSE
//////////////////////////////////////////////////////////////////////
template <class _Type>
inline bool _CMatrix_base<_Type>::operator==(const _CMatrix_base<_Type>& other) const
{
	// 首先检查行列数是否相等
	if (m_Col != other.GetSizeCol() || m_Row != other.GetSizeRow())
		return false;

	for (int i=0; i<m_Row; ++i)
	{
		for (int j=0; j<m_Col; ++j)
		{
			if (m_pp[i][j] != other.m_pp[i][j])
				return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// 重载运算符!=，判断矩阵是否不相等
//
// 参数：
// 1. const CMatrix& other - 用于比较的矩阵
//
// 返回值：BOOL 型，两个不矩阵相等则为TRUE，否则为FALSE
//////////////////////////////////////////////////////////////////////
template <class _Type>
inline bool _CMatrix_base<_Type>::operator!=(const _CMatrix_base<_Type>& other) const
{
	// 首先检查行列数是否相等
	if (m_Col == other.GetSizeCol() || m_Row == other.GetSizeRow())
		return false;

	for (int i=0; i<m_Row; ++i)
	{
		for (int j=0; j<m_Col; ++j)
		{
			if (m_pp[i][j] == other.m_pp[i][j])
				return false;
		}
	}

	return true;
}


template <class _Type>
void _CMatrix_base<_Type>::Resize(int nRow, int nCol)
{
	if (nRow<0)
		nRow = 0;
	if (nCol<0)
		nCol = 0;

	if(nRow==m_Row && nCol==m_Col)
		return;

	Destroy();
	Construct(nRow, nCol);	
}


template <class _Type>
inline _CMatrix_base<_Type>::operator _Type**()
{
	return m_pp;
}


template <class _Type>
inline _CMatrix_base<_Type>::operator const _Type* const *() const
{
	return m_pp;
}


//get the buffer of CMatrix
template <class _Type>
inline _Type** _CMatrix_base<_Type>::Data()
{
	return m_pp;
}


//get the buffer of CMatrix
template <class _Type>
inline const _Type* const* _CMatrix_base<_Type>::Data() const
{
	return m_pp;
}


//////////////////////////////////////////////////////////////////////
// 设置矩阵各元素的值
//
//Input  parameter:	value	一维数组，长度为m_Col*m_Row，存储
//                     矩阵各元素的值
//
// 返回值：无
//////////////////////////////////////////////////////////////////////
template <class _Type>
void _CMatrix_base<_Type>::SetData(int n, const _Type *value)
{
	assert(n==m_Row*m_Col);

	for(int i=0; i<m_Row; i++)
	{
		for(int j=0; j<m_Col; j++)
			m_pp[i][j] = value[m_Col*i+j];
	}
}


//********************************************************************
//得到矩阵的值
//Output parameter:	value	一维数组，长度为m_Col*m_Row，以一维数组表示的矩阵值
//********************************************************************
template <class _Type>
void _CMatrix_base<_Type>::GetData(int n, _Type *value) const
{
	assert(n==m_Row*m_Col);

	for(int i=0; i<m_Row; i++)
	{
		for(int j=0; j<m_Col; j++)
			value[m_Col*i+j] = m_pp[i][j];
	}
}

template <class _Type>
void _CMatrix_base<_Type>::SetData(int m, int n, const _Type * const *value)
{
	assert(m==m_Row && n==m_Col);

	for(int i=0; i<m_Row; i++)
	{
		for(int j=0; j<m_Col; j++)
			m_pp[i][j] = value[i][j];
	}
}

template <class _Type>
void _CMatrix_base<_Type>::GetData(int m, int n, _Type **value) const
{
	assert(m==m_Row && n==m_Col);

	for(int i=0; i<m_Row; i++)
	{
		for(int j=0; j<m_Col; j++)
			value[i][j] = m_pp[i][j];
	}
}

template <class _Type>
void _CMatrix_base<_Type>::SetDataAll(const _Type& value)
{
	for(int i=0; i<m_Row; i++)
	{
		for(int j=0; j<m_Col; j++)
			m_pp[i][j] = value;
	}
}


//********************************************************************
/// 获取指定行的向量
/// @Author	Wang Hua
/// @Date	2008.4.17
/// @Input
/// @Param  nRow    获取的行数   
/// @In/Out
/// @Param	pVector	返回获取的向量，需要事先分配好内存
//********************************************************************
template <class _Type>
void _CMatrix_base<_Type>::GetRowVector(int nRow, _Type* pVector) const
{
	assert(pVector != 0);

	for (int j=0; j<m_Col; ++j)
		pVector[j] = m_pp[nRow][j];
}


//********************************************************************
/// 获取指定列的向量
/// @Author	Wang Hua
/// @Date	2008.4.17
/// @Input
/// @Param  nRow    获取的列数   
/// @In/Out
/// @Param	pVector	返回获取的向量，需要事先分配好内存
//********************************************************************
template <class _Type>
void _CMatrix_base<_Type>::GetColVector(int nCol, _Type* pVector) const
{
	assert(pVector != 0);

	for (int i=0; i<m_Row; ++i)
		pVector[i] = m_pp[i][nCol];
}


template <class _Type>
void _CMatrix_base<_Type>::GetRowVector(int nRow, CVector<_Type> &vec) const
{
	assert(vec.GetSize() == m_Col);

	for (int j=0; j<m_Col; ++j)
		vec[j] = m_pp[nRow][j];
}


template <class _Type>
void _CMatrix_base<_Type>::GetColVector(int nCol, CVector<_Type> &vec) const
{
	assert(vec.GetSize() == m_Row);

	for (int i=0; i<m_Row; ++i)
		vec[i] = m_pp[i][nCol];
}


//********************************************************************
/// 获取指定子矩阵
/// @Author	Wang Hua
/// @Date	2008.4.23
/// @Input
/// @Param  begRow      开始取子矩阵的行数
/// @Param  begCol      开始取子矩阵的列数
/// @Param  sizeRow     子矩阵的行数
/// @Param  sizeCol     子矩阵的列数
/// @In/Out
/// @Param	pVector	返回获取的向量，需要事先分配好内存
//********************************************************************
template <class _Type>
void _CMatrix_base<_Type>::GetSubMatrix(int begRow, int begCol, int sizeRow, int sizeCol, _Type** pp) const
{
	assert(pp != 0);

	for (int i=0; i<sizeRow; i++)
        for (int j=0; j<sizeCol; j++)
    		pp[i][j] = m_pp[begRow+i][begCol+j];
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//TEMPLATE CLASS CMatrix<_Type>


template <class _Type>
CMatrix<_Type>::CMatrix() : _CMatrix_base<_Type>()
{
}

template <class _Type>
CMatrix<_Type>::CMatrix(const _Type& d11, const _Type& d12, const _Type& d13, 
					 const _Type& d21, const _Type& d22, const _Type& d23, 
					 const _Type& d31, const _Type& d32, const _Type& d33)
					: _CMatrix_base<_Type>(d11, d12, d13, d21, d22, d23, d31, d32, d33)
{
}

template <class _Type>
CMatrix<_Type>:: CMatrix(int nRows, int nCols) : _CMatrix_base<_Type>(nRows,nCols)
{
}

template <class _Type>
CMatrix<_Type>::CMatrix(const CMatrix<_Type>& other) : _CMatrix_base<_Type>(other)
{
}

template <class _Type>
CMatrix<_Type>::CMatrix(const _CMatrix_base<_Type>& mat) : _CMatrix_base<_Type>(mat)
{
}

template <class _Type>
CMatrix<_Type>::~CMatrix()
{
}

//////////////////////////////////////////////////////////////////////
// 重载运算符=，给矩阵赋值
//
// 参数：
// 1. const CMatrix& other - 用于给矩阵赋值的源矩阵
//
// 返回值：CMatrix型的引用，所引用的矩阵与other相等
//////////////////////////////////////////////////////////////////////
template <class _Type>
CMatrix<_Type>& CMatrix<_Type>::operator=(const CMatrix<_Type>& other)
{
	assert(m_Row==other.GetSizeRow() && m_Col==other.GetSizeCol());

	if (this==&other)
		return *this;

	for (i=0; i<m_Row; i++)
	{
		for(int j=0; j<m_Col; j++)
			m_pp[i][j] = other[i][j];
	}

	return *this ;
}


// 矩阵的转置
template <class _Type>
const CMatrix<_Type> CMatrix<_Type>::Transpose() const
{
	// 构造目标矩阵
	CMatrix<_Type> Trans(m_Col, m_Row);

	// 转置各元素
	for (int i = 0 ; i < m_Row ; ++i)
	{
		for (int j = 0 ; j < m_Col ; ++j)
			Trans.m_pp[j][i] = m_pp[i][j];
	}

	return Trans;
}


template <class _Type>
inline const CMatrix<_Type>& CMatrix<_Type>::operator+=(const CMatrix<_Type>& other)
{
	assert (m_Col == other.GetSizeCol() && m_Row == other.GetSizeRow());

	for (int i = 0 ; i < m_Row ; ++i)
	{
		for (int j = 0 ; j <  m_Col; ++j)
			m_pp[i][j] += other.m_pp[i][j] ;
	}

	return *this ;
}

//////////////////////////////////////////////////////////////////////
// 重载运算符+，实现矩阵的加法
//
// 参数：
// 1. const CMatrix& other - 与指定矩阵相加的矩阵
//
// 返回值：CMatrix型，指定矩阵与other相加之和
//////////////////////////////////////////////////////////////////////
template <class _Type>
inline const CMatrix<_Type> CMatrix<_Type>::operator+(const CMatrix<_Type>& other) const
{
	assert (m_Col == other.GetSizeCol() && m_Row == other.GetSizeRow());

	CMatrix<_Type> result = *this;
	result += other;
	return result ;
}

template <class _Type>
inline const CMatrix<_Type>& CMatrix<_Type>::operator-=(const CMatrix<_Type>& other)
{
	assert (m_Col == other.GetSizeCol() && m_Row == other.GetSizeRow());

	for (int i=0 ; i<m_Row ; ++i)
	{
		for (int j=0 ; j< m_Col; ++j)
			m_pp[i][j] -= other.m_pp[i][j] ;
	}

	return *this ;
}

//////////////////////////////////////////////////////////////////////
// 重载运算符-，实现矩阵的减法
//
// 参数：
// 1. const CMatrix& other - 与指定矩阵相减的矩阵
//
// 返回值：CMatrix型，指定矩阵与other相减之差
//////////////////////////////////////////////////////////////////////
template <class _Type>
inline const CMatrix<_Type> CMatrix<_Type>::operator-(const CMatrix<_Type>& other) const
{
	assert (m_Col == other.GetSizeCol() && m_Row == other.GetSizeRow());

	CMatrix<_Type> result = *this;
	result -= other;
	return result ;
}

//重载运算符-，实现求矩阵的负
template <class _Type>
inline const CMatrix<_Type> CMatrix<_Type>::operator-() const
{
	CMatrix<_Type> m = *this;
	for (int i=0; i<m_Row; ++i)
	{
		for (int j=0; j<m_Col; ++j)
			m.m_pp[i][j] = -m_pp[i][j];
	}
	return m;
}

//////////////////////////////////////////////////////////////////////
// 重载运算符*，实现矩阵的乘法
//
// 参数：
// 1. const CMatrix& other - 与指定矩阵相乘的矩阵
//
// 返回值：CMatrix型，指定矩阵与other相乘之积
//////////////////////////////////////////////////////////////////////
template <class _Type>
inline const CMatrix<_Type> CMatrix<_Type>::operator*(const CMatrix<_Type>& other) const
{
	assert (m_Col == other.GetSizeRow());

	// construct the object we are going to return
	CMatrix<_Type>	result(m_Row, other.GetSizeCol()) ;

	double	value ;
	for (int i=0 ; i<result.GetSizeRow(); ++i)
	{
		for (int j=0 ; j<other.GetSizeCol(); ++j)
		{
			value = 0.0 ;
			for (int k=0 ; k<m_Col; ++k)
			{
				value += m_pp[i][k]*other.m_pp[k][j] ;
			}
			result.m_pp[i][j] = value ;
		}
	}

	return result ;
}


//////////////////////////////////////////////////////////////////////
// 重载运算符*，实现矩阵和矢量的乘法
//
// 参数：
// 1. const double& vec - 与指定矩阵相乘的矢量
//
// 返回值：相乘之积
//////////////////////////////////////////////////////////////////////
template <class _Type>
inline const CVector<_Type>	CMatrix<_Type>::operator*(const CVector<_Type>& vec) const
{
	assert (m_Col == vec.GetSize());

	// construct the object we are going to return
	CVector<_Type>	result(m_Col);

	double	value ;
	for (int i = 0 ; i < m_Row; ++i)
	{
		value = 0.0 ;
		for (int j = 0 ; j < m_Col; ++j)
			value += m_pp[i][j] * vec[j];			
		result[i] = value;
	}

	return result ;
}

template <class _Type>
inline const CMatrix<_Type>& CMatrix<_Type>::operator*=(const _Type& other)
{
	for (int i=0 ; i<m_Row ; ++i)
	{
		for (int j=0 ; j< m_Col; ++j)
			m_pp[i][j] *= other;
	}
	return *this ;
}

//矩阵乘数
template <class _Type>
inline const CMatrix<_Type>  CMatrix<_Type>::operator*(const _Type& dNum) const
{
	CMatrix<_Type> result = *this;
	result *= dNum;
	return result;
}

//数乘矩阵
template <class _Type>
inline const CMatrix<_Type> operator*(const _Type& dNum, const CMatrix<_Type>& m)	
{
	CMatrix<_Type> result = m;
	result *= dNum;
	return result;
}

template <class _Type>
inline const CMatrix<_Type>&  CMatrix<_Type>::operator/=(const _Type& dNum)					
{
	for(int i=0; i<m_Row; i++)
		for(int j=0; j<m_Col; j++)
			m_pp[i][j] = m_pp[i][j]/dNum;
	return *this;

}

//矩阵除数
template <class _Type>
inline const CMatrix<_Type>  CMatrix<_Type>::operator/(const _Type& dNum) const						
{
	CMatrix<_Type> result = *this;
	result /= dNum;
	return result;

}

template <class _Type>
inline const CMatrix<_Type>& CMatrix<_Type>::operator/=(const CMatrix<_Type>& m)
{
	assert(m.m_Col==1 && m.m_Row==1);
	double dNum = m.m_pp[0][0];
	return operator/=(dNum);
}

template <class _Type>
inline const CMatrix<_Type> CMatrix<_Type>::operator/(const CMatrix<_Type>& m) const
{
	assert(m.m_Col==1 && m.m_Row==1);
	double dNum = m.m_pp[0][0];
	return operator/(dNum);
}



//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//TEMPLATE CLASS CMatrix<double>


inline const CMatrix<double>& CMatrix<double>::operator+=(const CMatrix<double>& other)
{
	assert (m_Col == other.GetSizeCol() && m_Row == other.GetSizeRow());

	for (int i=0 ; i<m_Row ; ++i)
	{
		for (int j=0 ; j<m_Col; ++j)
			m_pp[i][j] += other.m_pp[i][j] ;
	}

	return *this ;
}

//////////////////////////////////////////////////////////////////////
// 重载运算符+，实现矩阵的加法
//
// 参数：
// 1. const CMatrix& other - 与指定矩阵相加的矩阵
//
// 返回值：CMatrix型，指定矩阵与other相加之和
//////////////////////////////////////////////////////////////////////
inline const CMatrix<double> CMatrix<double>::operator+(const CMatrix<double>& other) const
{
	assert (m_Col == other.GetSizeCol() && m_Row == other.GetSizeRow());

	CMatrix<double> result = *this;
	result += other;
	return result ;
}

inline const CMatrix<double>& CMatrix<double>::operator-=(const CMatrix<double>& other)
{
	assert (m_Col == other.GetSizeCol() && m_Row == other.GetSizeRow());

	for (int i=0 ; i<m_Row ; ++i)
	{
		for (int j=0 ; j< m_Col; ++j)
			m_pp[i][j] -= other.m_pp[i][j] ;
	}

	return *this ;
}

// 重载运算符-，实现矩阵的减法
inline const CMatrix<double> CMatrix<double>::operator-(const CMatrix<double>& other) const
{
	assert (m_Col == other.GetSizeCol() && m_Row == other.GetSizeRow());

	CMatrix<double> result = *this;
	result -= other;
	return result ;
}

//********************************************************************
//重载运算符-，实现求矩阵的负
//********************************************************************
inline const CMatrix<double> CMatrix<double>::operator-() const
{
	CMatrix<double> m = *this;
	for (int i=0; i<m_Row ; ++i)
	{
		for (int j=0; j<m_Col; ++j)
			m.m_pp[i][j] = -m_pp[i][j];
	}
	return m;
}


inline const CMatrix<double>& CMatrix<double>::operator*=(const _MyT& other)
{
	assert (m_Col == other.GetSizeRow() && m_Row==m_Col && m_Row==other.GetSizeCol());

	CMatrix<double> result(m_Row, m_Row) ;
	double	value ;

	for (int i=0 ; i<m_Row; ++i)
	{
		for (int j=0 ; j<m_Row; ++j)
		{
			value = 0.0 ;
			for (int k=0 ; k<m_Col; ++k)
				value += m_pp[i][k]*other.m_pp[k][j] ;
			result.m_pp[i][j] = value ;
		}
	}

	*this = result;
	return *this ;
}


// 重载运算符*，实现矩阵的乘法
inline const CMatrix<double> CMatrix<double>::operator*(const CMatrix<double>& other) const
{
	assert (m_Col == other.GetSizeRow());

	// construct the object we are going to return
	CMatrix<double> result(m_Row, other.GetSizeCol()) ;
	double	value ;

	for (int i=0 ; i<m_Row; ++i)
	{
		for (int j=0 ; j<other.GetSizeCol(); ++j)
		{
			value = 0.0 ;
			for (int k=0 ; k<m_Col; ++k)
				value += m_pp[i][k]*other.m_pp[k][j] ;
			result.m_pp[i][j] = value ;
		}
	}

	return result ;
}


// 重载运算符*，实现矩阵和矢量的乘法
inline const CVector<double> CMatrix<double>::operator*(const CVector<double>& vec) const
{
	assert (m_Col == vec.GetSize());

	// construct the object we are going to return
	CVector<double>	result(m_Row);

	for (int i = 0 ; i < m_Row; ++i)
	{
		double value = 0.0 ;
		for (int j = 0 ; j < m_Col; ++j)
			value += m_pp[i][j] * vec[j];			
		result[i] = value;
	}

	return result ;
}


inline const CMatrix<double>& CMatrix<double>::operator*=(double other)
{
	for (int i=0 ; i<m_Row ; ++i)
	{
		for (int j=0 ; j< m_Col; ++j)
			m_pp[i][j] *= other;
	}
	return *this ;
}


//矩阵乘数
inline const CMatrix<double>  CMatrix<double>::operator*(double dNum) const
{
	CMatrix<double> result = *this;
	result *= dNum;
	return result;
}


inline const CMatrix<double>&  CMatrix<double>::operator/=(double dNum)					
{
	for(int i=0; i<m_Row; i++)
		for(int j=0; j<m_Col; j++)
			m_pp[i][j] = m_pp[i][j]/dNum;
	return *this;

}

//矩阵除数
inline const CMatrix<double>  CMatrix<double>::operator/(double dNum) const						
{
	CMatrix<double> result = *this;
	result /= dNum;
	return result;

}

inline const CMatrix<double>& CMatrix<double>::operator/=(CMatrix<double> m)
{
	assert(m.m_Col==1 && m.m_Row==1);
	double dNum = m.m_pp[0][0];
	return operator/=(dNum);
}

inline const CMatrix<double> CMatrix<double>::operator/(CMatrix<double> m) const
{
	assert(m.m_Col==1 && m.m_Row==1);
	double dNum = m.m_pp[0][0];
	return operator/(dNum);
}


//********************************************************************
/// 按行合并两个矩阵
///  |1 2|  and  |5 6|  to |1 2 5 6|
///  |3 4|       |7 8|     |3 4 7 8|
/// Author	Wang Hua
/// Date	2007.9.4
/// Input	mtxA
//			mtxB
/// Output	mtxAB
/// Return	
//********************************************************************
template <class _Type>
void  AsMtxMergeRow(const CMatrix<_Type>& mtxA, const CMatrix<_Type>& mtxB,
								CMatrix<_Type>& mtxAB)
{
	assert(mtxA.GetSizeRow()==mtxB.GetSizeRow());

	int i,j;
	int r=mtxA.GetSizeRow();
	mtxAB.Resize(r, mtxA.GetSizeCol()+mtxB.GetSizeCol());
	for (i=0; i<r; i++) {
		for (j=0; j<mtxA.GetSizeCol(); j++) {
			mtxAB[i][j]=mtxA[i][j];
		}
	}
	for (i=0; i<r; i++) {
		for (j=0; j<mtxB.GetSizeCol(); j++) {
			mtxAB[i][j+mtxA.GetSizeCol()]=mtxB[i][j];
		}
	}
}


//********************************************************************
///按列合并两个矩阵
///  |1 2|  and  |5 6|  
///  |3 4|       |7 8|  
///  to |1 2|
///     |3 4|
///     |5 6|
///     |7 8|
/// Author	Wang Hua
/// Date	2007.9.4
/// Input	mtxA
//			mtxB
/// Output	mtxAB
/// Return	
//********************************************************************
template <class _Type>
void  AsMtxMergeCol(const CMatrix<_Type>& mtxA, const CMatrix<_Type>& mtxB,
								CMatrix<_Type>& mtxAB)
{
	assert(mtxA.GetSizeCol()==mtxB.GetSizeCol());

	int i,j;
	int c=mtxA.GetSizeCol();
	mtxAB.Resize(mtxA.GetSizeRow()+mtxB.GetSizeRow(), c);
	for (i=0; i<mtxA.GetSizeRow(); i++) {
		for (j=0; j<c; j++) {
			mtxAB[i][j]=mtxA[i][j];
		}
	}
	for (i=0; i<mtxB.GetSizeRow(); i++) {
		for (j=0; j<c; j++) {
			mtxAB[i+mtxA.GetSizeRow()][j]=mtxB[i][j];
		}
	}
}


// Output stream function
template <class _Type>
std::ostream& operator<<(std::ostream &s, const CMatrix<_Type> &A)
{
    int M=A.GetSizeRow();
    int N=A.GetSizeCol();

    s << M << " " << N << "\n";

    for (int i=0; i<M; i++) {
        for (int j=0; j<N; j++) {
            s << A[i][j] << " ";
        }
		if (i<M-1)
	        s << "\n";
    }


    return s;
}

// input stream function
template <class _Type>
std::istream& operator>>(std::istream &s, CMatrix<_Type> &A)
{

    int M, N;

    s >> M >> N;

    if ( !(M == A.GetSizeRow() && N == A.GetSizeCol() )) {
        A.Resize(M,N);
    }


    for (int i=0; i<M; i++)
        for (int j=0; j<N; j++) {
            s >>  A[i][j];
        }


    return s;
}

