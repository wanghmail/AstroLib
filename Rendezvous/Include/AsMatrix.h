////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsMatrix.h
//
//////////////////////////////////////////////////////////////////////


#if !defined(_ASMATRIX_H_)
#define _ASMATRIX_H_


#include <iostream>
#include "AsCommon.h"
#include "AsVector.h"





//////////////////////////////////////////////////////////////////////
//TEMPLATE CLASS _CMatrix_base<_Type>
template <class _Type>
class _CMatrix_base
{

	typedef _CMatrix_base<_Type> _MyT;

public:
	virtual ~_CMatrix_base();

	inline int	GetSizeCol() const;
	inline int	GetSizeRow() const;

	inline const _Type*		operator [](int n) const;
	inline		 _Type*		operator [](int n);
	inline const _Type&		operator ()(int nRow,int nCol) const;
	inline		 _Type&		operator ()(int nRow,int nCol);

	inline bool				operator==(const _MyT& other) const;
	inline bool				operator!=(const _MyT& other) const;

	inline   operator _Type**();
	inline   operator const _Type* const *() const;

	inline _Type**				Data();
	inline const _Type*const*	Data() const ;

	void	Resize(int nRow, int nCol);

	void	SetData(int n, const _Type *value);
	void	GetData(int n, _Type *value) const;
	void	SetData(int m, int n, const _Type* const *value);
	void	GetData(int m, int n, _Type **value) const;
	void	SetDataAll(const _Type& value);

	void	GetRowVector(int nRow, _Type* pVector) const;
	void	GetColVector(int nCol, _Type* pVector) const;
	void	GetRowVector(int nRow, CVector<_Type> &vec) const;
	void	GetColVector(int nCol, CVector<_Type> &vec) const;
	void	GetSubMatrix(int begRow, int begCol, int sizeRow, int sizeCol, _Type** pp) const;

protected:
	_CMatrix_base();
	_CMatrix_base(const _Type& d11, const _Type& d12, const _Type& d13, 
				  const _Type& d21, const _Type& d22, const _Type& d23, 
				  const _Type& d31, const _Type& d32, const _Type& d33);
	explicit _CMatrix_base(int nRows, int nCols);
	_CMatrix_base(const _MyT& other);

private:
	void	Construct(int nRow, int nCol);
	void	Destroy();


private:
	_Type	m_Data[9];
	_Type  *m_pData[9];

protected:
	int		m_Row, m_Col;
	_Type **m_pp;

};




//////////////////////////////////////////////////////////////////////
//TEMPLATE CLASS CMatrix<_Type>
template <class _Type>
class CMatrix : public _CMatrix_base<_Type>
{
	typedef CMatrix<_Type> _MyT;

public:

	CMatrix();
	CMatrix(const _Type& d11, const _Type& d12, const _Type& d13, 
			const _Type& d21, const _Type& d22, const _Type& d23, 
			const _Type& d31, const _Type& d32, const _Type& d33);
	explicit CMatrix(int nRows, int nCols);
	CMatrix(const _MyT& other);
	CMatrix(const _CMatrix_base<_Type>& mat);
	virtual ~CMatrix();

	const	_MyT	Transpose() const;

	_MyT&				operator =(const _MyT& other);

	inline const _MyT&			operator+=(const _MyT& other);
	inline const _MyT			operator +(const _MyT& other) const;
	inline const _MyT&			operator-=(const _MyT& other);
	inline const _MyT			operator -(const _MyT& other) const;
	inline const _MyT			operator -() const;
	inline const _MyT			operator *(const _MyT& other) const;
	inline const CVector<_Type>	operator *(const CVector<_Type>& vec) const;
	inline const _MyT&			operator*=(const _Type& other);
	inline const _MyT			operator *(const _Type& dNum) const;
	inline const _MyT&			operator/=(const _Type& dNum);
	inline const _MyT			operator /(const _Type& dNum) const;
	inline const _MyT&			operator/=(const _MyT& m);
	inline const _MyT			operator /(const _MyT& m) const;
};


//////////////////////////////////////////////////////////////////////
//TEMPLATE CLASS CMatrix<double>
template <>
class CMatrix<double> : public _CMatrix_base<double>
{
	typedef double _Type;
	typedef CMatrix<_Type> _MyT;

public:

	CMatrix();
	CMatrix(_Type d11, _Type d12, _Type d13, 
			_Type d21, _Type d22, _Type d23, 
			_Type d31, _Type d32, _Type d33);
	explicit CMatrix(int nRows, int nCols);
	CMatrix(const _MyT& other);
	CMatrix(const _CMatrix_base<double>& mat);
	virtual ~CMatrix();


	void			Resize(int nRow, int nCol);
	const	_MyT	Transpose() const;

	_MyT&				operator= (const _MyT& other);				//用矩阵赋值

	inline const _MyT&			operator+=(const _MyT& other);				//矩阵加法
	inline const _MyT			operator+ (const _MyT& other) const;		//矩阵加法
	inline const _MyT&			operator-=(const _MyT& other);				//矩阵减法
	inline const _MyT			operator- (const _MyT& other) const;		//矩阵减法
	inline const _MyT			operator- () const;							//矩阵求负
	inline const _MyT&			operator*=(const _MyT& other);				//矩阵乘矩阵
	inline const _MyT			operator* (const _MyT& other) const;		//矩阵乘矩阵
	inline const CVector<_Type>	operator* (const CVector<_Type>& v) const;	//矩阵乘矢量
	inline const _MyT&			operator*=(_Type other);
	inline const _MyT			operator* (_Type dNum) const;				//矩阵乘数
	inline const _MyT&			operator/=(_Type dNum);				
	inline const _MyT			operator/ (_Type dNum) const;				//矩阵除数
	inline const _MyT&			operator/=(_MyT m);
	inline const _MyT			operator/ (_MyT m) const;					//矩阵除数

	
	void		MakeUnit();									// 将方阵初始化为单位矩阵
	_Type		Det () const;								// calculate the determinant of a matrix
	_Type		NormM () const;								// 得到矩阵的模，即绝对值最大的元素的绝对值,m normM norm
	_Type		NormF () const;								// calculate the norm of a matrix, F norm
	_Type		Cofactor (int row, int col) const;			// calculate the cofactor of a matrix for a given element
	const _MyT	Adj () const;								// calculate adjoin of a matrix
	bool		IsSingular () const;						// Determine if the matrix is singular
	bool		IsDiagonal () const;						// Determine if the matrix is diagonal
	bool		IsScalar () const;							// Determine if the matrix is scalar	  
	bool		IsUnit () const;							// Determine if the matrix is a unit matrix
	bool		IsNull () const;							// Determine if this is a null matrix
	bool		IsSymmetric() const;						// Determine if the matrix is symmetric
	bool		IsSkewSymmetric () const;					// Determine if the matrix is skew-symmetric
	bool		IsUpperTriangular () const;					// Determine if the matrix is upper triangular
	bool		IsLowerTriangular () const;					// Determine if the matrix is lower triangular


	//
	// 算法
	//

	const _MyT	Inv() const;											//矩阵求逆
	int			RankGauss() const;										//求矩阵秩的全选主元高斯消去法


	//
	// 内部函数
	//
private:
	int			Pivot (int row);		// private partial pivoting method


};



template <class _Type>
inline const CMatrix<_Type> operator*(const _Type& dNum, const CMatrix<_Type>& m);	


#include "AsMatrix_Impl.h"





#endif // !defined(_ASMATRIX_H_)
