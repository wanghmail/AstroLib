#include "ToolFunctions.h"

double fix(double x)
{
    if (x == 0)
        return 0;
    else if (x > 0)
        return floor(x);
    else
        return floor(x) + 1;
}

long double modulo(long double x)
{
    /*
    * % modulo 2 pi function

    % input

    %  x = argument (radians)

    % output

    %  y = x modulo 2 pi
    */
    long double cons = 2;
    long double pi2 = cons * pi;
    long double y;
    y = x - pi2 * fix(x / pi2);
    if (y < 0)
        y = y + pi2;
    return y;
}

double sign(double x)
{
    if (x > 0)
        return 1;
    else if (x == 0)
        return 0;
    else
        return -1;
}

//double fabs(double x)
//{
//    double re;
//    re = sign(x) * x;
//    return re;
//}

double atan3(double x, double z)
{
    /*% four quadrant inverse tangent

    % input

    %  a = sine of angle
    %  b = cosine of angle

    % output

    %  y = angle (radians; 0 =< c <= 2 * pi)
    */
    double epsilon = 0.0000000001;
    double pidiv2 = 0.5 * pi;
    double y;
    double c;
    if (fabs(x) < epsilon)
    {
        y = (1 - sign(z)) * pidiv2;
        return y;
    }
    c = (2 - sign(x)) * pidiv2;
    if (fabs(z) < epsilon)
        return c;
    else
    {
        y = c + sign(x) * sign(z) * (fabs(atan(x / z)) - pidiv2);
        return y;
    }
}

double r2r(double x)
{
    /*
    * % revolutions to radians function

    % input

    %  x = argument (revolutions; 0 <= x <= 1)

    % output

    %  y = equivalent x (radians; 0 <= y <= 2 pi)
    */

    double y = 0;
    y = 2 * pi * (x - fix(x));
    return y;
}

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


double * orb2eci(double oev[6])
{
    /*% convert classical orbital elements to eci state vector

        % input
        % oev[1) = semimajor axis(kilometers)
        % oev[2) = orbital eccentricity(non - dimensional)
        % (0 <= eccentricity < 1)
        % oev[3) = orbital inclination(radians)
        % (0 <= inclination <= pi)
        % oev[4) = argument of perigee(radians)
        % (0 <= argument of perigee <= 2 pi)
        % oev[5) = right ascension of ascending node(radians)
        % (0 <= raan <= 2 pi)
        % oev[6) = true anomaly(radians)
        % (0 <= true anomaly <= 2 pi)

        % output
        % r = eci position vector(kilometers)
        % v = eci velocity vector(kilometers / second)*/

        // unload orbital elements array

    static double RV[6] = { 0.0 };

    double sma = oev[0];
    double ecc = oev[1];
    double inc = oev[2];
    double argper = oev[3];
    double raan = oev[4];
    double tanom = oev[5];

    double slr = sma * (1.0 - ecc * ecc);

    double rm = slr / (1.0 + ecc * cos(tanom));

    double arglat = argper + tanom;

    double sarglat = sin(arglat);
    double carglat = cos(arglat);

    double c4 = sqrt(mu / slr);
    double c5 = ecc * cos(argper) + carglat;
    double c6 = ecc * sin(argper) + sarglat;

    double sinc = sin(inc);
    double cinc = cos(inc);

    double sraan = sin(raan);
    double craan = cos(raan);

    //% position vector
    RV[0] = rm * (craan * carglat - sraan * cinc * sarglat);
    RV[1] = rm * (sraan * carglat + cinc * sarglat * craan);
    RV[2] = rm * sinc * sarglat;

    //% velocity vector

    RV[3] = -c4 * (craan * c6 + sraan * cinc * c5);
    RV[4] = -c4 * (sraan * c6 - craan * cinc * c5);
    RV[5] = c4 * c5 * sinc;

    return RV;

}


