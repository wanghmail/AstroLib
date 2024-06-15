// Graphics3D.cpp : 定义控制台应用程序的入口点。
//

#include <vector>
#include <iostream>
#include "glut.h"


#define aa  6.378137e4
#define f  (1 / 298.257223563)	//扁率
#define e2  (f*(2-f))			//第一偏心率平方
#define PI 3.1415926

using namespace std;

//经纬度函数传参
typedef struct BLH
{
	double B;//维度
	double L;//经度
	double H;//高
}BLH;

typedef struct Vec3
{
	Vec3(double x1, double y1,double z1)
	{
		x = x1;
		y = y1;
		z = z1;
	}
	double x, y, z;
}Vec3;


//经纬度转换(弧度)
BLH XYZtoLB(double X, double Y, double Z);

void lookat(int type);

int LoadTexture(const char* filename, int width, int height, int depth, GLenum color_type, GLenum filter_type);

void LoadTextures(GLuint* texture_id, int MaxNrofTextures);

void drawEarth(float r, float angle);

Vec3 calSatellitePosition(float r);

void drawSatellite(Vec3 _p, bool bUseModel);

void drawTrackLine(float lineWidth = 1.0f);

void drawEarthMap();

void drawTrackLineOnMap();

void display();

void keyboard(unsigned char key, int x, int y);

void InitWindow(int argc, char *argv[], int width, int height, const char* title);

void changeView();

void reshape(int w, int h);

void specialKeys(int key, int x, int y);


//鼠标事件
void MouseEvent(int button, int state, int x, int y);
//鼠标移动事件
void Motion(int button, int state, int x, int y);

void IdleEvent();

