//***********************************************************************
/// 星历读取
/// @Author	Li Pengfei
/// @Date	2024-4-18
//***********************************************************************


#include "LowOrbitConstellation1.h"
#include <fstream>

using namespace std;


void CEx01::Readxingli(int i, vector<double>& time, vector<double>& pos)
{
	ifstream ifs;
	switch (i)
	{
	case 1:
		ifs.open(".\\Output\\观测卫星1星历.txt", ios::in);
		break;
	case 2:
		ifs.open(".\\Output\\观测卫星2星历.txt", ios::in);
		break;
	case 3:
		ifs.open(".\\Output\\目标卫星星历.txt", ios::in);
		break;
	default:
		break;
	}
	//读取正确匹配特征点

	vector<double> target_1, target_2, target_3;		//用于存放目标数据

	vector<string> item;				//用于存放文件中的一行数据

	string temp;						//把文件中的一行数据作为字符串放入容器中

	while (getline(ifs, temp))          //利用getline（）读取每一行，并放入到 item 中
	{
		item.push_back(temp);
	}

	for (auto it = item.begin(); it != item.end(); it++)
	{
		//cout << *it << endl;

		istringstream istr(*it);                 //其作用是把字符串分解为单词(在此处就是把一行数据分为单个数据)

		string str;

		int count = 0;							 //统计一行数据中单个数据个数

		//获取文件中的第 1、2 列数据
		while (istr >> str)                      //以空格为界，把istringstream中数据取出放入到依次s中
		{
			//获取第1列数据
			if (count == 0)
			{
				double r = atof(str.c_str());
				time.push_back(r);
			}
			else if (count == 1)
			{
				double r = atof(str.c_str());

				target_1.push_back(r);
			}
			//获取第2列数据
			else if (count == 2)
			{
				double r = atof(str.c_str());

				target_2.push_back(r);
			}
			else if (count == 3)
			{
				double r = atof(str.c_str());

				target_3.push_back(r);
			}
			count++;
		}

	}
	pos.insert(pos.end(), target_1.begin(), target_1.end());
	pos.insert(pos.end(), target_2.begin(), target_2.end());
	pos.insert(pos.end(), target_3.begin(), target_3.end());
}