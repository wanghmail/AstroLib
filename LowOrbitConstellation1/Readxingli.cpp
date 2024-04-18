//***********************************************************************
/// ������ȡ
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
		ifs.open(".\\Output\\�۲�����1����.txt", ios::in);
		break;
	case 2:
		ifs.open(".\\Output\\�۲�����2����.txt", ios::in);
		break;
	case 3:
		ifs.open(".\\Output\\Ŀ����������.txt", ios::in);
		break;
	default:
		break;
	}
	//��ȡ��ȷƥ��������

	vector<double> target_1, target_2, target_3;		//���ڴ��Ŀ������

	vector<string> item;				//���ڴ���ļ��е�һ������

	string temp;						//���ļ��е�һ��������Ϊ�ַ�������������

	while (getline(ifs, temp))          //����getline������ȡÿһ�У������뵽 item ��
	{
		item.push_back(temp);
	}

	for (auto it = item.begin(); it != item.end(); it++)
	{
		//cout << *it << endl;

		istringstream istr(*it);                 //�������ǰ��ַ����ֽ�Ϊ����(�ڴ˴����ǰ�һ�����ݷ�Ϊ��������)

		string str;

		int count = 0;							 //ͳ��һ�������е������ݸ���

		//��ȡ�ļ��еĵ� 1��2 ������
		while (istr >> str)                      //�Կո�Ϊ�磬��istringstream������ȡ�����뵽����s��
		{
			//��ȡ��1������
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
			//��ȡ��2������
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