// satelliteParameter.cpp: implementation of the satelliteParameter class.
//
//////////////////////////////////////////////////////////////////////


#include "satelliteParameter.h"
#include "AstroLib.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

double dot(double a[3], double b[3])
{
	/// 向量点乘
	double output = 0;
	output = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	return output;
}

double norm(double a[3])
{
	// 向量的二范数
	double output = 0;
	output = sqrt(dot(a, a));
	return output;
}

double** mattrans(double x[3][3])
{
	///矩阵的转置
	double** mat = 0;
	mat = new double*[3];
	for (int i = 0; i < 3; i++)
	{
		*(mat + i) = new double[3];
	}

	mat[0][0] = x[0][0]; mat[0][1] = x[1][0]; mat[0][2] = x[2][0];
	mat[1][0] = x[0][1]; mat[1][1] = x[1][1]; mat[1][2] = x[2][1];
	mat[2][0] = x[0][2]; mat[2][1] = x[1][2]; mat[2][2] = x[2][2];
	return mat;
}

double* matm31(double x[3][3], double y[3])
{
	/// 3*3 矩阵  *   3*1 矩阵
	static double mat[3];
	mat[0] = 0; mat[1] = 0; mat[2] = 0;
	double x1[3], x2[3], x3[3];
	x1[0] = x[0][0]; x1[1] = x[0][1]; x1[2] = x[0][2];
	x2[0] = x[1][0]; x2[1] = x[1][1]; x2[2] = x[1][2];
	x3[0] = x[2][0]; x3[1] = x[2][1]; x3[2] = x[2][2];
	mat[0] = linemulti(x1, y);
	mat[1] = linemulti(x2, y);
	mat[2] = linemulti(x3, y);

	return mat;
}

long double julian(double x[6])
{
	double year, month, day, hour, minute, second;
	long double jd;
	year = x[0];
	month = x[1];
	day = x[2];
	hour = x[3];
	minute = x[4];
	second = x[5];
	jd = 367 * year - floor((7 * (year + floor((month + 9) / 12))) / 4) + floor(275 * month / 9) + day + 1721013.5 + ((second / 60 + minute) / 60 + hour) / 24;
	return jd;
}

double** C_RNP_Fun(double Time[6])
{
	double jdate0 = julian(Time);
	double gst0 = gast(jdate0);

	double R0[3][3] = { 0.0 };
	double N0[3][3] = { 0.0 };
	double P0[3][3] = { 0.0 };

	R0[0][0] = cos(gst0);    R0[0][1] = sin(gst0);   R0[0][2] = 0.0;
	R0[1][0] = -sin(gst0);   R0[1][1] = cos(gst0);   R0[1][2] = 0.0;
	R0[2][0] = 0.0;          R0[2][1] = 0.0;         R0[2][2] = 1.0;

	double ** N0_ = nutation(jdate0);
	double ** P0_ = precession(jdate0);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			N0[i][j] = N0_[i][j];
			P0[i][j] = P0_[i][j];
		}
	}

	double RN[3][3] = { 0.0 };
	double RNP[3][3] = { 0.0 };

	double ** RN_;
	RN_ = matmulti(R0, N0);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) RN[i][j] = RN_[i][j];
	}

	double** RNP_;
	RNP_ = matmulti(RN, P0);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) RNP[i][j] = RNP_[i][j];
	}

	double** mat = 0;
	mat = new double*[3];
	for (int i = 0; i < 3; i++)
	{
		*(mat + i) = new double[3];
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mat[i][j] = RNP[i][j];
		}
	}
	return mat;

}


