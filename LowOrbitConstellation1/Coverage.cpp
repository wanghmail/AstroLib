//***********************************************************************
/// 视场分析，输出观测结果
/// @Author	Li Pengfei
/// @Date	2024-4-18
//***********************************************************************

#include "LowOrbitConstellation1.h"

using namespace std;

void CEx01::Coverage()
{
	//ifstream ifs;
	int i = 0, j = 0, n = 0, hh = 0, hhh = 0, jishu = 0;
	vector<double> pos1, pos2, pos3, angle_13, angle_23, xx_13, xx_23, xx_10, xx_20, xx_03, xx_013, xx_023, seein1, seeout1, seeout2, seein2;
	vector<bool> see1, see2;
	double xx13[3], xx10[3], xx23[3], xx20[3];
	double Re = 6371000;
	vector<double> time;
	double angle13, angle23;
	for (i = 1; i < 4; i++)
	{
		switch (i)
		{
		case 1:
			Readxingli(i, time, pos1);
			break;
		case 2:
			Readxingli(i, time, pos2);
			break;
		case 3:
			Readxingli(i, time, pos3);
			break;
		default:
			break;
		}
	}
	n = size(time) / 3;

	for (j = 0; j < n; j++)//长度、角度计算
	{
		xx13[0] = pos3[j] - pos1[j]; xx13[1] = pos3[j + n] - pos1[j + n]; xx13[2] = pos3[j + 2 * n] - pos1[j + 2 * n];
		xx10[0] = -pos1[j]; xx10[1] = -pos1[j + n]; xx10[2] = -pos1[j + 2 * n];
		xx23[0] = pos3[j] - pos2[j]; xx23[1] = pos3[j + n] - pos2[j + n]; xx23[2] = pos3[j + 2 * n] - pos2[j + 2 * n];
		xx20[0] = -pos2[j]; xx20[1] = -pos2[j + n]; xx20[2] = -pos2[j + 2 * n];

		xx_13.push_back(sqrt((xx13[0] * xx13[0] + xx13[1] * xx13[1] + xx13[2] * xx13[2])));
		xx_10.push_back(sqrt((xx10[0] * xx10[0] + xx10[1] * xx10[1] + xx10[2] * xx10[2])));
		xx_03.push_back(sqrt(pos3[j] * pos3[j] + pos3[j + n] * pos3[j + n] + pos3[j + 2 * n] * pos3[j + 2 * n]));
		xx_23.push_back(sqrt((xx23[0] * xx23[0] + xx23[1] * xx23[1] + xx23[2] * xx23[2])));
		xx_20.push_back(sqrt((xx20[0] * xx20[0] + xx20[1] * xx20[1] + xx20[2] * xx20[2])));

		xx_013.push_back(sqrt((xx_13[j] + xx_10[j] + xx_03[j]) * (xx_13[j] + xx_10[j] - xx_03[j]) * (xx_13[j] - xx_10[j] + xx_03[j]) * (-xx_13[j] + xx_10[j] + xx_03[j])) / 2 / xx_13[j]);
		xx_023.push_back(sqrt((xx_23[j] + xx_20[j] + xx_03[j]) * (xx_23[j] + xx_20[j] - xx_03[j]) * (xx_23[j] - xx_20[j] + xx_03[j]) * (-xx_23[j] + xx_20[j] + xx_03[j])) / 2 / xx_23[j]);

		angle13 = 180 / 3.14 * acos((xx13[0] * xx10[0] + xx13[1] * xx10[1] + xx13[2] * xx10[2]) / xx_13[j] / xx_10[j]);
		angle23 = 180 / 3.14 * acos((xx23[0] * xx20[0] + xx23[1] * xx20[1] + xx23[2] * xx20[2]) / xx_23[j] / xx_20[j]);
		if (angle13 > 90)
		{
			angle13 = 180 - angle13;
		}
		if (angle23 > 90)
		{
			angle23 = 180 - angle23;
		}
		angle_13.push_back(angle13);
		angle_23.push_back(angle23);

		//能否观察到目标星

		if (angle_13[j] > 40 || (xx_13[j] > 5685e3))
		{
			see1.push_back(false);
		}
		else
		{
			see1.push_back(true);
		}
		if (angle_23[j] > 40 || (xx_23[j] > 5685e3))
		{
			see2.push_back(false);
		}
		else
		{
			see2.push_back(true);
		}
	}


	//起初就在视场中
	if (see1[0])
	{
		seein1.push_back(time[0]);
	}
	if (see2[0])
	{
		seein2.push_back(time[0]);
	}
	for (j = 1; j < n; j++)
	{
		if (see1[j] && !see1[j - 1])
		{
			seein1.push_back(time[j]);
		}
		if (!see1[j] && see1[j - 1])
		{
			seeout1.push_back(time[j]);
		}
		if (see2[j] && !see2[j - 1])
		{
			seein2.push_back(time[j]);
		}
		if (!see2[j] && see2[j - 1])
		{
			seeout2.push_back(time[j]);
		}

	}
	//时间结束还在视场中
	if (size(seein1) > size(seeout1))
	{
		seeout1.push_back(time[n - 1]);
	}
	if (size(seein2) > size(seeout2))
	{
		seeout2.push_back(time[n - 1]);
	}

	cout << "卫星编号" << '\t' << "起始时间" << '\t' << "结束时间" << '\t' << "持续时长" << endl;

	auto it = seein1.begin(); auto itt = seeout1.begin();

	for (; it != seein1.end(), itt != seeout1.end(); it++, itt++)
	{
		cout << "卫星1" << '\t' << '\t' << *it << '\t' << '\t' << *itt << '\t' << '\t' << *itt - *it << endl;
	}

	auto ittt = seein2.begin(); auto itttt = seeout2.begin();

	for (; ittt != seein2.end(), itttt != seeout2.end(); ittt++, itttt++)
	{
		cout << "卫星2" << '\t' << '\t' << *ittt << '\t' << '\t' << *itttt << '\t' << '\t' << *itttt - *ittt << endl;
	}

}