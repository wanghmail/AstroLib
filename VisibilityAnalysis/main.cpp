#include "Parameter.h"
#include "ToolFunctions.h"

using namespace std;

int main()
{
	/*********************************************************************/
	/*********************************************************************/
	/* 
	% input

		% theta = 地面站仰角 约束

		% Time0 = [years, month, day, hour, minute, sec];场景开始时间 UTCG

		% oev = [a, e, inc, ome, Omega, f]; 卫星轨道六根数

		% LLH = [Lon, lat, HH]; 地面站经纬高 

		% dT   仿真步长

		% tsim 仿真总时长

	% output

		% 观测情况
	*/
	/*********************************************************************/
	/*********************************************************************/

	double Time0[6] = { 0.0 };
	double Time_scenario[6] = { 0.0 };
	double years, month, day, hour, minute, sec;

	double oev_origin[6] = { 0.0 };
	double a, e, inc, ome, Omega, f;

	double theta;

	double Lon, lat, HH;

	double dT;
	double tsim;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////           输入          /////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 约束 仰角 °
	theta = 10.0 * pi / 180.0; // rad, pi前数字单位为°

	// 场景开始时间 UTCG (年/月/日/时/分/秒)
	years = 2024;
	month = 5;
	day = 17;
	hour = 4;
	minute = 0;
	sec = 0;

	// 卫星 -- 轨道六根数
	a = 7100.0;					// km
	e = 1e-4;					// 无单位
	inc = 53.0 * pi / 180.0;	// rad
	ome = 100.0 * pi / 180.0;	// rad
	Omega = 327.0 * pi / 180.0; // rad
	f = 50.0 * pi / 180.0;		// rad

	// 地面站 -- 经纬高
	Lon = 120.0 * pi / 180.0; // rad
	lat = 25.0 * pi / 180.0;  // rad
	HH = 1.0;				  // km


	dT = 1;				// 仿真步长/sec 	
	tsim = 1 * 86400;	// 仿真总时长/sec   

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	oev_origin[0] = a;		oev_origin[1] = e;		oev_origin[2] = inc;
	oev_origin[3] = ome;	oev_origin[4] = Omega;	oev_origin[5] = f;

	Time0[0] = years;	Time0[1] = month;	Time0[2] = day;
	Time0[3] = hour;	Time0[4] = minute;	Time0[5] = sec;

	// 卫星 ECI系下位速
	double* ECI;
	ECI = orb2eci(oev_origin);

	// 地面站 ECF系下位速
	double* ground_ECF;
	ground_ECF = LLH2ECF(lat, Lon, HH);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 设置仿真持续时间

	int m_dT = round(tsim / dT + 1); // 步数

	double r_eci[3] = { ECI[0], ECI[1], ECI[2] };
	double v_eci[3] = { ECI[3], ECI[4], ECI[5] };

	// 定义储存数据用的二维动态数组
	vector<vector<double>> r_out(3), v_out(3);
	//设置每一行的元素个数
	for (int i = 0; i < 3; i++)
	{
		r_out[i].resize(m_dT + 1);
		v_out[i].resize(m_dT + 1);
	}

	for (int i = 0; i < 3; i++)
	{
		r_out[i][0] = r_eci[i];
		v_out[i][0] = v_eci[i];
	}

	///////// 计算卫星轨道数据 /////////
	double t = 0;
	int ii = 0;
	int jj = 0;

	for (int i = 0; i < m_dT; i++)
	{
		t = t + dT;
		ii = ii++;

		// 储存临时数据
		double PV[6] = { 0 };
		for (int i = 0; i < 3; i++)
		{
			PV[i] = r_eci[i];
			PV[i + 3] = v_eci[i];
		}

		double statenew[6] = { 0 };
		double statenew_d[6] = { 0 };
		double F[3] = { 0 };

		// 递推航天器位置速度
		orb_rkf78(PV, 1, t - dT, t, F, statenew, 6);
		r_eci[0] = statenew[0]; r_eci[1] = statenew[1]; r_eci[2] = statenew[2];
		v_eci[0] = statenew[3]; v_eci[1] = statenew[4]; v_eci[2] = statenew[5];

		for (int axis = 0; axis < 3; axis++)
		{
			r_out[axis][ii] = r_eci[axis];
			v_out[axis][ii] = v_eci[axis];
		}

	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 计算 地面站对卫星 观测情况
	vector<vector<double>> Window_all;		 // 窗口时间
	vector<vector<double>> distance_all;	 // 距离

	int col = r_out[0].size() - 1;	// cout << col << endl;
	double Time_new[6] = { 0.0 };

	// 赋于 初始时间
	for (int i = 0; i < 5; i++) Time_new[i] = Time0[i];

	for (int i = 0; i < col; i++)
	{
		// 当前时间
		Time_new[5] = Time0[5] + i * dT;

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
		r_sat[0] = r_out[0][i];
		r_sat[1] = r_out[1][i];
		r_sat[2] = r_out[2][i];

		// 角度筛选
		double r1[3] = { 0.0 };
		double r2[3] = { 0.0 };

		r1[0] = ground_ECI[0];	r1[1] = ground_ECI[1];	r1[2] = ground_ECI[2];

		// 地面站指向卫星的矢量
		r2[0] = r_sat[0] - ground_ECI[0];
		r2[1] = r_sat[1] - ground_ECI[1];
		r2[2] = r_sat[2] - ground_ECI[2];

		double cos_alpha = linemulti(r1, r2) / norm(r1) / norm(r2);  // norm(r1)/norm(r2)
		double alpha = acos(cos_alpha);
		double distance_new = norm(r2);

		if (alpha <= 90.0 * pi / 180.0 - theta)
		{
			vector<double> time = { i * dT };
			Window_all.insert(Window_all.end(), time);

			vector<double> dis = { distance_new };
			distance_all.insert(distance_all.end(), dis);
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	int row = Window_all.size();
	vector<int> obs;

	if (row == 0)
	{
		printf("卫星未能侦测到指定地点\n");//判断是否能侦测到
	}
	else
	{

		// 前后时间 不一致 ，故分为两段
		for (int i = 0; i < row - 1; i++)
		{
			if (Window_all[i + 1][0] - Window_all[i][0] != dT)
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
		vector<vector<string>> All_Timeperiod(2);
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
		cout << "显示结果，将结果储存于 -- 观测情况.txt" << endl;
		cout << endl;
		cout << "		  观测开始时间(UTCG)	    观测结束时间(UTCG)	  观测持续时间(sec)" << endl;
		for (int i = 0; i < pic; i++)
		{
			cout << "第" << to_string(i + 1) << "次观测	" << All_Timeperiod[0][i] << "	  " << All_Timeperiod[1][i] << "	" << Duration[i] << endl;
		}
		cout << endl;


		ofstream fwD;
		fwD.open("../观测情况.txt");
		fwD << "		  观测开始时间(UTCG)	    观测结束时间(UTCG)	  观测持续时间(sec)" << endl;
		for (int i = 0; i < pic; i++)
		{
			fwD << "第" << to_string(i + 1) << "次观测	" << All_Timeperiod[0][i] << "	  "
				<< All_Timeperiod[1][i] << "	" << Duration[i] << endl;
		}
		fwD.close();
	}

}