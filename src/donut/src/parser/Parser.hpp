/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <memory>
#include <cinamo/Handler.h>
#include <donut/source/Source.hpp>

namespace donut {
using namespace cinamo;

class ParserImpl;
class CompilerImpl;
class Parser
{
private:
	ParserImpl* parserImpl;
	CompilerImpl* compilerImpl;
	explicit Parser(ParserImpl* impl);
public:
	~Parser();
	static std::shared_ptr<Parser> fromFile(std::string const& filename);
	static std::shared_ptr<Parser> fromString(std::string const& src, std::string const& filename="<ON MEMORY>", int line=0);
	static std::shared_ptr<Parser> fromStream(std::istream& stream_, std::string const& filename);
	Handler<donut::Source> parseProgram();
};

}
