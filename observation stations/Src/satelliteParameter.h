// satelliteParameter.h: interface for the satelliteParameter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_satelliteParameter_H__)
#define _satelliteParameter_H__


#include "AstroLib.h"
#include <iostream>
#include <string>
#include <complex>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <math.h>
#include <numeric>
#include <cstring>
#include <cstdlib>
#include <algorithm>

// 基础函数

double fix(double x);
long double modulo(long double x);
double sign(double x);
double atan3(double x, double z);
double r2r(double x);
double dot(double a[3], double b[3]);
double norm(double a[3]);


double* orb2eci(double oev[6]);
double* LLH2ECF(double Latitude, double longitude, double altitude);

long double julian(double x[6]);
double gast(double x);

double linemulti(double x[3], double y[3]);
double** matmulti(double x[3][3], double y[3][3]);
double** mattrans(double x[3][3]);
double* matm31(double x[3][3], double y[3]);


double** R1(double x);
double** R2(double x);
double** R3(double x);

double** nutation(double x);
double** precession(double x);

double** C_RNP_Fun(double Time[6]);

void Add1Day(int year, int month, int day, int* out_year, int* out_month, int* out_day);
double* AddSec(double Time[6], double addsec);

string UTCGTime(double* UTCG0);


#endif // !defined(_satelliteParameter_H__)
