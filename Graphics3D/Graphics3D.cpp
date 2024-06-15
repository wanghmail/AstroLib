// Graphics3D.cpp : 定义控制台应用程序的入口点。
//
#include "Graphics3D.h"


using namespace std;


string appPath;
GLuint texture_id[2];
static const float green[] = { 1,1,1,1 };
float m_rotX = 0, m_rotY = 0, m_scale = 1.0f;
int angle_earth = 0;
bool bTrack = true;
bool b2D = false;
float angle_satellite = 0;
double scale = 1.0;
GLfloat aspect = 1.0;
vector<Vec3> track_line;

//经纬度转换(弧度)
BLH XYZtoLB(double X, double Y, double Z)
{
	BLH res = { 0 };
	double B = 0.0, N = 0.0, H = 0.0, R0, R1, deltaH, deltaB;
	R0 = sqrt(pow(X, 2) + pow(Y, 2));
	R1 = sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
	//经度直接求解
	res.L = atan2(Y, X);
	//迭代求大地维度和大地高
	N = aa;
	H = R1 - aa;
	B = atan2(Z * (N + H), R0 * (N * (1 - e2) + H));
	do
	{
		deltaH = N;//判断收敛所用
		deltaB = B;
		N = aa / sqrt(1 - e2 * pow(sin(B), 2));
		H = R0 / cos(B) - N;
		B = atan2(Z * (N + H), R0 * (N * (1 - e2) + H));
	} while (fabs(deltaH - H) > 1.0e-3 && fabs(deltaB - B) > 1.0e-9);
	res.B = B;
	res.H = H;
	return res;
}

void lookat(int type);

int LoadTexture(const char* filename, int width, int height, int depth, GLenum color_type, GLenum filter_type)
{
	GLubyte* raw_bitmap;
	FILE* file;
	errno_t err = fopen_s(&file, filename, "rb");
	if (err != 0)
	{
		return 1;
	}

	raw_bitmap = (GLubyte*)malloc(width*height*depth*sizeof(GLubyte));
	if (raw_bitmap == NULL)
	{
		fclose(file);
		return 2;
	}

	fread(raw_bitmap, width*height*depth, 1, file);
	fclose(file);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_type);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, color_type, GL_UNSIGNED_BYTE, raw_bitmap);
	//    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, color_type, GL_UNSIGNED_BYTE, raw_bitmap);
	free(raw_bitmap);

	return 0;
}

void LoadTextures(GLuint* texture_id, int MaxNrofTextures)
{
	string name_earth = "2k_earth_daymap.bmp";
	glGenTextures(MaxNrofTextures, &texture_id[0]);
	glBindTexture(GL_TEXTURE_2D, 1);
	if (LoadTexture(name_earth.c_str(), 2048, 1024, 3, GL_BGR_EXT, GL_LINEAR))
	{
		cout << "load texture error";
		return;
	}
}

void drawEarth(float r, float angle)
{
	glBindTexture(GL_TEXTURE_2D, 1);
	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	GLUquadricObj* sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricTexture(sphere, GL_TRUE);
	gluSphere(sphere, r, 1000, 1000);

	glPopMatrix();
}

Vec3 calSatellitePosition(float r)
{
	float gama = PI / 6;
	double matR[3][3]{ 1, 0, 0,
	0, std::cos(gama), std::sin(gama),
	0, -std::sin(gama), std::cos(gama) };
	
	float x = r*std::sin(angle_satellite);
	float y = r*std::cos(angle_satellite);
	float z = 0;

	Vec3 p(x, y, z);
	Vec3 pos(0, 0, 0);
	pos.x = r*std::sin(angle_satellite);
	pos.y = std::cos(gama)* r*std::cos(angle_satellite);
	pos.z = -std::sin(gama) * r*std::cos(angle_satellite);

	if (bTrack)
		track_line.push_back(Vec3(pos.x, pos.y, pos.z));


	return pos;
}

void drawSatellite(Vec3 _p,bool bUseModel)
{
	glPushMatrix();

	glTranslated(_p.x, _p.y, _p.z);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glScaled(1000.0f, 1000.0f, 1000.0f);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	//Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);   glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);   glVertex3f( 1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);   glVertex3f( 1.0f,  1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);   glVertex3f(-1.0f,  1.0f, 1.0f);
	//Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);   glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);   glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);   glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);   glVertex3f(1.0f, -1.0f, -1.0f);
	//Top Face
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);   glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);   glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);   glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);   glVertex3f(1.0f, 1.0f, -1.0f);
	//Bottom Face
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);   glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);   glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);   glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);   glVertex3f(-1.0f, -1.0f, 1.0f);
	//Right Face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);   glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);   glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);   glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);   glVertex3f(1.0f, -1.0f, 1.0f);
	//Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);   glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);   glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f);   glVertex3f(-1.0f, 1.0f,   1.0f);
	glTexCoord2f(0.0f, 1.0f);   glVertex3f(-1.0f, 1.0f,  -1.0f);
	glEnd();



	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	//glDisable(GL_DEPTH_TEST);
}

void drawTrackLine(float lineWidth)
{
	glLineWidth(lineWidth);
	glPushMatrix();
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < track_line.size(); i++)
	{
		glVertex3f(track_line[i].x, track_line[i].y, track_line[i].z);
		glColor3f(1, 1, 1);
	}
	glEnd();
	glPopMatrix();
}

