#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h> // этот файл должен идти первым в списке !
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <glut.h>
#include <SOIL2\SOIL2.h>
#include <future>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace std::chrono;
using namespace glm;

class Liquid
{
private:
	long width;
	long height;
	vec3* tangent;
	float k1, k2, k3;

public:
	vec3* buf[2];
	long renderBuffer;
	vec3* normal;
	
	Liquid(long n, long m, float d, float t, float c, float mu) : width(n), height(m)
	{
		long count = n * m;
		buf[0] = new vec3[count];
		buf[1] = new vec3[count];
		renderBuffer = 0;
		normal = new vec3[count];
		tangent = new vec3[count];

		float f1 = c * t / d;
		f1 *= f1;
		float f2 = 1.0f / (mu * t + 2);
		k1 = (4.0f - 8.0f * f1) * f2;
		k2 = (mu * t - 2) * f2;
		k3 = 2.0f * f1 * f2;

		long a = 0;
		for (long j = 0; j < m; j++)
		{
			float y = d * j;
			for (long i = 0; i < n; i++)
			{
				buf[0][a] = { d * i, y, 0.0f };
				buf[1][a] = buf[0][a];
				normal[a] = { 0.0f, 0.0f, 2.0f * d };
				tangent[a] = { 2.0f * d, 0.0f, 0.0f };
				a++;
			}
		}
	}
	
	~Liquid()
	{
		delete[] tangent;
		delete[] normal;
		delete[] buf[1];
		delete[] buf[0];
	}

	void Evaluate()
	{
		for (long j = 1; j < height - 1; j++)
		{
			const vec3 * crnt = buf[renderBuffer] + j * width;
			vec3 * prev = buf[1 - renderBuffer] + j * width;
			for (long i = 1; i < width - 1; i++)
			{
				prev[i].z = k1 * crnt[i].z + k2 * prev[i].z + k3 * (crnt[i + 1].z + crnt[i - 1].z + crnt[i + width].z + crnt[i - width].z);
			}
		}
		renderBuffer = 1 - renderBuffer;
		for (long j = 1; j < height - 1; j++)
		{
			const vec3 * next = buf[renderBuffer] + j * width;
			vec3 * nrml = normal + j * width;
			vec3 * tang = tangent + j * width;
			for (long i = 1; i < width - 1; i++)
			{
				nrml[i].x = next[i - 1].z - next[i + 1].z;
				nrml[i].y = next[i - width].z - next[i + width].z;
				tang[i].z = next[i + 1].z - next[i - 1].z;
			}
		}
	}

};

GLfloat eye_x = 7.0, eye_y = 5.0, eye_z = 7.0;
GLfloat rot_x = 0.0, rot_y = 0.0, rot_z = 0.0;
int	mouseOldX = 0;
int	mouseOldY = 0;
GLuint shaderProgram;


int grid = 170;
const float dv = 1. / (grid - 1);
float dt = 1.0f / 60.0f;
Liquid water(grid, grid, dv, dt, 0.25, 0.15);

char *readTextFile(const char *fn) {
	FILE *fp;
	char *content = NULL;
	int count = 0;
	if (fn != NULL) {
		errno_t error = fopen_s(&fp, fn, "rt");
		if (error == 0) {
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
		else printf("File not found\n");
	}
	return content;
}

std::future<void> fut;

void init() {

	glEnable(GL_DEPTH_TEST);

	const char* vertexShaderCode = readTextFile("F://Документы//Git//Portfolio//3D моделирование//laba4//Project2//lvs.vs");

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);

	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	const char* fragmentShaderCode = readTextFile("F://Документы//Git//Portfolio//3D моделирование//laba4//Project2//lfs.fs");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	vec3* buf = water.buf[water.renderBuffer];
	float x, y;

	float x0 = 0.5;
	float y0 = 0.5;

	for (int i = 0; i < grid; i++) {
		for (int j = 0; j < grid; j++) {
			x = buf[grid*j + i].x - x0;
			y = buf[grid*j + i].y - y0;
			buf[grid*j + i].z = 0.1*std::exp(-1000.0f * (x*x + y*y));
			//buf[grid*j + i].z = 0.0025;
		}
	}

	fut = std::async([&]() {
		std::this_thread::sleep_for(std::chrono::duration<float>(dt));
		water.Evaluate();
	});
}



