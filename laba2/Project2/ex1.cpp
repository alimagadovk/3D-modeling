//#define _CRT_SECURE_NO_WARNINGS
//#include <GL/glew.h> // этот файл должен идти первым в списке !
//#include <iostream>
//#include <math.h>
//#include <SOIL2/SOIL2.h>
//#include <glut.h>
//
//
//
//GLdouble eye_x = 7.0, eye_y = 5.0, eye_z = 7.0;
//GLdouble rot_x = 0.0, rot_y = 0.0, rot_z = 0.0;
//
//GLdouble light_x = 0.0, light_y = 0.0, light_z = 15.0;
//float light[3] = { (float)light_x, (float)light_y, (float)light_z };
//
//int  mouseOldX = 0;
//int  mouseOldY = 0;
//
//float matDif[] = { 0.8f, 0.8f, 0.8f };
//float matSpec[] = { 0.4f, 0.4f, 0.4f };
//float matEm[] = { 0.5f, 0.5f, 0.5f };
//float matAmb[] = { 0.2f, 0.2f, 0.2f };
//
//
//GLuint shaderProgram;
//
//char *readTextFile(char *fn) {
//	FILE *fp;
//	char *content = NULL;
//	int count = 0;
//	if (fn != NULL) {
//		errno_t error = fopen_s(&fp, fn, "rt");
//		if (error == 0) {
//			fseek(fp, 0, SEEK_END);
//			count = ftell(fp);
//			rewind(fp);
//			if (count > 0) {
//				content = (char *)malloc(sizeof(char) * (count + 1));
//				count = fread(content, sizeof(char), count, fp);
//				content[count] = '\0';
//			}
//			fclose(fp);
//		}
//	}
//	return content;
//}
//
//
//void init() {
//	glEnable(GL_DEPTH_TEST);
//
//	char* vertexShaderCode = readTextFile((char*)"E:\\Документы\\ДЗ\\3D моделирование\\Лабораторные\\laba2\\1.vs");
//
//	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//
//	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
//
//	glCompileShader(vertexShader);
//
//	GLint success;
//	GLchar infoLog[512];
//
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//	if (!success) {
//		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//	}
//
//
//	char* fragmentShaderCode = readTextFile((char*)"E:\\Документы\\ДЗ\\3D моделирование\\Лабораторные\\laba2\\1.fs");
//
//	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//
//	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
//
//	glCompileShader(fragmentShader);
//
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//
//	if (!success) {
//		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//	}
//
//	shaderProgram = glCreateProgram();
//
//	glAttachShader(shaderProgram, vertexShader);
//	glAttachShader(shaderProgram, fragmentShader);
//
//	glLinkProgram(shaderProgram);
//
//	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//	if (!success) {
//		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//	}
//
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//
//	GLuint texId;
//	glGenTextures(1, &texId);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texId);
//
//	int width, height;
//
//	unsigned char *image = SOIL_load_image("E:\\Документы\\ДЗ\\3D моделирование\\Лабораторные\\laba2\\01.jpg", &width, &height, NULL, SOIL_LOAD_RGB);
//	double Sz, Tz, h1, h2, h3, h4, a = 0.8;
//	int index[5];
//	for (int x = 0; x < width; x++) {
//		for (int y = 0; y < height; y++) {
//			index[0] = (x + y * width) * 3;
//			index[1] = (x == width - 1) ? index[0] : (x + 1 + y * width) * 3;
//			index[2] = (x == 0) ? index[0] : (x - 1 + y * width) * 3;
//			index[3] = (y == height - 1) ? index[0] : (x + (y + 1) * width) * 3;
//			index[4] = (y == 0) ? index[0] : (x + (y - 1) * width) * 3;
//			h1 = image[index[1]] / 255.0;
//			h2 = image[index[2]] / 255.0;
//			h3 = image[index[3]] / 255.0;
//			h4 = image[index[4]] / 255.0;
//
//			Sz = a * (h1 - h2);
//			Tz = a * (h3 - h4);
//			double k = 1 / sqrt(Sz*Sz + Tz*Tz + 1.0);
//			double N[3] = { -k * Sz, -k * Tz, k };
//			image[index[0]] = (int)((N[0] + 1.0) / 2.0 * 255.0);
//			image[index[0] + 1] = (int)((N[1] + 1.0) / 2.0 * 255.0);
//			image[index[0] + 2] = (int)((N[2] + 1.0) / 2.0 * 255.0);
//		}
//	}
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//
//	SOIL_free_image_data(image);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//}
//
//
//void display() {
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//	float view[3] = { (float)eye_x, (float)eye_y, (float)eye_z };
//
//
//	glUseProgram(shaderProgram);
//
//
//
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glRotatef(rot_x, 1, 0, 0);
//	glRotatef(rot_y, 0, 1, 0);
//	glRotatef(rot_z, 0, 0, 1);
//
//	glScalef(5.0, 5.0, 5.0);
//
//	glBegin(GL_QUADS);
//
//	glTexCoord2f(0.0, 0.0);
//	glVertex3f(-1.0, -1.0, 0.0);
//
//	glTexCoord2f(0.0, 1.0);
//	glVertex3f(1.0, -1.0, 0.0);
//
//	glTexCoord2f(1.0, 1.0);
//	glVertex3f(1.0, 1.0, 0.0);
//
//	glTexCoord2f(1.0, 0.0);
//	glVertex3f(-1.0, 1.0, 0.0);
//
//	glEnd();
//
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
//	glMaterialfv(GL_FRONT, GL_EMISSION, matEm);
//	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
//
//	glPopMatrix();
//	glutSwapBuffers();
//}
//
//void redraw() {
//	glutPostRedisplay();
//}
//
//void reshape(int w, int h)
//{
//	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 60.0);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	gluLookAt(eye_x, eye_y, eye_z, 0, 0, 0, 0.0, 0.0, 1.0);
//}
//
//void motion(int x, int y) {
//	rot_y -= ((mouseOldY - y) * 180.0f) / 200.0f;
//	rot_z -= ((mouseOldX - x) * 180.0f) / 200.0f;
//	rot_x = 0;
//
//	if (rot_z > 360)
//		rot_z -= 360;
//
//	if (rot_z < -360)
//		rot_z += 360;
//
//	if (rot_y > 360)
//		rot_y -= 360;
//
//	if (rot_y < -360)
//		rot_y += 360;
//
//	mouseOldX = x;
//	mouseOldY = y;
//
//	glutPostRedisplay();
//}
//
//void mouse(int button, int state, int x, int y) {
//	if (state == GLUT_DOWN) {
//		mouseOldX = x;
//		mouseOldY = y;
//	}
//}
//
//void key(unsigned char key, int x, int y) {
//	if (key == 27)
//		exit(0);
//}
//
//int main(int argc, char** argv) {
//	glutInit(&argc, argv);
//
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//	glutInitWindowSize(500, 500);
//
//	glutCreateWindow("GLEW Test");
//	GLenum err = glewInit();
//	if (GLEW_OK != err)
//	{
//		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
//		getchar();
//	}
//	std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
//
//	glutDisplayFunc(display);
//	glutReshapeFunc(reshape);
//	glutKeyboardFunc(key);
//	glutMouseFunc(mouse);
//	glutMotionFunc(motion);
//	glutIdleFunc(redraw);
//	init();
//	glutMainLoop();
//	return 0;
//}