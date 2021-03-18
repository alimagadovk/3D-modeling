//#include <GL/glew.h> // ���� ���� ������ ���� ������ � ������ !
//#include <cstdio>
//#include <iostream>
//#include <ctime>
//#include <math.h>
//#include <SOIL2/SOIL2.h>
//#include <glut.h>
//
//GLdouble eye_x = 7.0, eye_y = 5.0, eye_z = 7.0;
//GLdouble rot_x = 0.0, rot_y = 0.0, rot_z = 0.0;
//
//GLdouble light_x = 5.0, light_y = 0.0, light_z = 4.0;
//
//int			mouseOldX = 0;
//int			mouseOldY = 0;
//
///////////////////////////////////////////////////////////////////////////////////
//
//float matDif[] = { 0.8f, 0.8f, 0.8f };
//float matSpec[] = { 0.4f, 0.4f, 0.4f };
//float matEm[] = { 0.0f, 0.5f, 0.5f };
//float matAmb[] = { 0.2f, 0.2f, 0.2f };
//float lightPos[] = { light_x, light_y, light_z };
////GLfloat lightSpec[] = { 0.5f, 0.5f, 0.5f };
////GLfloat lightDif[] = { 0.5f, 0.5f, 0.5f };
////GLfloat lightAmb[] = { 0.5, 0.5, 0.5, 1.0 };
//
//
//GLuint shaderProgram;
//
//
//// ������� ��������� ��������� ���� � ��������� ����������
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
//	// �������� ������ ������ �������
//	glEnable(GL_DEPTH_TEST);
//
//	// ��������� �������� ��� ���������� ������� �� ������
//	char* vertexShaderCode = readTextFile("E:\\���������\\��\\3D �������������\\������������\\laba1\\lvs.vs");
//
//	// ������ ������������� ������ ���������� �������
//	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//
//	// ��������� ������������� ������� � ��� �������� ���
//	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
//
//	// ����������� ������
//	glCompileShader(vertexShader);
//
//	// ��������� ���������� ���������� ���� �������
//	GLint success;
//	GLchar infoLog[512];
//
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//	if (!success) {
//		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//	}
//
//	// ��������� �������� ��� ������������ ������� �� ������
//	char* fragmentShaderCode = readTextFile("E:\\���������\\��\\3D �������������\\������������\\laba1\\lfs.fs");
//
//	// ������ ������������� ������ ���������� �������
//	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//
//	// ��������� ������������� ������� � ��� �������� ���
//	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
//
//	// ����������� ������
//	glCompileShader(fragmentShader);
//
//	// ��������� ���������� ���������� ���� �������
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//
//	if (!success) {
//		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//	}
//
//	// ������ "���������": ������������� ����� ��������
//	shaderProgram = glCreateProgram();
//
//	// ��������� � ��������� �������
//	glAttachShader(shaderProgram, vertexShader);
//	glAttachShader(shaderProgram, fragmentShader);
//
//	// ������ (��������) �������� � "���������"
//	glLinkProgram(shaderProgram);
//
//	// ��������� ���������� ������ ���������
//	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//	if (!success) {
//		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//	}
//
//	// ������� �������� ������ �������������� ��������
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//}
//
//unsigned int start_time = clock();
//
//void display() {
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	int diff = clock() - start_time;
//
//	// �������� ������
//	glUseProgram(shaderProgram);
//
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glRotatef(rot_x, 1, 0, 0);
//	glRotatef(rot_y, 0, 1, 0);
//	glRotatef(rot_z, 0, 0, 1);
//
//
//	//glColor3f(0.0, 0.0, 1.0);
//	//glutSolidCube(1.0);
//	//glColor3f(1.0, 0.0, 0.0);
//	glutSolidTeapot(3);
//	//glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
//	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
//	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
//
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
//	//glutSolidCube(4);
//	glColor3f(1.0, 0.0, 0.0);
//
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
//	gluLookAt(eye_x, eye_y, eye_z,	// eye
//		0, 0, 0,				// center
//		0.0, 0.0, 1.0);		// up
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
//
//	if (key == 27 || key == 'q' || key == 'Q') // �����
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
//		/* Problem: glewInit failed, something is seriously wrong. */
//		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
//		getchar();
//		//exit(1);
//	}
//	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
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