double * LLH2ECF(double Latitude, double longitude, double altitude)
{
    /*
    % Purpose:  This fuction convertes Geodetic latitude
    %           and longitude to ECEF coordinates.
    %
    % Inputs:  o Lat  - Geodetic latitude of spacecraft in radians
    %          o Long - Longitude of spacecraft in radians
    %          o H    - Altitude in km
    %
    % Outputs: o XYZ  - A 3x1 vector of position in Earth-Centered Fixed coordinates in km
     */

    static double rt_ecf[3] = {0.0};

    double esqr = 2 * flat - flat * flat; // 地球第一偏心率平方

    double N = Re / sqrt(1 - esqr * sin(Latitude) * sin(Latitude)); // 基准椭球体的曲率半径

    double X = (N + altitude) * cos(Latitude) * cos(longitude);
    double Y = (N + altitude) * cos(Latitude) * sin(longitude);
    double Z = (N * pow((1 - flat), 2) + altitude) * sin(Latitude);

    rt_ecf[0] = X;
    rt_ecf[1] = Y;
    rt_ecf[2] = Z;

    return rt_ecf;

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

double gast(double x)
{
    long double dtr, atr;
    long double t;
    long double t2, t3;
    long double l, lp, lraan;
    long double dpsi, deps;
    long double eps0, obliq;
    long double gstm, gst;
    dtr = pi / 180.0;
    atr = dtr / 3600.0;
    t = (x - 2451545.0) / 36525.0;
    t2 = t * t;
    t3 = t * t2;
    l = modulo(dtr * (280.4665 + 36000.7698 * t));
    lp = modulo(dtr * (218.3165 + 481267.8813 * t));
    lraan = modulo(dtr * (125.04452 - 1934.136261 * t));
    dpsi = atr * (-17.2 * sin(lraan) - 1.32 * sin(2.0 * l) - 0.23 * sin(2.0 * lp) + 0.21 * sin(2.0 * lraan));
    deps = atr * (9.2 * cos(lraan) + 0.57 * cos(2.0 * l) + 0.1 * cos(2.0 * lp) - 0.09 * cos(2.0 * lraan));
    long double iner1, iner2;
    iner1 = (26.0 / 60);
    iner2 = 21.448 / 3600.0;
    eps0 = modulo(dtr * (23 + iner1 + iner2) + atr * (-46.815 * t - 0.00059 * t2 + 0.001813 * t3));
    obliq = eps0 + deps;
    gstm = modulo(dtr * (280.46061837 + 360.98564736629 * (x - 2451545) + 0.000387933 * t2 - t3 / 38710000));
    gst = modulo(gstm + dpsi * cos(obliq));
    return gst;
}




double linemulti(double x[3], double y[3])
{
    ////向量乘法
    double res = 0;
    res = x[0] * y[0] + x[1] * y[1] + x[2] * y[2];
    return res;
}

double ** matmulti(double x[3][3], double y[3][3])
{
    ////矩阵乘法
    double** mat = 0;
    mat = new double* [3];
    for (int i = 0; i < 3; i++)
    {
        *(mat + i) = new double[3];
    }

    int i, j;
    double matx[3], maty[3];
    mat[0][0] = 0; mat[0][1] = 0; mat[0][2] = 0;
    mat[1][0] = 0; mat[1][1] = 0; mat[1][2] = 0;
    mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 0;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            matx[0] = x[i][0]; matx[1] = x[i][1]; matx[2] = x[i][2];
            maty[0] = y[0][j]; maty[1] = y[1][j]; maty[2] = y[2][j];
            mat[i][j] = linemulti(matx, maty);
        }
    }
    return mat;
}