void Add1Day(int year, int month, int day, int* out_year, int* out_month, int* out_day)
{
	int add_day = day + 1;

	if (add_day > 28 && add_day < 31)
	{
		if (month == 2 && add_day == 29)
		{
			if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
			{
				*out_year = year;
				*out_month = month;
				*out_day = add_day;
			}
			else
			{
				*out_year = year;
				*out_month = month + 1;
				*out_day = 1;
			}

		}
		else if (month == 2 && add_day == 30)
		{
			*out_year = year;
			*out_month = month + 1;
			*out_day = 1;
		}
		else
		{
			*out_year = year;
			*out_month = month;
			*out_day = add_day;
		}

	}
	else if (add_day == 31)
	{
		if (month == 4 || month == 6 || month == 9 || month == 11)
		{
			*out_year = year;
			*out_month = month + 1;
			*out_day = 1;
		}
		else
		{
			*out_year = year;
			*out_month = month;
			*out_day = add_day;
		}


	}
	else if (add_day == 32)
	{
		if (month == 12)
		{
			*out_year = year + 1;
			*out_month = 1;
			*out_day = 1;
		}
		else
		{
			*out_year = year;
			*out_month = month + 1;
			*out_day = 1;
		}
	}
	else
	{
		*out_year = year;
		*out_month = month;
		*out_day = add_day;
	}

}

double* AddSec(double Time[6], double addsec)
{
	static double UTCG[6] = { 0.0 };

	double ReTime = addsec;
	int addD = floor(ReTime / 86400.0);

	ReTime = ReTime - 86400.0 * addD;
	int addH = floor(ReTime / 3600.0);

	ReTime = ReTime - 3600.0 * addH;
	int addM = floor(ReTime / 60.0);

	ReTime = ReTime - 60.0 * addM;
	double addS = ReTime;

	double allY, allMO, allD, allH, allM, allS;

	allS = addS + Time[5];
	allM = addM + Time[4];
	allH = addH + Time[3];

	if (allS >= 60.0)
	{
		allS = allS - 60.0;
		allM = allM + 1;
	}

	if (allM >= 60.0)
	{
		allM = allS - 60.0;
		allH = allH + 1;
	}

	if (allH >= 24.0)
	{
		allH = allH - 24.0;
		addD = addD + 1;
	}

	int default_year = 0;		int default_month = 0;			int default_day = 0;
	int* year_ = &default_year;	int* month_ = &default_month;	int* day_ = &default_day;

	if (addD >= 1)
	{
		for (int i = 0; i < addD; i++)
		{
			Add1Day(Time[0], Time[1], Time[2], year_, month_, day_);
		}
		allY = *year_;
		allMO = *month_;
		allD = *day_;
	}
	else
	{
		allY = Time[0];
		allMO = Time[1];
		allD = Time[2];
	}

	UTCG[0] = allY;
	UTCG[1] = allMO;
	UTCG[2] = allD;
	UTCG[3] = allH;
	UTCG[4] = allM;
	UTCG[5] = allS;

	return UTCG;

}


string UTCGTime(double* UTCG0)
{
	//// 月
	string UTCG0_mo = "";
	if (int(UTCG0[1]) < 10)
	{
		UTCG0_mo = "0" + to_string(int(UTCG0[1]));
	}
	else
	{
		UTCG0_mo = to_string(int(UTCG0[1]));
	}

	//// 日
	string UTCG0_d = "";
	if (int(UTCG0[2]) < 10)
	{
		UTCG0_d = "0" + to_string(int(UTCG0[2]));
	}
	else
	{
		UTCG0_d = to_string(int(UTCG0[2]));
	}

	//// 时
	string UTCG0_h = "";
	if (int(UTCG0[3]) < 10)
	{
		UTCG0_h = "0" + to_string(int(UTCG0[3]));
	}
	else
	{
		UTCG0_h = to_string(int(UTCG0[3]));
	}

	//// 分
	string UTCG0_m = "";
	if (int(UTCG0[4]) < 10)
	{
		UTCG0_m = "0" + to_string(int(UTCG0[4]));
	}
	else
	{
		UTCG0_m = to_string(int(UTCG0[4]));
	}

	//// 秒
	string UTCG0_s = "";
	stringstream UTCG0_sec;
	UTCG0_sec << std::setiosflags(std::ios::fixed) << std::setprecision(2) << UTCG0[5];
	if (UTCG0[5] < 10.0)
	{
		UTCG0_s = "0" + UTCG0_sec.str();
	}
	else
	{
		UTCG0_s = UTCG0_sec.str();
	}


	string UTCG0_str = to_string(int(UTCG0[0])) + "/" + UTCG0_mo + "/" + UTCG0_d + " " +
		UTCG0_h + ":" + UTCG0_m + ":" + UTCG0_s;

	return UTCG0_str;
}


