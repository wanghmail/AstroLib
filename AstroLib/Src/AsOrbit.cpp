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
/// 计算运动方程的右函数，用于R-K积分，重载父类的纯虚函数
/// @author	Wang Hua
/// @Date	2004.5
/// @Input
/// @Param	t			开始本步轨道积分的时间,初始必须为0(s)
///			x			初始函数值
///						[0]~[2]位置(m)
///						[3]~[5]速度(m/s)
/// @Output
/// @Param	result		计算得到的函数值
/// @Return	
//********************************************************************
void COrbit::CFuncOrbit::operator()(double t, const CVector<double> &x,
            CVector<double> &result) const
{
    CCoord pos, vel;
    CCoord pertAccel;
    memcpy((double*)pos, (const double*)x, 3*sizeof(double));
    memcpy((double*)vel, (const double*)x+3, 3*sizeof(double));

    //地球引力
    AsBasicGravAccel(AsCEarthGrav, pos, pertAccel);

    memcpy((double*)result, (const double*)x+3, 3*sizeof(double));
    memcpy((double*)result+3, (const double*)pertAccel, 3*sizeof(double));
}


//********************************************************************
/// 没有外力或冲量作用时计算轨道，积分一步
/// @author	Wang Hua
/// @Date	2011-1-14
/// @Input
/// @Param	step			仿真步长 (sec), 不能太大，一般不能超过20s
/// @In/Out
/// @Param	pos				飞行器位置(m)
/// @Param	vel				飞行器速度(m/s)
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
/// 计算飞行器所受的地心引力引起的加速度(不考虑摄动)
/// @Author	Wang Hua
/// @Date	2004.3
/// @Input	gm
//			pos		位置矢量	meter
/// @Output	accel	加速度		meter/s^2
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
//			pos			position vector，地固系或惯性系,	(meter)
/// @Output	pertAccel	perturbing acceleration,地固系或惯性系，与输入位置坐标系相同(m/s^2)
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
/// 给定初始和最终的状态，根据CW方程计算所需施加的冲量
///   参考坐标系为VVLH坐标系
/// @Author	Wang Hua
/// @Date	2003.8 2004.9.6
/// @Input
/// @Param	iniPos		初始相对位置[m]
/// @Param	iniVel		初始相对速度[m/s]
/// @Param	endPos		终端相对位置[m]
/// @Param	endVel		终端相对速度[m/s]
/// @Param	t			两个冲量之间的飞行时间[s]
/// @Param	orbAngVel	参考飞行器轨道角速度[rad/s]
/// @Output
/// @Param	impulse1	第一个冲量值[m/s]
/// @Param	impulse2	第二个冲量值[m/s]
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

    //状态转移矩阵赋初值
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

    //计算两次冲量的大小
    impulse1 = (m2.Inv())*(endPos-m1*iniPos)-iniVel;
    impulse2 = endVel-(m3*iniPos+m4*(iniVel+impulse1));
}


void	AsCWImpulse(
            const double iniPos[3],
            const double iniVel[3],
            const double endPos[3],
            const double endVel[3],
            double t,
            double orbAngVel,
            double impulse1[3],
            double impulse2[3])
{
    CCoord v1, v2;
    AsCWImpulse(
        CCoord(iniPos),
        CCoord(iniVel),
        CCoord(endPos),
        CCoord(endVel),
        t,
        orbAngVel,
        v1, v2);
    memcpy(impulse1, v1.Data(), 3*sizeof(double));
    memcpy(impulse2, v2.Data(), 3*sizeof(double));
}