void drawEarthMap()
{
	glBindTexture(GL_TEXTURE_2D, 1);
	GLfloat w = 2048;
	GLfloat h = 1024;
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex2f(-w / 2, -h / 2);
	glTexCoord2d(0, 1);
	glVertex2f(-w / 2, h / 2);
	glTexCoord2d(1, 1);
	glVertex2f(w / 2, h / 2);
	glTexCoord2d(1, 0);
	glVertex2f(w / 2, -h / 2);
	glEnd();
}

void drawTrackLineOnMap()
{
	glPointSize(2.0);
	glPushMatrix();
	glBegin(GL_POINTS);
	GLdouble x, y;
	for (int i = 0; i < track_line.size(); i++)
	{
		BLH blh = XYZtoLB(track_line[i].x, track_line[i].y, track_line[i].z);

		//弧度转经纬度
		double b = blh.B * 90 / PI;
		double l = blh.L * 90 / PI;

		//经纬度转屏幕坐标
		x = (l)* 2048 / 180.0;
		y = (b)* 1024 / 180.0;

		glColor3f(1, 1, 0);
		glVertex2f(x, y);
		glColor3f(1, 1, 1);
	}
	glEnd();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(x, y, 0);
	
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 0.0f, 0.0f);
	int w = 20,h = 20;
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex2f(-w / 2, -h / 2);
	glTexCoord2d(0, 1);
	glVertex2f(-w / 2, h / 2);
	glTexCoord2d(1, 1);
	glVertex2f(w / 2, h / 2);
	glTexCoord2d(1, 0);
	glVertex2f(w / 2, -h / 2);

	glEnd();
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void display()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓存区和深度缓冲区

	angle_earth -= 1;
	angle_satellite -= 0.001;
	float earthR = 6.378137e4;
	float r = 1.1*earthR;

	glPushMatrix();

	auto p = calSatellitePosition(r);

	if (b2D) {
		drawEarthMap();
		if (bTrack)
			drawTrackLineOnMap();// draw track lines
	}
	else
	{
		glRotatef(m_rotX, 1, 0, 0);
		glRotatef(m_rotY, 0, 1, 0);
		glScalef(m_scale, m_scale, m_scale);

		drawEarth(earthR, angle_earth);//draw earth
		drawSatellite(p,false);// load satellite
		if (bTrack)
			drawTrackLine();// draw track lines
	}
	glPopMatrix();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	//    qDebug() << key;
	switch (key) {
	case 'w':
		m_rotX += 1.0;
		break;
	case 's':
		m_rotX -= 1.0;
		break;

	case 'a':
		m_rotY += 1.0;
		break;
	case 'd':
		m_rotY -= 1.0;
		break;

	case 'r':
		m_rotY = 0.0;
		m_rotX = 0.0;
		break;

	case 32:
		bTrack ^= 1;
		track_line.clear();
		break;
	case '1':
		lookat(1);
		break;
	case '2':
		lookat(2);
		break;
	case '3':
		lookat(3);
		break;
	case '4':
		lookat(4);
		break;
	default:
		break;
	}
}

void InitWindow(int argc, char *argv[], int width, int height, const char* title)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);//指定窗口大小
	glutInitWindowPosition(100, 100);//指定窗口左上角位置
	glutCreateWindow(title);//创建openGL渲染窗口

	glClearColor(0, 0, 0, 1);//设置清除颜色
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	LoadTextures(texture_id, 3);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void lookat(int type)
{
	switch (type) {
	case 1:
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, -1.5e5, 0, 0, 0, 0, 1, 1);
		break;
	case 2:
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 2e5, 0, 0, 0, 0, 0, 0, 1);
		break;
	case 3:
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, -3e7, 0, 0, 0, 0, 0, 0, 1);
		break;
	case 4:
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(-1.5e5, 0, 0, 0, 0, 0, 0, 0, 1);
		break;
	default:
		break;
	}
}

void changeView()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	if (b2D)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-1024, 1024, -512, 512);
	}
	else
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, aspect, 0.1, 3e5);
		lookat(2);
	}
}

void reshape(int w, int h)
{
	aspect = (GLfloat)w / (GLfloat)h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	if (b2D)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-1024, 1024, -512, 512);
	}
	else
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, aspect, 0.1, 3e5);
		lookat(2);
	}
}

void specialKeys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_HOME:
		b2D ^= 1;
		changeView();
		break;
	default:
		break;
	}
}

//鼠标事件
void MouseEvent(int button, int state, int x, int y)
{
	//1.鼠标缩放：往上滚轮放大，往下滚轮缩小
	if (state == GLUT_UP)
	{

	}
}
//鼠标移动事件
void Motion(int button, int state, int x, int y)
{
	//1.鼠标缩放：往上滚轮放大，往下滚轮缩小
	if (state == GLUT_UP)
	{

	}
}
void IdleEvent()
{
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	appPath = argv[0];
	appPath = appPath.substr(0, appPath.find_last_of("\\"))+"\\source\\";

	InitWindow(argc, argv, 1920, 1080, "earth");
	glutDisplayFunc(display);//添加显示回调函数
	glutIdleFunc(IdleEvent);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(MouseEvent);
	glutMouseFunc(Motion);
	glutSpecialFunc(specialKeys);
	glutMainLoop();//启动程序

	return 1;
}
