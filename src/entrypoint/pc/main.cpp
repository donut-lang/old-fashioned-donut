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

#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <libgen.h>
#include <cinamo/Exception.h>
#include <cinamo/Logger.h>

#include "../../chisa/tk/Universe.h"
#include "../../nes/Hexe.h"
#include "../../nes/machine/exception/EmulatorException.h"
#include "SDLPlatformFairy.h"

using namespace std;

namespace chisa {
namespace entrypoint {
namespace pc {

static const std::string TAG("ChisaEntry");
void usage(int argc, char* argv[]){
	static const std::string USAGE_TAB="    ";
	std::cout << "Usage: " << basename(argv[0]) << " [switches] [--] [main universe name] [arguments]" << std::endl;
	std::cout << USAGE_TAB << std::left << std::setw(15) << "--trace" << "set log level." << std::endl;
	std::cout << USAGE_TAB << std::left << std::setw(15) << "--verbose" << "set log level." << std::endl;
	std::cout << USAGE_TAB << std::left << std::setw(15) << "--debug"<<"set log level." << std::endl;
	std::cout << USAGE_TAB << std::left << std::setw(15) << "--warning"<<"set log level." << std::endl;
	std::cout << USAGE_TAB << std::left << std::setw(15) << "--info"<<"set log level." << std::endl;
	std::cout << USAGE_TAB << std::left << std::setw(15) << "--error"<<"set log level." << std::endl;
	std::cout << USAGE_TAB << std::left << std::setw(15) << "--version"<<"output the version, then exit." << std::endl;
	std::cout << USAGE_TAB << std::left << std::setw(15) << "-h, --help"<<"output the help, then exit." << std::endl;
	exit(0);
}

void version(int argc, char* argv[]){
	std::cout << "Chisa - a GUI toolkit for time leapers!" << ": " <<" (build at " << __DATE__ << " " << __TIME__ << ")" << std::endl;
	exit(0);
}

const struct option ARG_OPTIONS[] = {
		{"trace", no_argument, 0, 1},
		{"verbose", no_argument, 0, 2},
		{"debug", no_argument, 0, 3},
		{"warning", no_argument, 0, 4},
		{"info", no_argument, 0, 5},
		{"error", no_argument, 0, 6},
		{"help", no_argument, 0, 7},
		{"version", no_argument, 0, 8},
		{0,0,0,0}
};

int runChisa(Logger& log, int argc, char** argv){
	int returnCode=0;
	Handler<SDLPlatformFairy> platform(new SDLPlatformFairy(log));
#if CHISA_ANDROID
	std::string basepath("/sdcard");//SDL_AndroidGetExternalStoragePath());
#else
	std::string basepath(".");
#endif
	Handler<Hexe> hexe(new nes::Hexe(log, basepath+"/universe"));
	Handler<Chisa> chisa (new Chisa(log, platform, hexe));
	{
		try {
			//XXX: 成果報告会が終わったら戻す
#if CHISA_ANDROID
			chisa->init("Chisa", 1280,720,false, 8, 8, 8, 8, 16, 0);
#else
			chisa->init("Chisa", 1024,768,false, 8, 8, 8, 8, 16, 0);
#endif
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_STENCIL_TEST);

			//glEnable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			chisa->start(argc > 0 ? argv[0] : "main");
		} catch ( ::cinamo::Exception& e) {
			std::cerr << "Exception caught at " << e.file() << ":" << e.line() << std::endl;
			std::cerr << "<msg>" << e.msg() << std::endl;
			returnCode = -1;
		} catch ( ::donut::DonutException& e) {
			std::cerr << "Donut Exception caught at " << e.file() << ":" << e.line() << std::endl;
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
	return returnCode;
}

int main(int argc, char** argv) {
	int indexptr=0;
	Logger::Level level = Logger::Level::WARN_;
	while(1){
		int opt = getopt_long(argc, argv, "h", ARG_OPTIONS, &indexptr);
		if(opt < 0){
			break;
		}
		switch(opt)
		{
		case 1:
			level = Logger::TRACE_;
			break;
		case 2:
			level = Logger::VERBOSE_;
			break;
		case 3:
			level = Logger::DEBUG_;
			break;
		case 4:
			level = Logger::INFO_;
			break;
		case 5:
			level = Logger::WARN_;
			break;
		case 6:
			level = Logger::ERROR_;
			break;
		case 7:
		case 'h':
			usage(argc, argv);
			break;
		case 8:
			version(argc, argv);
			break;
		case '?':
			exit(0);
			break;
		}
	}

	Logger log(std::cout, level);
	log.t(TAG, "Logger created. Level: %d", level);
	return runChisa(log, argc-optind, &argv[optind]);
}

}}}


int main(int argc, char** argv) {
	return chisa::entrypoint::pc::main(argc, argv);
}

