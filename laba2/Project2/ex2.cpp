#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h> // этот файл должен идти первым в списке !
#include <iostream>
#include <math.h>
#include <SOIL2/SOIL2.h>
#include <glut.h>
#include <vector>

GLdouble eye_x = 7.0, eye_y = 5.0, eye_z = 7.0;
GLdouble rot_x = 0.0, rot_y = 0.0, rot_z = 0.0;

GLdouble light_x = 5.0, light_y = 50.0, light_z = 400.0;
float light[3] = { (float)light_x, (float)light_y, (float)light_z };

int  mouseOldX = 0;
int  mouseOldY = 0;

float matDif[] = { 0.8f, 0.8f, 0.8f };
float matSpec[] = { 0.4f, 0.4f, 0.4f };
float matEm[] = { 0.01f, 0.01f, 0.01f };
float matAmb[] = { 0.2f, 0.2f, 0.2f };


GLuint shaderProgram;

char *readTextFile(char *fn) {
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
	}
	return content;
}

GLuint attrlocation1, attrlocation2, attrlocation3;

void init() {
	glEnable(GL_DEPTH_TEST);

	char* vertexShaderCode = readTextFile((char*)"F:\\Git\\Документы\\Portfolio\\3D моделирование\\laba2\\2.vs");

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


	char* fragmentShaderCode = readTextFile((char*)"F:\\Git\\Документы\\Portfolio\\3D моделирование\\laba2\\2.fs");

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

	attrlocation1 = glGetAttribLocation(shaderProgram, "T");
	attrlocation2 = glGetAttribLocation(shaderProgram, "B");
	attrlocation3 = glGetAttribLocation(shaderProgram, "N");

	GLuint texId;
	glGenTextures(1, &texId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);

	int width, height;

	unsigned char *image = SOIL_load_image("F:\\Документы\\Git\\Portfolio\\3D моделирование\\laba2\\Metal2.jpg", &width, &height, NULL, SOIL_LOAD_RGB);
	double Sz, Tz, h1, h2, h3, h4, a = 4.5;
	int index[5];
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			index[0] = (x + y * width) * 3;
			index[1] = (x == width - 1) ? index[0] : (x + 1 + y * width) * 3;
			index[2] = (x == 0) ? index[0] : (x - 1 + y * width) * 3;
			index[3] = (y == height - 1) ? index[0] : (x + (y + 1) * width) * 3;
			index[4] = (y == 0) ? index[0] : (x + (y - 1) * width) * 3;
			h1 = image[index[1]] / 255.0;
			h2 = image[index[2]] / 255.0;
			h3 = image[index[3]] / 255.0;
			h4 = image[index[4]] / 255.0;

			Sz = a * (h1 - h2);
			Tz = a * (h3 - h4);
			double k = 1 / sqrt(Sz*Sz + Tz*Tz + 1.0);
			double N[3] = { -k * Sz, -k * Tz, k };
			image[index[0]] = (int)((N[0] + 1.0) / 2.0 * 255.0);
			image[index[0] + 1] = (int)((N[1] + 1.0) / 2.0 * 255.0);
			image[index[0] + 2] = (int)((N[2] + 1.0) / 2.0 * 255.0);
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062


std::vector<float> cross(std::vector<float> a, std::vector<float> b)
{
	std::vector<float> r = { a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0] };
	return r;
}


void drawSphere()
{
	float r = 1.0;
	int ni = 20;
	int nj = 20;
	float st_a = PI / (float)ni;
	float st_b = 2 * PI / (float)nj;
	float sw_x = 1.0 / (float)(0.5*ni);
	float sw_y = 1.0 / (float)(0.5*nj);
	float a, b;
	std::vector<float> vert1, vert2, vert3, vert4;
	std::vector<float> vect1, vect2;
	std::vector<float> T_vect, B_vect, N_vect, temp;
	for (int i = 0; i < ni; i++)
	{
		for (int j = 0; j < nj; j++)
		{
			a = i * st_a;
			b = j * st_b;

			vert1 = { r*std::sin(a)*std::cos(b), r*std::sin(a)*std::sin(b), r*std::cos(a) };
			vert2 = { r*std::sin(a + st_a)*std::cos(b), r*std::sin(a + st_a)*std::sin(b), r*std::cos(a + st_a) };
			vert3 = { r*std::sin(a + st_a)*std::cos(b + st_b), r*std::sin(a + st_a)*std::sin(b + st_b), r*std::cos(a + st_a) };
			vert4 = { r*std::sin(a)*std::cos(b + st_b), r*std::sin(a)*std::sin(b + st_b), r*std::cos(a) };

			T_vect = { vert1[0] - vert2[0],  vert1[1] - vert2[1],  vert1[2] - vert2[2] };
			B_vect = { vert3[0] - vert2[0],  vert3[1] - vert2[1],  vert3[2] - vert2[2] };

			N_vect = cross(T_vect, B_vect);

			glBegin(GL_QUADS);

			temp = { i*sw_x, j*sw_y };
			glTexCoord2fv(temp.data());
			glVertexAttrib3f(attrlocation1, T_vect[0], T_vect[1], T_vect[2]);
			glVertexAttrib3f(attrlocation2, B_vect[0], B_vect[1], B_vect[2]);
			glVertexAttrib3f(attrlocation3, N_vect[0], N_vect[1], N_vect[2]);
			glVertex3fv(vert1.data());

			temp = { (i + 1)*sw_x, j*sw_y };
			glTexCoord2fv(temp.data());
			glVertexAttrib3f(attrlocation1, T_vect[0], T_vect[1], T_vect[2]);
			glVertexAttrib3f(attrlocation2, B_vect[0], B_vect[1], B_vect[2]);
			glVertexAttrib3f(attrlocation3, N_vect[0], N_vect[1], N_vect[2]);
			glVertex3fv(vert2.data());

			temp = { (i + 1)*sw_x, (j + 1)*sw_y };
			glTexCoord2fv(temp.data());
			glVertexAttrib3f(attrlocation1, T_vect[0], T_vect[1], T_vect[2]);
			glVertexAttrib3f(attrlocation2, B_vect[0], B_vect[1], B_vect[2]);
			glVertexAttrib3f(attrlocation3, N_vect[0], N_vect[1], N_vect[2]);
			glVertex3fv(vert3.data());

			temp = { i*sw_x, (j + 1)*sw_y };
			glTexCoord2fv(temp.data());
			glVertexAttrib3f(attrlocation1, T_vect[0], T_vect[1], T_vect[2]);
			glVertexAttrib3f(attrlocation2, B_vect[0], B_vect[1], B_vect[2]);
			glVertexAttrib3f(attrlocation3, N_vect[0], N_vect[1], N_vect[2]);
			glVertex3fv(vert4.data());

			glEnd();

		}
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float view[3] = { (float)eye_x, (float)eye_y, (float)eye_z };

	glUseProgram(shaderProgram);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glRotatef(rot_x, 1, 0, 0);
	glRotatef(rot_y, 0, 1, 0);
	glRotatef(rot_z, 0, 0, 1);

	glScalef(5.0, 5.0, 5.0);
	drawSphere();

	glLightfv(GL_LIGHT0, GL_POSITION, light);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	//glMaterialfv(GL_FRONT, GL_EMISSION, matEm);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);

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
	if (key == 27)
		exit(0);
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
	std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

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