double** mattrans(double x[3][3])
{
    ///矩阵的转置
    double** mat = 0;
    mat = new double* [3];
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



double ** R1(double x)
{
    double** r = 0;     //初始化一个二维数组
    r = new double* [3];   //建立包含row个指针的指针数组
    for (int i = 0; i < 3; i++)
    {
        *(r + i) = new double[3];   //每个一维指针数组的长度都是col
    }

    r[0][0] = 1; r[0][1] = 0; r[0][2] = 0;
    r[1][0] = 0; r[1][1] = cos(x); r[1][2] = sin(x);
    r[2][0] = 0; r[2][1] = -sin(x); r[2][2] = cos(x);
    return r;
}

double ** R2(double x)
{
    double** r = 0;     
    r = new double* [3];   
    for (int i = 0; i < 3; i++)
    {
        *(r + i) = new double[3];  
    }

    r[0][0] = cos(x); r[0][1] = 0; r[0][2] = -sin(x);
    r[1][0] = 0; r[1][1] = 1; r[1][2] = 0;
    r[2][0] = sin(x); r[2][1] = 0; r[2][2] = cos(x);
    return r;
}

double** R3(double x)
{
    double** r = 0;
    r = new double* [3];
    for (int i = 0; i < 3; i++)
    {
        *(r + i) = new double[3];
    }

    r[0][0] = cos(x); r[0][1] = sin(x); r[0][2] = 0;
    r[1][0] = -sin(x); r[1][1] = cos(x); r[1][2] = 0;
    r[2][0] = 0;       r[2][1] = 0; r[2][2] = 1;
    return r;
}


double** nutation(double x)
{
    ///章动
   // Nutation matrix
   //
   // Input:  o JD - Julian date
   //
   // Output : o N - Nutation matrix

    int i;

    double T;
    T = (x - 2451545) / 36525;

    double a1[106] = { 0, 0, -2, 2, -2, 1, 0, 2, 0, 0,
        0, 0, 0, 2, 0, 0, 0, 0, 0, -2,
        0, 2, 0, 1, 2, 0, 0, 0, -1, 0,
        0, 1, 0, 1, 1, -1, 0, 1, -1, -1,
        1, 0, 2, 1, 2, 0, -1, -1, 1, -1,
        1, 0, 0, 1, 1, 2, 0, 0, 1, 0,
        1, 2, 0, 1, 0, 1, 1, 1, -1, -2,
        3, 0, 1, -1, 2, 1, 3, 0, -1, 1,
        -2, -1, 2, 1, 1, -2, -1, 1, 2, 2,
        1, 0, 3, 1, 0, -1, 0, 0, 0, 1,
        0, 1, 1, 2, 0, 0 };

    double a2[106] = { 0, 0, 0, 0, 0, -1, -2, 0, 0, 1,
        1, -1, 0, 0, 0, 2, 1, 2, -1, 0,
        -1, 0, 1, 0, 1, 0, 1, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, -1, 0, 0, 0, 0, 0, 0, 0,
        -1, 0, 1, 0, 0, 1, 0, -1, -1, 0,
        0, -1, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, -1, 0, 0,
        0, 0, 0, 0, 1, -1, 0, 0, 1, 0,
        -1, 1, 0, 0, 0, 1 };

    double a3[106] = { 0, 0, 2, -2, 2, 0, 2, -2, 2, 0,
        2, 2, 2, 0, 2, 0, 0, 2, 0, 0,
        2, 0, 2, 0, 0, -2, -2, 0, 0, 2,
        2, 0, 2, 2, 0, 2, 0, 0, 0, 2,
        2, 2, 0, 2, 2, 2, 2, 0, 0, 2,
        0, 2, 2, 2, 0, 2, 0, 2, 2, 0,
        0, 2, 0, -2, 0, 0, 2, 2, 2, 0,
        2, 2, 2, 2, 0, 0, 0, 2, 0, 0,
        2, 2, 0, 2, 2, 2, 4, 0, 2, 2,
        0, 4, 2, 2, 2, 0, -2, 2, 0, -2,
        2, 0, -2, 0, 2, 0 };

    double a4[106] = { 0, 0, 0, 0, 0, -1, -2, 0, -2, 0,
        -2, -2, -2, -2, -2, 0, 0, -2, 0, 2,
        -2, -2, -2, -1, -2, 2, 2, 0, 1, -2,
        0, 0, 0, 0, -2, 0, 2, 0, 0, 2,
        0, 2, 0, -2, 0, 0, 0, 2, -2, 2,
        -2, 0, 0, 2, 2, -2, 2, 2, -2, -2,
        0, 0, -2, 0, 1, 0, 0, 0, 2, 0,
        0, 2, 0, -2, 0, 0, 0, 1, 0, -4,
        2, 4, -4, -2, 2, 4, 0, -2, -2, 2,
        2, -2, -2, -2, 0, 2, 0, -1, 2, -2,
        0, -2, 2, 2, 4, 1 };

    double a5[106] = { 1, 2, 1, 0, 2, 0, 1, 1, 2, 0,
        2, 2, 1, 0, 0, 0, 1, 2, 1, 1,
        1, 1, 1, 0, 0, 1, 0, 2, 1, 0,
        2, 0, 1, 2, 0, 2, 0, 1, 1, 2,
        1, 2, 0, 2, 2, 0, 1, 1, 1, 1,
        0, 2, 2, 2, 0, 2, 1, 1, 1, 1,
        0, 1, 0, 0, 0, 0, 0, 2, 2, 1,
        2, 2, 2, 1, 1, 2, 0, 2, 2, 0,
        2, 2, 0, 2, 1, 2, 2, 0, 1, 2,
        1, 2, 2, 0, 1, 1, 1, 2, 0, 0,
        1, 1, 0, 0, 2, 0 };

    double A1[106] = { -171996, 2062, 46, 11, -3, -3, -2, 1, -13187, 1426,
        -517, 217, 129, 48, -22, 17, -15, -16, -12, -6,
        -5, 4, 4, -4, 1, 1, -1, 1, 1, -1,
        -2274, 712, -386, -301, -158, 123, 63, 63, -58, -59,
        -51, -38, 29, 29, -31, 26, 21, 16, -13, -10,
        -7, 7, -7, -8, 6, 6, -6, -7, 6, -5,
        5, -5, -4, 4, -4, -3, 3, -3, -3, -2,
        -3, -3, 2, -2, 2, -2, 2, 2, 1, -1,
        1, -2, -1, 1, -1, -1, 1, 1, 1, -1,
        -1, 1, 1, -1, 1, 1, -1, -1, -1, -1,
        -1, -1, -1, 1, -1, 1 };

    double B1[106] = { 92025, -895, -24, 0, 1, 0, 1, 0, 5736, 54,
        224, -95, -70, 1, 0, 0, 9, 7, 6, 3,
        3, -2, -2, 0, 0, 0, 0, 0, 0, 0,
        977, -7, 200, 129, -1, -53, -2, -33, 32, 26,
        27, 16, -1, -12, 13, -1, -10, -8, 7, 5,
        0, -3, 3, 3, 0, -3, 3, 3, -3, 3,
        0, 3, 0, 0, 0, 0, 0, 1, 1, 1,
        1, 1, -1, 1, -1, 1, 0, -1, -1, 0,
        -1, 1, 0, -1, 1, 1, 0, 0, -1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0 };

    for (int i = 0; i < 106; i++)
    {
        A1[i] = A1[i] * 0.0001;
        B1[i] = B1[i] * 0.0001;
    }

    double A2[106] = { 0 };
    double B2[106] = { 0 };

    A2[0] = -174.2;
    A2[1] = 0.2;
    A2[8] = -1.6;
    A2[9] = -3.4;
    A2[10] = 1.2;
    A2[11] = -0.5;
    A2[12] = 0.1;
    A2[15] = -0.1;
    A2[17] = 0.1;
    A2[30] = -0.2;
    A2[31] = 0.1;
    A2[32] = -0.4;
    A2[37] = 0.1;
    A2[38] = -0.1;

    B2[0] = 8.9;
    B2[1] = 0.5;
    B2[8] = -3.1;
    B2[9] = -0.1;
    B2[10] = -0.6;
    B2[11] = 0.3;
    B2[30] = -0.5;
    B2[33] = -0.1;

    for (int i = 0; i < 106; i++)
    {
        A2[i] = A2[i] * 0.0001;
        B2[i] = B2[i] * 0.0001;
    }


    double l, ll, F, D, Omega;
    l = (134.96340251 + (1717915923.2178 * T + 31.8792 * T * T + 0.051635 * T * T * T - 0.0002447 * T * T * T * T) / 3600) * pi / 180;
    ll = (357.52910918 + (129596581.0481 * T - 0.5532 * T * T + 0.000136 * T * T * T - 0.00001149 * T * T * T * T) / 3600) * pi / 180;
    F = (93.27209062 + (1739527262.8478 * T - 12.7512 * T * T - 0.001037 * T * T * T + 0.00000417 * T * T * T * T) / 3600) * pi / 180;
    D = (297.85019547 + (1602961601.209 * T - 6.3706 * T * T + 0.006593 * T * T * T - 0.00003169 * T * T * T * T) / 3600) * pi / 180;
    Omega = (125.04455501 + (-6962890.2665 * T + 7.4722 * T * T + 0.007702 * T * T * T - 0.00005939 * T * T * T * T) / 3600) * pi / 180;

    double epsilon = (84381.448 - 46.8150 * T - 0.00059 * T * T + 0.001813 * T * T * T) / 3600 * pi / 180;

    double delta_psi = 0;
    double delta_epsilon = 0;

    int j;
    double beta;
    for (j = 0; j < 106; j++)
    {
        beta = a1[j] * l + a2[j] * ll + a3[j] * F + a4[j] * D + a5[j] * Omega;

        delta_psi = delta_psi + (A1[j] + A2[j] * T) * sin(beta);
        delta_epsilon = delta_epsilon + (B1[j] + B2[j] * T) * cos(beta);
    }
    delta_psi = delta_psi / 3600 * pi / 180;
    delta_epsilon = delta_epsilon / 3600 * pi / 180;


    double** lr1, ** lr2, ** lr3;
    lr1 = R1(-epsilon - delta_epsilon);
    //lr2 = R3(-delta_psi);
    //lr3 = R1(epsilon);

    double mat1[3][3] = { 0.0 }, mat2[3][3] = { 0.0 }, mat3[3][3] = { 0.0 };
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            mat1[i][j] = lr1[i][j];
        }
    }

    lr2 = R3(-delta_psi);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            mat2[i][j] =lr2[i][j];
        }
    }

    lr3 = R1(epsilon);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            mat3[i][j] = lr3[i][j];
        }
    }

    double** lmat12;
    lmat12 = matmulti(mat1, mat2);
    double mat12[3][3];
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            mat12[i][j] = lmat12[i][j];
        }
    }

    double** lmat123;
    lmat123 = matmulti(mat12, mat3);

    double** mat123 = 0;
    mat123 = new double* [3];
    for (int i = 0; i < 3; i++)
    {
        *(mat123 + i) = new double[3];
    }

    mat123[0][0] = 0; mat123[0][1] = 0; mat123[0][2] = 0;
    mat123[1][0] = 0; mat123[1][1] = 0; mat123[1][2] = 0;
    mat123[2][0] = 0; mat123[2][1] = 0; mat123[2][2] = 0;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            mat123[i][j] = lmat123[i][j];
        }
    }
    return mat123;


}

