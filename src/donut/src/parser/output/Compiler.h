/** \file
 *  This C header file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : d:/Dropbox/src/chisa/src/donut/src/parser\\Compiler.g
 *     -                            On : 2013-06-07 19:41:01
 *     -           for the tree parser : CompilerTreeParser
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The tree parser
Compiler

has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 *
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pCompiler, which is returned from a call to CompilerNew().
 *
 * The methods in pCompiler are  as follows:
 *
 *  -
 Handler<donut::Source>
      pCompiler->prog(pCompiler)
 *  -
 Compiler_closure_return
      pCompiler->closure(pCompiler)
 *  -
 std::vector<std::string>
      pCompiler->vars(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->defvar(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->block(pCompiler)
 *  -
 std::string
      pCompiler->operation(pCompiler)
 *  -
 std::string
      pCompiler->unary_operation(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->expr(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->preop(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->postop(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->assignop(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->assign(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->accessor(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->apply(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->cond(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->loop(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->literal(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->return_(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->interrupt(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->object(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->object_pair(pCompiler)
 *  -
 std::vector<donut::Instruction>
      pCompiler->array(pCompiler)
 *  -
 Compiler_arglist_return
      pCompiler->arglist(pCompiler)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD license"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_Compiler_H
#define _Compiler_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */

#undef __cplusplus


#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct Compiler_Ctx_struct Compiler, * pCompiler;



#include <string>
#include <vector>
#include <cstdlib>
#include <cstddef>
#include <algorithm>
#include <cinamo/String.h>
#include <donut/source/Source.hpp>
#include <donut/source/Closure.hpp>
#include "../ParseUtil.hpp"

using namespace cinamo;
using namespace donut;
using std::string;
using std::nullptr_t;
using donut::unescapeString;
using cinamo::parseAs;

typedef pANTLR3_COMMON_TOKEN Token;


#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif
typedef struct Compiler_closure_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    std::vector<donut::Instruction> asmlist;
    unsigned int closureNo;
}
    Compiler_closure_return;



typedef struct Compiler_arglist_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    std::vector<donut::Instruction> asmlist;
    int count;
}
    Compiler_arglist_return;




/** Context tracking structure for
Compiler

 */
struct Compiler_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_TREE_PARSER	    pTreeParser;


     Handler<donut::Source>
     (*prog)	(struct Compiler_Ctx_struct * ctx);

     Compiler_closure_return
     (*closure)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<std::string>
     (*vars)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*defvar)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*block)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::string
     (*operation)	(struct Compiler_Ctx_struct * ctx);

     std::string
     (*unary_operation)	(struct Compiler_Ctx_struct * ctx);

     std::vector<donut::Instruction>
     (*expr)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*preop)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*postop)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*assignop)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*assign)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*accessor)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*apply)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*cond)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*loop)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*literal)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*return_)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*interrupt)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*object)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*object_pair)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     std::vector<donut::Instruction>
     (*array)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);

     Compiler_arglist_return
     (*arglist)	(struct Compiler_Ctx_struct * ctx, donut::Source* code);
    // Delegated rules

    const char * (*getGrammarFileName)();
    void            (*reset)  (struct Compiler_Ctx_struct * ctx);
    void	    (*free)   (struct Compiler_Ctx_struct * ctx);
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pCompiler CompilerNew         (
pANTLR3_COMMON_TREE_NODE_STREAM
 instream);
ANTLR3_API pCompiler CompilerNewSSD      (
pANTLR3_COMMON_TREE_NODE_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the
tree parser
 will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif
#define EOF      -1
#define T__64      64
#define T__65      65
#define T__66      66
#define T__67      67
#define T__68      68
#define T__69      69
#define T__70      70
#define T__71      71
#define T__72      72
#define T__73      73
#define T__74      74
#define T__75      75
#define T__76      76
#define T__77      77
#define T__78      78
#define T__79      79
#define T__80      80
#define T__81      81
#define T__82      82
#define T__83      83
#define T__84      84
#define T__85      85
#define T__86      86
#define T__87      87
#define T__88      88
#define T__89      89
#define T__90      90
#define T__91      91
#define T__92      92
#define T__93      93
#define T__94      94
#define T__95      95
#define T__96      96
#define T__97      97
#define T__98      98
#define T__99      99
#define T__100      100
#define T__101      101
#define T__102      102
#define ADD      4
#define AND      5
#define APPLY      6
#define ARGS      7
#define ARRAY      8
#define ASSIGN      9
#define ASSIGN_OP      10
#define BIT_AND      11
#define BIT_OR      12
#define CEQ      13
#define CGE      14
#define CGT      15
#define CLE      16
#define CLOS      17
#define CLT      18
#define CNE      19
#define COMMENT      20
#define COND      21
#define CONT      22
#define DIGIT      23
#define DIV      24
#define DOT      25
#define ESC_SEQ      26
#define EXPONENT      27
#define FALSE_LITERAL      28
#define FLOAT_LITERAL      29
#define FOR      30
#define FUNC      31
#define HEX_DIGIT      32
#define HEX_LITERAL      33
#define IDENT      34
#define INTERRUPT      35
#define INT_LITERAL      36
#define LETTER      37
#define MINUS      38
#define MOD      39
#define MUL      40
#define NONZERO_DIGIT      41
#define NOT      42
#define NULL_LITERAL      43
#define OBJECT      44
#define OCT_DIGIT      45
#define OCT_LITERAL      46
#define OR      47
#define PAIR      48
#define PLUS      49
#define POST_OP      50
#define PRE_OP      51
#define RETURN      52
#define SCOPE      53
#define SELF      54
#define STRING_DOUBLE      55
#define STRING_DOUBLE_ELEMENT      56
#define STRING_SINGLE      57
#define STRING_SINGLE_ELEMENT      58
#define SUB      59
#define TRUE_LITERAL      60
#define VAR      61
#define VARS      62
#define WS      63
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for Compiler
 * =============================================================================
 */
/** } */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
