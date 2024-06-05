// satelliteobservation.cpp : 定义控制台应用程序的入口点。
//

#include "satelliteobservation.h"
#include "satelliteParameter.h"
#include "AstroLib.h"
#include <fstream>
// 常量声明
#define pi 3.141592653589793

using namespace std;


//***********************************************************************
/// 仿真主程序
/// @Author	Wang Hua
/// @Date	2010-4-15
//***********************************************************************
void Csatobs::Main()
{
    //初始化
    Initialization();
    //循环到仿真结束
    TimeAdvance(step);
    //结果报告生成
    ReportGeneration();
}



//***********************************************************************
/// 初始化
/// @Author	Wang Hua
/// @Date	2010-4-15
//***********************************************************************
void Csatobs::Initialization()
{
	double Time0 = { 0.0 };
	double Vel = { 0.0 }; //卫星地心惯性系速度
	double Pos = { 0.0 }; //卫星地心惯性式位置
	double theta=0;       //地面可观测卫星的最小仰角
	double lla = { 0.0 }; //地面站经纬高数组
	double m_Time=0;

	//清空缓存
	m_TimeList.resize(0);
	m_PosList.resize(0);
	m_VelList.resize(0);
	
}


//***********************************************************************
/// 时间推进
/// @Author	Wang Hua
/// @Date	2010-4-15
/// @Return	true=仿真结束
//***********************************************************************
bool Csatobs::TimeAdvance(double step)
{
	COrbit			orb;
	bool    isEnd = false;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////           输入的参数          ///////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 认为仰角10 °以上才能捕捉
	theta = 10.0 * pi / 180.0; // rad, pi前数字单位为°
	double Re = 6378.136;//地球半径
	double flat = 0.00335281317789691; //地球扁率
	
	// 地面站 -- 经纬高
	double Lon = 120.0 * pi / 180.0; // rad
	double lat = 25.0 * pi / 180.0;  // rad
	double Alt = 1.0;				  // km
	CCoord lla(Lon,lat,Alt);
	double radius = 6378.136;
	double flatFact = 0.00335281317789691;
	step = 1;				// 仿真步长/sec 	
	m_Time = 1 * 86400;	   // 仿真总时长（24小时）/sec   

	double Time0[6] = { 2024,5,29,12,0,0 }; // 场景开始时间 UTCG (年/月/日/时/分/秒)

	// 地面站ECF系下位置
	CCoord ground_ECF;
	void	AsOblLLAToCart(const CLatLonAlt& lla,double radius, double flatFact,CCoord& ground_ECF);

	// 设置仿真持续时间
	int m_step = round(m_Time / step + 1); // 步数

	m_Pos = { Pos[0], Pos[1], Pos[2] }; //卫星 ECI系下初始位置
	m_Vel = { Vel[0], Vel[1], Vel[2] }; //卫星 ECI系下初始速度

	//卫星轨道计算
	for (int i = 0; i < m_step; i++)
	{
		//定步长方法
		orb.OrbitStep(step, m_Pos, m_Vel);

		m_TimeList.push_back(i * step + step);
		m_PosList.push_back(m_Pos);
		m_VelList.push_back(m_Vel);
	}
	
	// 计算 地面站对卫星 观测情况
	vector<vector<double>> Window_all;		 // 窗口时间
	double Time_new[6] = { 0.0 };

	// 赋 初始时间
	for (int i = 0; i < 5; i++) Time_new[i] = Time0[i];

	for (int i = 0; i <  m_step; i++)
	{
		// 当前时间
		Time_new[5] = Time0[5] + i * step;
		//CCTime t(Time_new);
		
		//AsTimeToJD(t, double& JD);
		//AsICRFToECFMtx(double jd, CMatrix<double>& mtx);//ECI转ECF矩阵

		// ECI到ECF的旋转矩阵
		double** C_RNP_ = C_RNP_Fun(Time_new);
		double C_RNP[3][3] = { 0.0 };
		for (int m = 0; m < 3; m++)
		{
			for (int n = 0; n < 3; n++) C_RNP[m][n] = C_RNP_[m][n];
		}

		// ECF到ECI的旋转矩阵
		double** C_RNPT_ = mattrans(C_RNP);
		double C_RNPT[3][3] = { 0.0 };
		for (int m = 0; m < 3; m++)
		{
			for (int n = 0; n < 3; n++) C_RNPT[m][n] = C_RNPT_[m][n];
		}

		// 地面站ECI的坐标
		double* ground_ECI = matm31(C_RNPT, ground_ECF);
				
		// 当前卫星 位置
		double r_sat[3] = { 0.0 };
		r_sat[0] = m_PosList[i][0];
		r_sat[1] = m_PosList[i][1];
		r_sat[2] = m_PosList[i][2];

		// 角度筛选
		double r1[3] = { 0.0 };
		double r2[3] = { 0.0 };

		r1[0] = ground_ECI[0];	r1[1] = ground_ECI[1];	r1[2] = ground_ECI[2];

		// 地面站指向卫星的矢量
		r2[0] = r_sat[0] - ground_ECI[0];
		r2[1] = r_sat[1] - ground_ECI[1];
		r2[2] = r_sat[2] - ground_ECI[2];

		double cos_alpha = linemulti(r1, r2) / norm(r1) / norm(r2);  
		double alpha = acos(cos_alpha);   //地心指向地面站矢量与地面站指向卫星矢量的夹角
		
		if (alpha <= 90.0 * pi / 180.0 - theta)
		{
			vector<double> time = { i * step };
			Window_all.insert(Window_all.end(), time);
		}
	}

    return isEnd;
}


