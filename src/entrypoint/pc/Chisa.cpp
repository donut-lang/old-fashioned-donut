//============================================================================
// Name        : Chisa.cpp
// Author      : PSI
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <GL/glut.h>
#include "../../chisa/tk/Universe.h"
using namespace std;

namespace chisa {
namespace entrypoint {
namespace pc {

tk::Universe* gUniverse;

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glutSolidTeapot(1/2.0);
//	glutSwapBuffers();
	gUniverse->render();
	glutSwapBuffers();
}

void idle(const int __)
{
	gUniverse->idle(16);
	glutPostRedisplay();
}

void reshape(int width, int height)
{
	gUniverse->reshape(tk::Box(width, height));
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Chisa");

	logging::Logger log(std::cout, logging::Logger::TRACE_);
	gUniverse = new tk::Universe(log);
	glutDisplayFunc(render);
	glutTimerFunc(16, idle, 0);
	glutReshapeFunc(reshape);

	glutMainLoop();
	return EXIT_SUCCESS;
}

}}}


int main(int argc, char** argv) {
	return chisa::entrypoint::pc::main(argc, argv);
}

