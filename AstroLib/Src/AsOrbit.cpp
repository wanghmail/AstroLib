////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// Orbit.cpp: implementation of the COrbit class.
//
//////////////////////////////////////////////////////////////////////


#include "AsOrbit.h"
#include "AsMath.h"
#include "AsDResolve.h"


using namespace std;



//////////////////////////////////////////////////////////////////////
// COrbit::CFuncOrbit

COrbit::CFuncOrbit::CFuncOrbit()
{
}


COrbit::CFuncOrbit::~CFuncOrbit()
{
}


//********************************************************************
/// �����˶����̵��Һ���������R-K���֣����ظ���Ĵ��麯��
/// @author	Wang Hua
/// @Date	2004.5
/// @Input
/// @Param	t			��ʼ����������ֵ�ʱ��,��ʼ����Ϊ0(s)
///			x			��ʼ����ֵ
///						[0]~[2]λ��(m)
///						[3]~[5]�ٶ�(m/s)
/// @Output
/// @Param	result		����õ��ĺ���ֵ
/// @Return	
//********************************************************************
void COrbit::CFuncOrbit::operator()(double t, const CVector<double> &x,
            CVector<double> &result) const
{
    CCoord pos, vel;
    CCoord pertAccel;
    memcpy((double*)pos, (const double*)x, 3*sizeof(double));
    memcpy((double*)vel, (const double*)x+3, 3*sizeof(double));

    //��������
    AsBasicGravAccel(AsCEarthGrav, pos, pertAccel);

    memcpy((double*)result, (const double*)x+3, 3*sizeof(double));
    memcpy((double*)result+3, (const double*)pertAccel, 3*sizeof(double));
}


//********************************************************************
/// û���������������ʱ������������һ��
/// @author	Wang Hua
/// @Date	2011-1-14
/// @Input
/// @Param	step			���沽�� (sec), ����̫��һ�㲻�ܳ���20s
/// @In/Out
/// @Param	pos				������λ��(m)
/// @Param	vel				�������ٶ�(m/s)
/// @Output
/// @Return	
//********************************************************************
void COrbit::OrbitStep(
            double step,
            CCoord& pos,
            CCoord& vel)
{
    if (step==0)
        return;

    CVector<double> x(6);

    memcpy((double*)x, (const double*)pos, 3*sizeof(double));
    memcpy((double*)x+3, (const double*)vel, 3*sizeof(double));

    AsODERungeKutta4(m_FuncOrbit, step, 0, x);

    memcpy((double*)pos, (const double*)x, 3*sizeof(double));
    memcpy((double*)vel, (const double*)x+3, 3*sizeof(double));
}


//********************************************************************
/// ������������ܵĵ�����������ļ��ٶ�(�������㶯)
/// @Author	Wang Hua
/// @Date	2004.3
/// @Input	gm
//			pos		λ��ʸ��	meter
/// @Output	accel	���ٶ�		meter/s^2
/// @Return		
//********************************************************************
void AsBasicGravAccel(double gm, const CCoord& pos, CCoord& accel)
{
    double r2 = pos.SqrNorm();
    double r3 = r2*sqrt(r2);
	accel = (-gm/r3)*pos;
}


//********************************************************************
//calculates the actual value of the perturbing acceleration of J2.
/// @Author	Wang Hua
/// @Date	2004.11.15
/// @Input	gm
//			J2			J2 perturb
//			refDist		reference distance (earth radius)	(meter)
//			pos			position vector���ع�ϵ�����ϵ,	(meter)
/// @Output	pertAccel	perturbing acceleration,�ع�ϵ�����ϵ��������λ������ϵ��ͬ(m/s^2)
/// @Return	
//********************************************************************
void AsJ2Accel (double gm, double J2, double refDist, const CCoord& pos, 
				CCoord& pertAccel)
{
	double r   = pos.Norm();
	double r2  = AsSqr(pos[2] / r);

	double Temp1 = -refDist * refDist * gm * 1.5 * J2 / pow(r, 5);
	double Temp2 = Temp1 * (1.0 - 5.0 * r2);
	pertAccel[0] = Temp2 * pos[0];
	pertAccel[1] = Temp2 * pos[1];
	pertAccel[2] = Temp1 * pos[2] * (3.0 - 5.0 * r2);

}


//*********************************************************************
/// ������ʼ�����յ�״̬������CW���̼�������ʩ�ӵĳ���
///   �ο�����ϵΪVVLH����ϵ
/// @Author	Wang Hua
/// @Date	2003.8 2004.9.6
/// @Input
/// @Param	iniPos		��ʼ���λ��[m]
/// @Param	iniVel		��ʼ����ٶ�[m/s]
/// @Param	endPos		�ն����λ��[m]
/// @Param	endVel		�ն�����ٶ�[m/s]
/// @Param	t			��������֮��ķ���ʱ��[s]
/// @Param	orbAngVel	�ο�������������ٶ�[rad/s]
/// @Output
/// @Param	impulse1	��һ������ֵ[m/s]
/// @Param	impulse2	�ڶ�������ֵ[m/s]
/// @Return	
//*********************************************************************
void    AsCWImpulse(
            const CCoord& iniPos,
            const CCoord& iniVel,
            const CCoord& endPos,
            const CCoord& endVel,
            double t,
            double orbAngVel,
            CCoord& impulse1,
            CCoord& impulse2)
{
    CMatrix<double> m1(3, 3), m2(3, 3), m3(3, 3), m4(3, 3);

    double aV1 = 1.0/orbAngVel;
    double tau = t*orbAngVel;
    double c = cos(tau);
    double s = sin(tau);

    //״̬ת�ƾ��󸳳�ֵ
    m1[0][0]=1.0;				m1[0][1]=0.0;			m1[0][2]=6.0*(tau-s);
    m1[1][0]=0.0;				m1[1][1]=c;				m1[1][2]=0.0;
    m1[2][0]=0.0;				m1[2][1]=0.0;			m1[2][2]=4.0-3.0*c;

    m2[0][0]=(4*s-3*tau)*aV1;	m2[0][1]=0;				m2[0][2]=2*(1-c)*aV1;
    m2[1][0]=0;					m2[1][1]=s*aV1;			m2[1][2]=0;
    m2[2][0]=-m2[0][2];			m2[2][1]=0;				m2[2][2]=m2[1][1];

    m3[0][0]=0;					m3[0][1]=0;				m3[0][2]=6*orbAngVel*(1-c);
    m3[1][0]=0;					m3[1][1]=-orbAngVel*s;	m3[1][2]=0;
    m3[2][0]=0;					m3[2][1]=0;				m3[2][2]=3*orbAngVel*s;

    m4[0][0]=4*c-3;				m4[0][1]=0;				m4[0][2]=s+s;
    m4[1][0]=0;					m4[1][1]=c;				m4[1][2]=0;
    m4[2][0]=-2*s;				m4[2][1]=0;				m4[2][2]=c;

    //�������γ����Ĵ�С
    impulse1 = (m2.Inv())*(endPos-m1*iniPos)-iniVel;
    impulse2 = endVel-(m3*iniPos+m4*(iniVel+impulse1));
}