double** precession(double x)
{
    double T;
    T = (x - 2451545.0) / 36525;

    double z, theta, zeta;
    z = (2306.2181 * T + 1.09468 * T * T + 0.018203 * T * T * T) / 3600 * pi / 180;
    theta = (2004.3109 * T - 0.42665 * T * T - 0.041833 * T * T * T) / 3600 * pi / 180;
    zeta = (2306.2181 * T + 0.30188 * T * T + 0.017998 * T * T * T) / 3600 * pi / 180;

    double** lmat1, ** lmat2, ** lmat3;
    lmat1 = R3(-z);
    lmat2 = R2(theta);

    double mat1[3][3], mat2[3][3], mat3[3][3];
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            mat1[i][j] = lmat1[i][j];
            mat2[i][j] = lmat2[i][j];
        }
    }
    lmat3 = R3(-zeta);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            mat3[i][j] = lmat3[i][j];
        }
    }
    double** lmat12;
    lmat12 = matmulti(mat1, mat2);
    double mat12[3][3];
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            mat12[i][j] = lmat12[i][j];
        }
    }
    double** lmat123;
    lmat123 = matmulti(mat12, mat3);
    
    double** mat123 = 0;
    mat123 = new double* [3];
    for (int i = 0; i < 3; i++)
    {
        *(mat123 + i) = new double[3];
    }

    mat123[0][0] = 0; mat123[0][1] = 0; mat123[0][2] = 0;
    mat123[1][0] = 0; mat123[1][1] = 0; mat123[1][2] = 0;
    mat123[2][0] = 0; mat123[2][1] = 0; mat123[2][2] = 0;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            mat123[i][j] = lmat123[i][j];
        }
    }
    return mat123;


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
    mat = new double* [3];
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
        allY  = Time[0];
        allMO = Time[1];
        allD  = Time[2];
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



