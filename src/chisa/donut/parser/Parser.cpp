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

#include "Parser.h"
#include "../../logging/Exception.h"
#include <iostream>
#include <sstream>
#include <string>

namespace chisa {
namespace donut {
	class Closure;
}}
#include "output/DonutLexer.h"
#include "output/DonutParser.h"
#include "output/Compiler.h"
#include "ParseUtil.h"

namespace chisa {
namespace donut {

class ParserImpl {
private:
	pANTLR3_INPUT_STREAM stream;
	std::string src;
	std::string filename_;
	pDonutLexer lexer;
	pANTLR3_COMMON_TOKEN_STREAM tokenStream;
	pDonutParser parser;
private:
	void setup()
	{
		this->lexer = DonutLexerNew(this->stream);
		if(!this->lexer){
			throw logging::Exception(__FILE__, __LINE__, "Failed to read stream for %s", filename_.c_str());
		}
		this->tokenStream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
		if(!this->tokenStream){
			throw logging::Exception(__FILE__, __LINE__, "Failed to create token stream for %s", filename_.c_str());
		}
		this->parser = DonutParserNew(tokenStream);
		if(!this->parser){
			throw logging::Exception(__FILE__, __LINE__, "Failed to create parser for %s", filename_.c_str());
		}
	}
public:
	std::string filename() { return this->filename_; };
	explicit ParserImpl()
	:stream(nullptr), src(), filename_(), lexer(nullptr), tokenStream(nullptr), parser(nullptr){}
	virtual ~ParserImpl() noexcept
	{
		if(stream){
			stream->free(stream);
			stream = nullptr;
		}
		if(lexer){
			lexer->free(lexer);
			lexer = nullptr;
		}
		if(tokenStream){
			tokenStream->free(tokenStream);
			tokenStream = nullptr;
		}
		if(parser){
			parser->free(parser);
			parser = nullptr;
		}
	}

	ParserImpl& fromFile(std::string const& filename)
	{
		this->filename_ = filename;
		stream = antlr3FileStreamNew((ANTLR3_UINT8*)filename.c_str(), ANTLR3_ENC_UTF8);
		if(!stream){
			throw logging::Exception(__FILE__, __LINE__, "Failed to create ANTLR3 File Stream for %s", filename.c_str());
		}
		setup();
		return *this;
	}
	ParserImpl& fromString(std::string const& src, std::string const& filename, int line)
	{
		this->src = src;
		this->filename_ = filename;
		stream = antlr3StringStreamNew((pANTLR3_UINT8)this->src.c_str(), ANTLR3_ENC_UTF8, src.size(), (pANTLR3_UINT8)this->filename_.c_str());
		if(!stream){
			throw logging::Exception(__FILE__, __LINE__, "Failed to create ANTLR3 String Stream for %s", filename.c_str());
		}
		stream->line=line;
		setup();
		return *this;
	}
	ParserImpl& fromStream(std::istream& stream_, const std::string filename){
		std::stringstream ss;
		char buff[8192];
		while(!stream_.eof()){
			stream_.read(buff, sizeof(buff));
			if ( stream_.fail() && !stream_.eof() ) {
				break;
			}
			ss.write(buff, stream_.gcount());
		}
		std::string src = ss.str();
		return fromString(src, filename, 0);
	}
public:
	pANTLR3_BASE_TREE parseProgram()
	{
		pANTLR3_BASE_TREE t = parser->source(parser).tree;

		// エラー自体は既に表示されているので、こちらでは例外を投げるだけ
		if(parser->pParser->rec->state->errorCount > 0)
		{
			//ANTLR3_EXCEPTION* ex = parser->pParser->rec->state->exception;
			throw logging::Exception(__FILE__, __LINE__, "Failed to parse.");
		}
		return t;
	}
};

class CompilerImpl {
private:
	const std::string filename;
	pANTLR3_BASE_TREE tree;
	pANTLR3_COMMON_TREE_NODE_STREAM stream;
	pCompiler compiler;
public:
	CompilerImpl(std::string const& filename, pANTLR3_BASE_TREE tree)
	:filename(filename), tree(tree), stream(nullptr), compiler(nullptr){
		stream = antlr3CommonTreeNodeStreamNewTree(tree, 0);
		compiler = CompilerNew(stream);
	}
	~CompilerImpl() noexcept {
		if(this->compiler){
			this->compiler->free(this->compiler);
			this->compiler = nullptr;
		}
		if(this->stream){
			stream->free(this->stream);
			this->stream = nullptr;
		}
		if(this->tree && tree->free){
			tree->free(tree);
			this->tree = nullptr;
		}
	}
	Handler<donut::Source> compile(){
		Handler<donut::Source> src = compiler->prog(compiler);
		// エラー自体は既に表示されているので、こちらでは例外を投げるだけ
		if(compiler->pTreeParser->rec->state->errorCount > 0)
		{
			//ANTLR3_EXCEPTION* ex = compiler->pTreeParser->rec->state->exception;
			throw logging::Exception(__FILE__, __LINE__, "Failed to compile.");
		}
		return src;
	}
};

Parser::Parser(ParserImpl* pimpl)
:parserImpl(nullptr), compilerImpl(nullptr)
{
	if(!pimpl){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Invalid parser impl pointer.");
	}
	this->parserImpl = pimpl;
	pANTLR3_BASE_TREE tree = pimpl->parseProgram();
//	std::string treeStr(createStringFromString(tree->toStringTree(tree)));
//	std::cout << treeStr << std::endl;
	this->compilerImpl = new CompilerImpl(pimpl->filename(), tree);
}
Parser::~Parser()
{
	delete this->compilerImpl;
	delete this->parserImpl;
}
std::shared_ptr<Parser> Parser::fromFile(std::string const& filename)
{
	ParserImpl* pimpl = new ParserImpl();
	pimpl->fromFile(filename);
	std::shared_ptr<Parser> parser(new Parser(pimpl));
	return parser;
}
std::shared_ptr<Parser> Parser::fromString(std::string const& src, std::string const& filename, int line)
{
	ParserImpl* impl = new ParserImpl();
	impl->fromString(src, filename, line);
	std::shared_ptr<Parser> parser(new Parser(impl));
	return parser;
}
std::shared_ptr<Parser> Parser::fromStream(std::istream& stream_, std::string const& filename)
{
	ParserImpl* impl = new ParserImpl();
	impl->fromStream(stream_, filename);
	std::shared_ptr<Parser> parser(new Parser(impl));
	return parser;
}

Handler<donut::Source> Parser::parseProgram()
{
	return compilerImpl->compile();
}

}}
