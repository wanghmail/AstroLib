////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2006                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsMatrix.cpp
//
//////////////////////////////////////////////////////////////////////


#include "AsMatrix.h"


using namespace std;



CMatrix<double>::CMatrix() : _CMatrix_base<double>()
{
	for (int i=0; i<m_Row; i++)
		memset(m_pp[i], 0, m_Col*sizeof(double));
}

CMatrix<double>::CMatrix(double d11, double d12, double d13, double d21, double d22, double d23, double d31, double d32, double d33)
			: _CMatrix_base<double>(d11, d12, d13, d21, d22, d23, d31, d32, d33)
{
}

CMatrix<double>:: CMatrix(int nRows, int nCols) : _CMatrix_base<double>(nRows,nCols)
{
	for (int i=0; i<m_Row; i++)
		memset(m_pp[i], 0, m_Col*sizeof(double));
}

CMatrix<double>::CMatrix(const CMatrix<double>& other) : _CMatrix_base<double>(other)
{
}

CMatrix<double>::CMatrix(const _CMatrix_base<double>& mat) : _CMatrix_base<double>(mat)
{
}

CMatrix<double>::~CMatrix()
{
}


// 重载运算符=，给矩阵赋值
CMatrix<double>& CMatrix<double>::operator=(const CMatrix<double>& other)
{
	assert(m_Row==other.GetSizeRow() && m_Col==other.GetSizeCol());

	if (this==&other)
		return *this;

	for (int i=0; i<m_Row; i++)
		memcpy(m_pp[i], other.m_pp[i], m_Col*sizeof(double));

	return *this ;
}


void CMatrix<double>::Resize(int nRow, int nCol)
{
	_CMatrix_base<double>::Resize(nRow, nCol);
	for (int i=0; i<m_Row; i++)
		memset(m_pp[i], 0, m_Col*sizeof(double));
}


// 矩阵的转置
const CMatrix<double> CMatrix<double>::Transpose() const
{
	// 构造目标矩阵
	CMatrix<double> Trans(m_Col, m_Row);

	// 转置各元素
	for (int i = 0 ; i < m_Row ; ++i)
	{
		for (int j = 0 ; j < m_Col ; ++j)
			Trans.m_pp[j][i] = m_pp[i][j];
	}

	return Trans;
}



// private partial pivoting method
int CMatrix<double>::Pivot (int row)
{
	int k = row;
	double amax,temp;

	amax = -1;
	for (int i=row; i < m_Row; i++)
		if ( (temp = fabs( m_pp[i][row])) > amax && temp != 0.0)
		{
			amax = temp;
			k = i;
		}
	if (m_pp[k][row] == double(0))
		return -1;
	if (k != int(row))
	{
		for (int i=0; i<m_Col; i++)
		{
			double rowptr = m_pp[k][i];
			m_pp[k][i] = m_pp[row][i];
			m_pp[row][i] = rowptr;
		}
		return k;
	}
	return 0;
}

// calculate the determinant of a matrix
// 计算行列式的值
double CMatrix<double>::Det () const
{
	assert(m_Row == m_Col);

	int i,j,k;
	double piv,detVal = 1.0;

	CMatrix<double> temp(*this);

	for (k=0; k < m_Row; k++)
	{
		int indx = temp.Pivot(k);
		if (indx == -1)
			return 0;
		if (indx != 0)
			detVal = - detVal;
		detVal = detVal * temp.m_pp[k][k];
		for (i=k+1; i < m_Row; i++)
		{
			piv = temp.m_pp[i][k] / temp.m_pp[k][k];
			for (j=k+1; j < m_Row; j++)
				temp.m_pp[i][j] -= piv * temp.m_pp[k][j];
		}
	}
	return detVal;
}

// calculate the norm of a matrix, F norm
double CMatrix<double>::NormF () const
{
	double retVal = 0.0;

	for (int i=0; i < m_Row; i++)
		for (int j=0; j < m_Col; j++)
			retVal += m_pp[i][j] * m_pp[i][j];
	retVal = sqrt( retVal);

	return retVal;
}

//得到矩阵的模，即绝对值最大的元素的绝对值,m norm
double	CMatrix<double>::NormM() const
{
	double a=0;
	for(int i=0; i<m_Row; i++)
	{
		for(int j=0; j<m_Col; j++)
		{
			if(a<fabs(m_pp[i][j]))
				a = fabs(m_pp[i][j]);
		}
	}
	return a;
}

// calculate the condition number of a matrix
/*	double Cond () const
{
   CMatrix<double> inv = ! (*this);
   return (Norm() * inv.Norm());
}
*/

// calculate the cofactor of a matrix for a given element
// 计算代数余子式
double CMatrix<double>::Cofactor (int row, int col) const
{
	assert(m_Row == m_Col && row<m_Row && col<m_Col);

	int i,i1,j,j1;

	CMatrix<double> temp (m_Row-1,m_Col-1);

	for (i=i1=0; i < m_Row; i++)
	{
		if (i == row)
			continue;
		for (j=j1=0; j < m_Col; j++)
		{
			if (j == col)
				continue;
			temp.m_pp[i1][j1] = m_pp[i][j];
			j1++;
		}
		i1++;
	}
	double  cof = temp.Det();
	if ((row+col)%2 == 1)
		cof = -cof;

	return cof;
}

