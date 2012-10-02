//============================================================================
// Name        : Chisa.cpp
// Author      : PSI
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <GL/glut.h>
using namespace std;

namespace chisa {
namespace entrypoint {
namespace pc {

void fillBlack()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSolidTeapot(1/2.0);
	glutSwapBuffers();

}

void idle()
{
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Chisa");

	glutDisplayFunc(fillBlack);
	glutIdleFunc(idle);

	glutMainLoop();
	return EXIT_SUCCESS;
}

}}}


int main(int argc, char** argv) {
	return chisa::entrypoint::pc::main(argc, argv);
}

