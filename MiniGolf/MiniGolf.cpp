#include "GameManager.h"
#include "Shader.h"
#include "Camera.h"

using namespace glm;

int window_width = 512;
int window_height = 512;

GameManager *manager;
bool keyState[256] = { false };
bool specialState[256] = { false };

void keyboard() //perform action based on keystates
{
	for (int i = 0; i < 256; i++)
	{
		if (keyState[i])
			switch (i) {
			case 'w':
				manager->get_camera()->change_view(rotate(-0.5f, vec3(1.0, 0.0, 0.0)));
				break;
			case 's':
				manager->get_camera()->change_view(rotate(0.5f, vec3(1.0, 0.0, 0.0)));
				break;
			case 'a':
				manager->get_camera()->change_view(rotate(0.5f, vec3(0.0, 0.0, 1.0)));
				break;
			case 'd':
				manager->get_camera()->change_view(rotate(-0.5f, vec3(0.0, 0.0, 1.0)));
				break;
			case 'x':
				manager->get_camera()->change_view(rotate(0.5f, vec3(0.0, 1.0, 0.0)));
				break;
			case 'z':
				manager->get_camera()->change_view(rotate(-0.5f, vec3(0.0, 1.0, 0.0)));
				break;
			case 'c':
				manager->get_camera()->change_view(translate(vec3(0.0, 0.2, 0.0)));
				break;
			case ' ':
				manager->get_camera()->change_view(translate(vec3(0.0, -0.2, 0.0)));
				break;
			case 'h':
				manager->get_current_level()->get_ball()->set_x(0.1f);
				break;
			case 'u':
				manager->get_current_level()->get_ball()->set_y(0.1f);
				break;
			case 'j':
				manager->get_current_level()->get_ball()->set_z(0.1f);
				break;
			case 'l':
				manager->get_current_level()->get_ball()->set_x(-0.1f);
				break;
			case 'b':
				manager->get_current_level()->get_ball()->set_y(-0.1f);
				break;
			case 'n':
				manager->get_current_level()->get_ball()->set_z(-0.1f);
				break;
			case '1':
				break;
			case 27:
				exit(0);
				break;
			default:
				break;
		}
	}

	for (int i = 0; i < 256; i++)
	{
		if (specialState[i])
			switch (i) {
			case GLUT_KEY_UP:
				manager->get_camera()->change_view(translate(vec3(0.0, 0.0, -0.2)));
				break;
			case GLUT_KEY_DOWN:
				manager->get_camera()->change_view(translate(vec3(0.0, 0.0, 0.2)));
				break;
			case GLUT_KEY_LEFT:
				manager->get_camera()->change_view(translate(vec3(-0.2, 0.0, 0.0)));
				break;
			case GLUT_KEY_RIGHT:
				manager->get_camera()->change_view(translate(vec3(0.2, 0.0, 0.0)));
				break;
			default:
				break;
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	keyboard();

	manager->tick();

	manager->draw();

	glutSwapBuffers();
}

void reshape(int w, int h) 
{
	manager->resize(w, h);
}

void print_opengl_info()
{
	cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl << endl;
}

void init_gl()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_MULTISAMPLE);

	glFrontFace(GL_CCW);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void idle(){
	display();
}

void keyboard_up(unsigned char c, int x, int y){
	keyState[c] = false;
}

void keyboard_down(unsigned char c, int x, int y){
	keyState[c] = true;
}

void special(int key, int x, int y){
	specialState[key] = true;
}

void specialUp(int key, int x, int y){
	specialState[key] = false;
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Mini Golf");

	glewInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard_down);
	glutIdleFunc(idle);
	glutKeyboardUpFunc(keyboard_up);
	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);

	print_opengl_info();
	init_gl();

	manager = new GameManager(argc, argv);

	glutMainLoop();
	
	return 0;
}