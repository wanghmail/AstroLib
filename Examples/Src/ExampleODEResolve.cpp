// ExampleODEResolve.cpp: implementation of the CExampleODEResolve class.
//
//////////////////////////////////////////////////////////////////////

#include "AstroLibExamples.h"
#include "ExampleODEResolve.h"



//********************************************************************
/// ��΢�ַ������ֵ�����������.
/// @Author	Wang Hua
/// @Date	2009.4.9
//********************************************************************
void ExampleODEResolve()
{
	//����������΢�ַ�������Һ�����
	//����ĳ�΢�ַ���Ϊ���������������亯��
	// dx/dt=v
	// dv/dt=a
	// a=9.8
	class CMyRightFunc : public CRightFunc 
	{
	public:
		/// @Input
		/// @Param	t		�Ա�����ֵ
		/// @Param	x		��ʼ����ֵ,x[0]Ϊλ��,x[1]Ϊ�ٶ�
		/// @Output
		/// @Param	result	����õ��ĺ���ֵ
		virtual	void operator() (double t, const CVector<double>& x, CVector<double>& result) const
		{
			result[0] = x[1];
			result[1] = -9.8;
		}		
	};
	
	CMyRightFunc func;
	CVector<double> x(2);

	x[0]=1000;	//��ʼλ��1000m
	x[1]=0;		//��ʼ�ٶ�0m/s

	//��ⳣ΢�ַ��̣����ֲ���ȡ1s��������ʱ��Ϊ10s
	for (int i=0; i<10; i++)
		AsODERungeKutta4 (func, 1, 0, x);

    printf("The final position is %f (m)\n", x[0]);
}