void OrbitDynamics(double t, double y[6], double f[3], double ydot[6])
{
    /*% 轨道动力学函数

        % 输入
        % 位置 km
        % 速度 km / s
        % 控制力， N

        % 输出
        % 速度 km / s
        % 加速度 km / s / s
    */

    // 摄动影响
    double agrav[3] = { 0 };

    // 位置 速度
    double px = y[0]; double py = y[1]; double pz = y[2];
    double vx = y[3]; double vy = y[4]; double vz = y[5];

    // 地心距
    double r = sqrt(pow(px, 2) + pow(py, 2) + pow(pz, 2));

    agrav[0] = -mu * px / pow(r, 3.0) * (3.0 / 2 * J2 * pow(Re / r, 2.0) * (1.0 - 5.0 * pow(pz / r, 2.0)));
    agrav[1] = -mu * py / pow(r, 3.0) * (3.0 / 2 * J2 * pow(Re / r, 2.0) * (1.0 - 5.0 * pow(pz / r, 2.0)));
    agrav[2] = -mu * pz / pow(r, 3.0) * (3.0 / 2 * J2 * pow(Re / r, 2.0) * (3.0 - 5.0 * pow(pz / r, 2.0)));

    ydot[0] = vx;
    ydot[1] = vy;
    ydot[2] = vz;

    ydot[3] = -mu * y[0] / pow(r, 3.0) + agrav[0];
    ydot[4] = -mu * y[1] / pow(r, 3.0) + agrav[1];
    ydot[5] = -mu * y[2] / pow(r, 3.0) + agrav[2];

}


