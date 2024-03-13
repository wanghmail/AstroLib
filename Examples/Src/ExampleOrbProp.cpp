// ExampleOrbProp.cpp: implementation of the ExampleOrbProp class.
//
//////////////////////////////////////////////////////////////////////

#include "AstroLibExamples.h"
#include "ExampleOrbProp.h"
#include <vector>
#include <stdio.h>



//********************************************************************
/// 高精度轨道计算例子.
/// @Author Wang Hua
/// @Date   2008.8.8
//********************************************************************
void ExampleOrbProp()
{
	COrbit			orb;
	int				i, n;
	double			duration, t1, t2, step;
	CCoord			pos, vel;
	vector<double>	timeVec;
	vector<CCoord>	posVec, velVec;

	//
	//设置参数
	//
	step=20;
	duration = 86400;

    pos = CCoord(-2830087.478,        5195460.811,        3098118.351);
    vel = CCoord(-4385.611,       -4846.083,        4125.976);

	//
	//轨道计算
	//
	t2 = duration;
	for (t1 = 0; t1<t2; t1+=step)
	{
		//定步长方法
		orb.OrbitStep(step, pos, vel);

		timeVec.push_back(t1+step);
		posVec.push_back(pos);
		velVec.push_back(vel);
	}
	orb.OrbitStep(t2-t1, pos, vel);
	timeVec.push_back(t2);
	posVec.push_back(pos);
	velVec.push_back(vel);

	n = posVec.size();
    printf("Positions result: %f, %f, %f\n", 
        posVec[n-1][0], posVec[n-1][1], posVec[n-1][2]);
    printf("Velocities result: %f, %f, %f\n", 
        velVec[n-1][0], velVec[n-1][1], velVec[n-1][2]);

	//
	//输出结果,插值后结果
	//
	FILE* fp;
	if ((fp=fopen("飞行器星历.txt", "w")) == NULL)
		cerr<<"Can't open!";

	fprintf(fp, "Time Elapsed (s)            Time (UTCG)              x (m)     \
         y (m)              z (m)          vx (m/sec)      vy (m/sec) \
     vz (m/sec)\n");
	fprintf(fp, "----------------    ------------------------    ---------------\
    ---------------    ---------------    ------------    ------------\
    ------------\n");
	for (i=0; i<posVec.size(); i++)
	{
		fprintf(fp, "%16.3f    ", timeVec[i]);
		fprintf(fp, "%15.3f    %15.3f    %15.3f    ", 
			posVec[i][0], 
			posVec[i][1], 
			posVec[i][2]);
		fprintf(fp, "%12.3f    %12.3f    %12.3f    ", 
			velVec[i][0], 
			velVec[i][1], 
			velVec[i][2]);
		fprintf(fp, "\n");
	}
	fclose(fp);

}