void display() {
	if (fut.wait_for(0s) == std::future_status::ready)
	{
		fut = std::async([&]() {
			std::this_thread::sleep_for(std::chrono::duration<float>(dt));
			water.Evaluate();
		});
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glRotatef(rot_x, 1, 0, 0);
	glRotatef(rot_y, 0, 1, 0);
	glRotatef(rot_z, 0, 0, 1);
	glPushMatrix();
	glScalef(5.0, 5.0, 5.0);
	

	vec3* buf = water.buf[0];
	vec3* norm = water.normal;
	for (int i = 0; i < grid - 1; i++) {
		for (int j = 0; j < grid - 1; j++) {
			glBegin(GL_TRIANGLES);

			glNormal3f(norm[grid*j + i].x, norm[grid*j + i].y, norm[grid*j + i].z);
			glVertex3f(buf[grid*j + i].x, buf[grid*j + i].y, buf[grid*j + i].z);

			glNormal3f(norm[grid*(j + 1) + i].x, norm[grid*(j + 1) + i].y, norm[grid*(j + 1) + i].z);
			glVertex3f(buf[grid*(j + 1) + i].x, buf[grid*(j + 1) + i].y, buf[grid*(j + 1) + i].z);

			glNormal3f(norm[grid*(j + 1) + i + 1].x, norm[grid*(j + 1) + i + 1].y, norm[grid*(j + 1) + i + 1].z);
			glVertex3f(buf[grid*(j + 1) + i + 1].x, buf[grid*(j + 1) + i + 1].y, buf[grid*(j + 1) + i + 1].z);




			glNormal3f(norm[grid*(j + 1) + i + 1].x, norm[grid*(j + 1) + i + 1].y, norm[grid*(j + 1) + i + 1].z);
			glVertex3f(buf[grid*(j + 1) + i + 1].x, buf[grid*(j + 1) + i + 1].y, buf[grid*(j + 1) + i + 1].z);

			glNormal3f(norm[grid*j + i + 1].x, norm[grid*j + i + 1].y, norm[grid*j + i + 1].z);
			glVertex3f(buf[grid*j + i + 1].x, buf[grid*j + i + 1].y, buf[grid*j + i + 1].z);

			glNormal3f(norm[grid*j + i].x, norm[grid*j + i].y, norm[grid*j + i].z);
			glVertex3f(buf[grid*j + i].x, buf[grid*j + i].y, buf[grid*j + i].z);
			glEnd();
		}
	}

	glPopMatrix();
	glutSwapBuffers();
}

void redraw() {
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 60.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye_x, eye_y, eye_z, 0, 0, 0, 0.0, 0.0, 1.0);
}

void motion(int x, int y) {
	rot_y -= ((mouseOldY - y) * 180.0f) / 200.0f;
	rot_z -= ((mouseOldX - x) * 180.0f) / 200.0f;
	rot_x = 0;

	if (rot_z > 360)
		rot_z -= 360;

	if (rot_z < -360)
		rot_z += 360;

	if (rot_y > 360)
		rot_y -= 360;

	if (rot_y < -360)
		rot_y += 360;

	mouseOldX = x;
	mouseOldY = y;

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		mouseOldX = x;
		mouseOldY = y;
	}
}

void key(unsigned char key, int x, int y) {
	if (key == 27 || key == 'q' || key == 'Q') // выход
	{
		exit(0);
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);

	glutCreateWindow("GLEW Test");
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		getchar();
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(redraw);
	init();
	glutMainLoop();
	return 0;
}