void orb_rkf78(double x[], double h, double ti, double tf, double f_c[], double* xoutAdr, int neq)
{
    static double ch[13] = { 0 };
    static double alph[13] = { 0 };
    static double beta[13][12] = { { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };

    double dt, twrk, temp, tetol, xerr, ter, tol, tconst, ab, ba, sdt;
    double xwrk[7] = { 0 }, xdot[7] = { 0 };
    //double x[7] = { 0 };
    double f[7][13] = { { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };
    int  i, k, kk, j, l, m;


    double sign(double a);
    void OrbitDynamics(double t, double y[6], double f[3], double ydot[6]);

    tetol = 1e-14;

    int rkcoef = 1;
    //double time, start, finish;
    if (rkcoef == 1)
    {
        /*
        // allocate arrays
        ch = zeros(13, 1);
        alph = zeros(13, 1);
        beta = zeros(13, 12);
        */

        // define integration coefficients
        ch[5] = 34. / 105;
        ch[6] = 9. / 35;
        ch[7] = ch[6];
        ch[8] = 9. / 280;
        ch[9] = ch[8];
        ch[11] = 41. / 840;
        ch[12] = ch[11];

        alph[1] = 2. / 27;
        alph[2] = 1. / 9;
        alph[3] = 1. / 6;
        alph[4] = 5. / 12;
        alph[5] = 0.5;
        alph[6] = 5. / 6;
        alph[7] = 1. / 6;
        alph[8] = 2. / 3;
        alph[9] = 1. / 3;
        alph[10] = 1.;
        alph[12] = 1.;

        beta[1][0] = 2. / 27;
        beta[2][0] = 1. / 36;
        beta[3][0] = 1. / 24;
        beta[4][0] = 5. / 12;
        beta[5][0] = 0.05;
        beta[6][0] = -25. / 108;
        beta[7][0] = 31. / 300;
        beta[8][0] = 2.;
        beta[9][0] = -91. / 108;
        beta[10][0] = 2383. / 4100;
        beta[11][0] = 3. / 205;
        beta[12][0] = -1777. / 4100;
        beta[2][1] = 1. / 12;
        beta[3][2] = 1. / 8;
        beta[4][2] = -25. / 16;
        beta[4][3] = -beta[4][2];
        beta[5][3] = 0.25;
        beta[6][3] = 125. / 108;
        beta[8][3] = -53. / 6;
        beta[9][3] = 23. / 108;
        beta[10][3] = -341. / 164;
        beta[12][3] = beta[10][3];
        beta[5][4] = 0.2;
        beta[6][4] = -65. / 27;
        beta[7][4] = 61. / 225;
        beta[8][4] = 704. / 45;
        beta[9][4] = -976. / 135;
        beta[10][4] = 4496. / 1025;
        beta[12][4] = beta[10][4];
        beta[6][5] = 125. / 54;
        beta[7][5] = -2. / 9;
        beta[8][5] = -107. / 9;
        beta[9][5] = 311. / 54;
        beta[10][5] = -301. / 82;
        beta[11][5] = -6. / 41;
        beta[12][5] = -289. / 82;
        beta[7][6] = 13. / 900;
        beta[8][6] = 67. / 90;
        beta[9][6] = -19. / 60;
        beta[10][6] = 2133. / 4100;
        beta[11][6] = -3. / 205;
        beta[12][6] = 2193. / 4100;
        beta[8][7] = 3.;
        beta[9][7] = 17. / 6;
        beta[10][7] = 45. / 82;
        beta[11][7] = -3. / 41;
        beta[12][7] = 51. / 82;
        beta[9][8] = -1. / 12;
        beta[10][8] = 45. / 164;
        beta[11][8] = 3. / 41;
        beta[12][8] = 33. / 164;
        beta[10][9] = 18. / 41;
        beta[11][9] = 6. / 41;
        beta[12][9] = 12. / 41;
        beta[12][11] = 1.;

        // set initialization indicator
        rkcoef = 0;
    }

    //compute integration "direction"
    sdt = sign(tf - ti);
    dt = fabs(h) * sdt;

    /*
    for (m = 0; m<neq; m++)
    x[m] = xin[m];
    */
    //dt = h;
    //start = clock();
    long steps = 0;
    while (1)
    {
        steps++;
        // load "working" time and integration vector
        twrk = ti;
        //xwrk = x;
        for (i = 0; i < neq; i++)
            xwrk[i] = x[i];
        // check for last dt
        if (fabs(dt) > fabs(tf - ti))
            dt = tf - ti;
        if (fabs(ti - tf) < 0.00000001)
        {
            for (i = 0; i < neq; i++)
                *(xoutAdr + i) = x[i];
            //finish = clock();
            //time = (double)(finish - start) / CLOCKS_PER_SEC;

            //printf("%ld times\n", steps);
            return;
        }
        // evaluate equations of motion
        //xdot = feval(deq, ti, x);
        OrbitDynamics(ti, x, f_c, xdot);
        // 	ppropJ2(x, xdot);
        for (i = 0; i < neq; i++)
            f[i][0] = xdot[i];

        // compute solution
        for (k = 2; k <= 13; k++)
        {
            kk = k - 1;
            for (i = 0; i < neq; i++)
            {
                temp = 0;
                for (j = 0; j < kk; j++)
                    temp = temp + beta[kk][j] * f[i][j];
                x[i] = xwrk[i] + dt * temp;
            }
            ti = twrk + alph[kk] * dt;
            //xdot = feval(deq, ti, x);
// 			ppropJ2(x, xdot);
            OrbitDynamics(ti, x, f_c, xdot);
            for (j = 0; j < neq; j++)
                f[j][kk] = xdot[j];
        }
        for (i = 0; i < neq; i++)
        {
            temp = 0;
            for (l = 0; l < 13; l++)
                temp = temp + ch[l] * f[i][l];
            x[i] = xwrk[i] + dt * temp;
        }

        // truncation error calculations
        xerr = tetol;
        for (i = 0; i < neq; i++)
        {
            ter = fabs((f[i][0] + f[i][10] - f[i][11] - f[i][12]) * ch[11] * dt);
            tol = fabs(x[i]) * tetol + tetol;
            tconst = ter / tol;
            if (tconst > xerr)
                xerr = tconst;
        }

        // compute new step size
        //dt = 0.8 * dt * (1 / xerr) ^ (1 / 8);
        ab = 1.0 / xerr;
        ba = 1.0 / 8;
        dt = 0.8 * dt * pow(ab, ba);
        if (xerr > 1)
        {
            // reject current step
            ti = twrk;
            for (i = 0; i < neq; i++)
                x[i] = xwrk[i];
        }

    }


}