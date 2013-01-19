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
#include <tarte/Exception.h>
#include <tarte/Logger.h>

#include "../../chisa/tk/Universe.h"
#include "../../nes/Hexe.h"
#include "../../nes/machine/exception/EmulatorException.h"
#include "SDLPlatformFairy.h"

using namespace std;

namespace chisa {
namespace entrypoint {
namespace pc {

Logger log(std::cout, Logger::TRACE_);
Handler<Chisa> gChisa;

int main(int argc, char** argv) {
	int returnCode=0;
	Handler<SDLPlatformFairy> platform(new SDLPlatformFairy(log));
	Handler<Hexe> hexe(new nes::Hexe(log, "./universe"));
	gChisa = Handler<Chisa>(new Chisa(log, platform, hexe));
	{
		try {
			gChisa->init("Chisa", 1280,720, 8, 8, 8, 8, 16, 0);
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_STENCIL_TEST);

			//glEnable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			gChisa->start("test");
			//XXX ::donut::Exceptionが隠れてしまってる
		} catch ( ::tarte::Exception& e) {
			std::cerr << "Exception caught at " << e.file() << ":" << e.line() << std::endl;
			std::cerr << "<msg>" << e.msg() << std::endl;
			returnCode = -2;
		} catch (nes::EmulatorException& e) {
			std::cerr << "Emulator Exception: \"" << e.getMessage() << "\"" << std::endl;
			returnCode = -3;
		} catch (std::exception& e) {
			std::cerr << "STL Exception: \"" << e.what() << "\"" << std::endl;
			returnCode = -4;
		} catch (...) {
			std::cerr << "caught unknwon error." << std::endl;
			returnCode = -5;
		}
		std::cout << "exit successfully." << std::endl << std::flush;
	}
	gChisa.reset();
	hexe.reset();
	platform.reset();
	return returnCode;
}

}}}


int main(int argc, char** argv) {
	return chisa::entrypoint::pc::main(argc, argv);
}