//***********************************************************************
/// 结果报告生成
/// @Author	Wang Hua
/// @Date	2010-4-15
//***********************************************************************
void Csatobs::ReportGeneration()
{
	int row = Window_all.size();
	vector<int> obs;

	// 前后时间 不一致 ，故分为两段
	for (int i = 0; i < row - 1; i++)
	{
		if (Window_all[i + 1][0] - Window_all[i][0] != step)
		{
			int obsp = i;
			obs.insert(obs.end(), obsp);
		}
	}

	// 观测段数
	int pic = obs.size() + 1;

	// 具体观测时间分布
	vector<vector<double>> Timeperiod(2);
	Timeperiod[0].resize(pic);
	Timeperiod[1].resize(pic);

	Timeperiod[0][0] = Window_all[0][0];
	Timeperiod[1][0] = Window_all[obs[0]][0];

	for (int i = 1; i < pic - 1; i++)
	{
		Timeperiod[0][i] = Window_all[obs[i - 1] + 1][0];
		Timeperiod[1][i] = Window_all[obs[i]][0];
	}

	Timeperiod[0][pic - 1] = Window_all[obs[pic - 2] + 1][0];
	Timeperiod[1][pic - 1] = Window_all[row - 1][0];


	/////////////////////////////////////////////////////////////////////////////////////////////////

	// 输出 UTCG 观测时间
	vector<vector<string>> All_Timeperiod;
	All_Timeperiod[0].resize(pic);
	All_Timeperiod[1].resize(pic);

	vector<double> Duration(pic);

	for (int i = 0; i < pic; i++)
	{
		double* UTCG0 = AddSec(Time0, Timeperiod[0][i]);
		string UTCG0_str = UTCGTime(UTCG0);

		double* UTCG1 = AddSec(Time0, Timeperiod[1][i]);
		string UTCG1_str = UTCGTime(UTCG1);

		All_Timeperiod[0][i] = UTCG0_str;
		All_Timeperiod[1][i] = UTCG1_str;

		Duration[i] = Timeperiod[1][i] - Timeperiod[0][i];
	}



	cout << endl;
	cout << "显示结果，将结果储存于 -- C++_Visibility_analysis/观测情况.txt" << endl;
	cout << endl;
	cout << "		  观测开始时间(UTCG)	    观测结束时间(UTCG)	  观测持续时间(sec)" << endl;
	for (int i = 0; i < pic; i++)
	{
		cout << "第" << to_string(i + 1) << "次观测	" << All_Timeperiod[0][i] << "	  " << All_Timeperiod[1][i] << "	" << Duration[i] << endl;
	}
	cout << endl;


	ofstream fwD;
	fwD.open("../../观测情况.txt");
	fwD << "		  观测开始时间(UTCG)	    观测结束时间(UTCG)	  观测持续时间(sec)" << endl;
	for (int i = 0; i < pic; i++)
	{
		fwD << "第" << to_string(i + 1) << "次观测	" << All_Timeperiod[0][i] << "	  "
			<< All_Timeperiod[1][i] << "	" << Duration[i] << endl;
	}
	fwD.close();


}


int main()
{
    printf("*****************************************************************\n");
    printf("*                                                               *\n");
	printf("*        <<Aerospace System Modeling and Simulation>>           *\n");
	printf("*                   satelliteobservation Simulation             *\n");
    printf("*                                                               *\n");
    printf("*****************************************************************\n\n");

    //执行交会对接仿真
    Csatobs observation;
	observation.Main();           //标称仿真
    
    return 0;
}

