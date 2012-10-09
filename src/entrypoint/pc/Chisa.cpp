//============================================================================
// Name        : Chisa.cpp
// Author      : PSI
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <GL/glfw.h>
#include "../../chisa/tk/Universe.h"
#include "../../chisa/logging/Exception.h"

using namespace std;

namespace chisa {
namespace entrypoint {
namespace pc {

logging::Logger log(std::cout, logging::Logger::TRACE_);
shared_ptr<tk::Universe> gUniverse;

void tempInit()
{
	std::shared_ptr<tk::Universe>(tk::Universe::create(log, "./universe")).swap(gUniverse);
	//暫定で接続コードとかおいてみる
	gUniverse->createNewWorld("test");
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gUniverse->render();
}

void reshape(int width, int height)
{
	gUniverse->reshape(tk::Area(0,0,width, height));
}

int main(int argc, char** argv) {
	if(glfwInit() == GL_FALSE){
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	try {
		if(GL_FALSE == glfwOpenWindow(640,480, 8, 8, 8, 8, 8, 8, GLFW_WINDOW)){
			std::cerr << "Failed to open window." << std::endl;
			return -1;
		}

		glfwSetWindowTitle("Chisa");
		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_TEXTURE_2D);

		tempInit();
		glfwSetWindowSizeCallback(reshape);
		//glfwSetWindowRefreshCallback(render);
		bool running=true;
		float last = glfwGetTime();
		float nextFrame = last+(1.0/60);
		while(running){
			gUniverse->idle(1000.0/60);
			float now = glfwGetTime();
			if(now < nextFrame){
				render();
				glfwSwapBuffers();
				float now = glfwGetTime();
				if(now < nextFrame){
					glfwSleep(nextFrame-now);
				}
			}
			nextFrame+=(1.0/60);
			running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
		}

	} catch (logging::Exception& e) {
		std::cerr << "Exception caught at " << e.file() << ":" << e.line() << std::endl;
		std::cerr << "<msg>" << e.msg() << std::endl;
		glfwTerminate();
		return -2;
	} catch (...) {
		std::cerr << "caught unknwon error." << std::endl;
		glfwTerminate();
		return -3;
	}
	glfwTerminate();
	return 0;
}

}}}


int main(int argc, char** argv) {
	return chisa::entrypoint::pc::main(argc, argv);
}

