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
	for (int i = 0; i < 256; i++) {
		if (keyState[i]) {
			vec3 p;
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
			case 'v':
				manager->get_camera()->change_view(translate(vec3(0.0, -0.2, 0.0)));
				break;
			case ' ':
				manager->get_current_level()->get_ball()->add_force();
				break;
			case 't':
				manager->get_current_level()->get_ball()->set_x(0.1f);
				break;
			case 'y':
				manager->get_current_level()->get_ball()->set_x(-0.1f);
				break;
			case 'o':
				manager->get_current_level()->get_ball()->set_z(0.1f);
				break;
			case 'p':
				manager->get_current_level()->get_ball()->set_z(-0.1f);
				break;
			case 'q':
				p = manager->get_current_level()->get_ball()->get_position();
				manager->get_camera()->set_view(lookAt(vec3(p.x - 0.7, p.y + 0.6, p.z + 0.7), p, vec3(0.0f, 1.0f, 0.0f)));
				break;
			case 27:
				exit(0);
				break;
			default:
				break;
			}
		}
	}

	for (int i = 0; i < 256; i++) {
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

	manager->update();

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
}

void idle(){
	double time_now = manager->get_timer().get_elapsed_time();

	if ((time_now - manager->get_current_time()) > 0.01666666666f) { // If we exceed the period time (60 FPS).
		manager->set_current_time(time_now); // Set new time.
		manager->update(); // Update.
	}
	glutPostRedisplay();
}

void keyboard_up(unsigned char c, int x, int y) {
	keyState[c] = false;
}

void keyboard_down(unsigned char c, int x, int y) {
	keyState[c] = true;
}

void special(int key, int x, int y) {
	specialState[key] = true;
}

void specialUp(int key, int x, int y) {
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