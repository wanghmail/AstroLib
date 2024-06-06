#include "Parameter.h"
#include "ToolFunctions.h"

using namespace std;

int main()
{
	/*********************************************************************/
	/*********************************************************************/
	/* 
	% input

		% theta = ����վ���� Լ��

		% Time0 = [years, month, day, hour, minute, sec];������ʼʱ�� UTCG

		% oev = [a, e, inc, ome, Omega, f]; ���ǹ��������

		% LLH = [Lon, lat, HH]; ����վ��γ�� 

		% dT   ���沽��

		% tsim ������ʱ��

	% output

		% �۲����
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
	//////////////////////////////////////////////////           ����          /////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Լ�� ���� ��
	theta = 10.0 * pi / 180.0; // rad, piǰ���ֵ�λΪ��

	// ������ʼʱ�� UTCG (��/��/��/ʱ/��/��)
	years = 2024;
	month = 5;
	day = 17;
	hour = 4;
	minute = 0;
	sec = 0;

	// ���� -- ���������
	a = 7100.0;					// km
	e = 1e-4;					// �޵�λ
	inc = 53.0 * pi / 180.0;	// rad
	ome = 100.0 * pi / 180.0;	// rad
	Omega = 327.0 * pi / 180.0; // rad
	f = 50.0 * pi / 180.0;		// rad

	// ����վ -- ��γ��
	Lon = 120.0 * pi / 180.0; // rad
	lat = 25.0 * pi / 180.0;  // rad
	HH = 1.0;				  // km


	dT = 1;				// ���沽��/sec 	
	tsim = 1 * 86400;	// ������ʱ��/sec   

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	oev_origin[0] = a;		oev_origin[1] = e;		oev_origin[2] = inc;
	oev_origin[3] = ome;	oev_origin[4] = Omega;	oev_origin[5] = f;

	Time0[0] = years;	Time0[1] = month;	Time0[2] = day;
	Time0[3] = hour;	Time0[4] = minute;	Time0[5] = sec;

	// ���� ECIϵ��λ��
	double* ECI;
	ECI = orb2eci(oev_origin);

	// ����վ ECFϵ��λ��
	double* ground_ECF;
	ground_ECF = LLH2ECF(lat, Lon, HH);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���÷������ʱ��

	int m_dT = round(tsim / dT + 1); // ����

	double r_eci[3] = { ECI[0], ECI[1], ECI[2] };
	double v_eci[3] = { ECI[3], ECI[4], ECI[5] };

	// ���崢�������õĶ�ά��̬����
	vector<vector<double>> r_out(3), v_out(3);
	//����ÿһ�е�Ԫ�ظ���
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

	///////// �������ǹ������ /////////
	double t = 0;
	int ii = 0;
	int jj = 0;

	for (int i = 0; i < m_dT; i++)
	{
		t = t + dT;
		ii = ii++;

		// ������ʱ����
		double PV[6] = { 0 };
		for (int i = 0; i < 3; i++)
		{
			PV[i] = r_eci[i];
			PV[i + 3] = v_eci[i];
		}

		double statenew[6] = { 0 };
		double statenew_d[6] = { 0 };
		double F[3] = { 0 };

		// ���ƺ�����λ���ٶ�
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
	// ���� ����վ������ �۲����
	vector<vector<double>> Window_all;		 // ����ʱ��
	vector<vector<double>> distance_all;	 // ����

	int col = r_out[0].size() - 1;	// cout << col << endl;
	double Time_new[6] = { 0.0 };

	// ���� ��ʼʱ��
	for (int i = 0; i < 5; i++) Time_new[i] = Time0[i];

	for (int i = 0; i < col; i++)
	{
		// ��ǰʱ��
		Time_new[5] = Time0[5] + i * dT;

		// ECI��ECF����ת����
		double** C_RNP_ = C_RNP_Fun(Time_new);
		double C_RNP[3][3] = { 0.0 };
		for (int m = 0; m < 3; m++)
		{
			for (int n = 0; n < 3; n++) C_RNP[m][n] = C_RNP_[m][n];
		}

		// ECF��ECI����ת����
		double** C_RNPT_ = mattrans(C_RNP);
		double C_RNPT[3][3] = { 0.0 };
		for (int m = 0; m < 3; m++)
		{
			for (int n = 0; n < 3; n++) C_RNPT[m][n] = C_RNPT_[m][n];
		}

		// ����վECI������
		double* ground_ECI = matm31(C_RNPT, ground_ECF);

		// ��ǰ���� λ��
		double r_sat[3] = { 0.0 };
		r_sat[0] = r_out[0][i];
		r_sat[1] = r_out[1][i];
		r_sat[2] = r_out[2][i];

		// �Ƕ�ɸѡ
		double r1[3] = { 0.0 };
		double r2[3] = { 0.0 };

		r1[0] = ground_ECI[0];	r1[1] = ground_ECI[1];	r1[2] = ground_ECI[2];

		// ����վָ�����ǵ�ʸ��
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
		printf("����δ����⵽ָ���ص�\n");//�ж��Ƿ�����⵽
	}
	else
	{

		// ǰ��ʱ�� ��һ�� ���ʷ�Ϊ����
		for (int i = 0; i < row - 1; i++)
		{
			if (Window_all[i + 1][0] - Window_all[i][0] != dT)
			{
				int obsp = i;
				obs.insert(obs.end(), obsp);
			}
		}


		// �۲����
		int pic = obs.size() + 1;

		// ����۲�ʱ��ֲ�
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

		// ��� UTCG �۲�ʱ��
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
		cout << "��ʾ���������������� -- �۲����.txt" << endl;
		cout << endl;
		cout << "		  �۲⿪ʼʱ��(UTCG)	    �۲����ʱ��(UTCG)	  �۲����ʱ��(sec)" << endl;
		for (int i = 0; i < pic; i++)
		{
			cout << "��" << to_string(i + 1) << "�ι۲�	" << All_Timeperiod[0][i] << "	  " << All_Timeperiod[1][i] << "	" << Duration[i] << endl;
		}
		cout << endl;


		ofstream fwD;
		fwD.open("../�۲����.txt");
		fwD << "		  �۲⿪ʼʱ��(UTCG)	    �۲����ʱ��(UTCG)	  �۲����ʱ��(sec)" << endl;
		for (int i = 0; i < pic; i++)
		{
			fwD << "��" << to_string(i + 1) << "�ι۲�	" << All_Timeperiod[0][i] << "	  "
				<< All_Timeperiod[1][i] << "	" << Duration[i] << endl;
		}
		fwD.close();
	}

}