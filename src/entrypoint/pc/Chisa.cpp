/**
 * Chisa
 * Copyright (C) 2012 psi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <GL/glfw.h>
#include <tarte/Exception.h>
#include <tarte/Logger.h>

#include "../../chisa/tk/Universe.h"
#include "../../nes/Hexe.h"
#include "../../nes/machine/exception/EmulatorException.h"

using namespace std;

namespace chisa {
namespace entrypoint {
namespace pc {

Logger log(std::cout, Logger::TRACE_);
chisa::Handler<tk::Universe> gUniverse;

Hexe* gHexe = nullptr;

void tempInit()
{
	gHexe = new nes::Hexe(log, "./universe");
	chisa::Handler<tk::Universe>(tk::Universe::create(log, gHexe)).swap(gUniverse);
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
	gUniverse->reshape(geom::Area(0,0,width, height));
}

int gMouseX=0;
int gMouseY=0;
bool gMousePressed=false;

void mousePosition(int x,int y){
	gMouseX = x;
	gMouseY = y;
	if(gMousePressed){
		const float timeMs = glfwGetTime() * 1000;
		gUniverse->onTouchMove(timeMs, 0, geom::Vector(gMouseX, gMouseY));
	}
}

void mouseButton(int button, int action){
	if(button != GLFW_MOUSE_BUTTON_LEFT){
		return;
	}
	const float timeMs = glfwGetTime() * 1000;
	switch( action ) {
	case GLFW_PRESS:
		gUniverse->onTouchDown(timeMs, 0, geom::Vector(gMouseX, gMouseY));
		gMousePressed = true;
		break;
	case GLFW_RELEASE:
		gUniverse->onTouchUp(timeMs, 0, geom::Vector(gMouseX, gMouseY));
		gMousePressed = false;
		break;
	}
}

int main(int argc, char** argv) {
	if(GL_TRUE != glfwInit()){
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	try {
		if(GL_TRUE != glfwOpenWindow(1024,768, 8, 8, 8, 8, 16, 0, GLFW_WINDOW)){
			TARTE_EXCEPTION(Exception, "Failed to open window.");
		}

		glfwSetWindowTitle("Chisa");

		glDisable(GL_ALPHA_TEST);
		glDisable(GL_STENCIL_TEST);

		glEnable(GL_SCISSOR_TEST);
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		tempInit();
		glfwSetWindowSizeCallback( reshape );
		glfwSetMouseButtonCallback( mouseButton );
		glfwSetMousePosCallback( mousePosition );
		//glfwSetWindowRefreshCallback(render);
		bool running=true;
		float last = glfwGetTime();
		float delta = 0.0f;
		float nextFrame = last+(1.0/60);
		while(running){
			gUniverse->idle(delta*1000);
			float now = glfwGetTime();
			delta = now-last;
			last = now;
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

	} catch (tarte::Exception& e) {
		std::cerr << "Exception caught at " << e.file() << ":" << e.line() << std::endl;
		std::cerr << "<msg>" << e.msg() << std::endl;
		glfwTerminate();
		return -2;
	} catch (nes::EmulatorException& e) {
		std::cerr << "Emulator Exception:" << e.getMessage() << std::endl;
		glfwTerminate();
		return -3;
	} catch (...) {
		std::cerr << "caught unknwon error." << std::endl;
		glfwTerminate();
		return -4;
	}
	decltype(gUniverse)().swap(gUniverse);
	delete gHexe;
	glfwTerminate();
	return 0;
}

}}}


int main(int argc, char** argv) {
	return chisa::entrypoint::pc::main(argc, argv);
}