// calculate adjoin of a matrix
// 计算伴随矩阵
const CMatrix<double> CMatrix<double>::Adj () const
{
	assert (m_Row == m_Col);

	CMatrix<double> temp(m_Row,m_Col);

	for (int i=0; i<m_Row; i++)
		for (int j=0; j < m_Col; j++)
			temp.m_pp[j][i] = Cofactor(i,j);
	return temp;
}

// Determine if the matrix is singular
bool CMatrix<double>::IsSingular () const
{
	if (m_Row != m_Col)
		return false;
	return (Det() == double(0));
}

// Determine if the matrix is diagonal
bool CMatrix<double>::IsDiagonal () const
{
	if (m_Row != m_Col)
		return false;
	for (int i=0; i < m_Row; i++)
		for (int j=0; j < m_Col; j++)
			if (i != j && m_pp[i][j] != double(0))
				return false;
   return true;
}

// Determine if the matrix is scalar
bool CMatrix<double>::IsScalar () const
{
	if (!IsDiagonal())
		return false;
	double v = m_pp[0][0];
	for (int i=1; i < m_Row; i++)
		if (m_pp[i][i] != v)
			return false;
	return true;
}

// Determine if the matrix is a unit matrix
bool CMatrix<double>::IsUnit () const
{
	if (IsScalar() && m_pp[0][0] == 1.0)
		return true;
	return false;
}

// Determine if this is a null matrix
bool CMatrix<double>::IsNull () const
{
	for (int i=0; i < m_Row; i++)
		for (int j=0; j < m_Col; j++)
			if (m_pp[i][j] != double(0))
				return false;
	return true;
}

// Determine if the matrix is symmetric
//return parameter:	true 对称，false不对称
bool CMatrix<double>::IsSymmetric() const
{
	if (m_Row != m_Col)
		return false;
	for (int i=0; i<m_Row; i++)
		for (int j=i+1; j<m_Col; j++)
			if (m_pp[i][j] != m_pp[j][i])
				return false;
	return true;
}

// Determine if the matrix is skew-symmetric
bool CMatrix<double>::IsSkewSymmetric () const
{
	if (m_Row != m_Col)
		return false;
	for (int i=0; i < m_Row; i++)
		for (int j=0; j < m_Col; j++)
			if (m_pp[i][j] != -m_pp[j][i])
				return false;
	return true;
}

// Determine if the matrix is upper triangular
bool CMatrix<double>::IsUpperTriangular () const
{
	if (m_Row != m_Col)
		return false;
	for (int i=1; i<m_Row; i++)
		for (int j=0; j<i-1; j++)
			if (m_pp[i][j] != double(0))
				return false;
	return true;
}

// Determine if the matrix is lower triangular
bool CMatrix<double>::IsLowerTriangular () const
{
	if (m_Row != m_Col)
		return false;

	for (int j=1; j<m_Col; j++)
		for (int i=0; i<j-1; i++)
			if (m_pp[i][j] != double(0))
				return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
// 将方阵初始化为单位矩阵
//////////////////////////////////////////////////////////////////////
void CMatrix<double>::MakeUnit()
{
	assert(m_Row == m_Col);

	for (int i=0; i<m_Row; i++)
	{
		for (int j=0; j<m_Row; j++)
			m_pp[i][j] = 0;
		m_pp[i][i] = 1.0;
	}
}



// inversion function
const CMatrix<double> CMatrix<double>::Inv () const
{
   assert (m_Row == m_Col);

   int i,j,k;
   double a1,a2,rowptr;

   CMatrix<double> temp(m_Row,m_Col), m(*this);

   temp.MakeUnit();
   for (k=0; k < m_Row; k++)
   {
		int indx = m.Pivot(k);
		if (indx == -1)
			cerr<< "Error: matrixT::operator!: Inversion of a singular matrix";

		if (indx != 0)
		{
			for (j=0; j<m_Row; j++)
			{
				rowptr = temp.m_pp[k][j];
				temp.m_pp[k][j] = temp.m_pp[indx][j];
				temp.m_pp[indx][j] = rowptr;
			}
		}
		a1 = m.m_pp[k][k];
		for (j=0; j < m_Row; j++)
		{
			m.m_pp[k][j] /= a1;
			temp.m_pp[k][j] /= a1;
		}
		for (i=0; i < m_Row; i++)
		{
			if (i != k)
			{
				a2 = m.m_pp[i][k];
				for (j=0; j < m_Row; j++)
				{
					m.m_pp[i][j] -= a2 * m.m_pp[k][j];
					temp.m_pp[i][j] -= a2 * temp.m_pp[k][j];
				}
			}
		}
	}

	return temp;
}



