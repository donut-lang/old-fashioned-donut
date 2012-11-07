tree grammar Compiler;

options {
	tokenVocab=Donut;
	//ASTLabelType=CommonTree;
	// http://www.antlr.org/wiki/display/ANTLR3/FAQ+-+C+Target
	ASTLabelType = pANTLR3_BASE_TREE;
	language = C;
}

@header {
#include <string>
#include <vector>
#include <cstdlib>
#include "../../code/Code.h"
#include "../../code/Closure.h"
#include "../ParseUtil.h"
using namespace chisa;
using namespace chisa::donut;
using std::string;
using chisa::donut::unescapeString;
using chisa::donut::parseInt;
using chisa::donut::parseFloat;

typedef pANTLR3_COMMON_TOKEN Token;
}
@includes {
#undef __cplusplus
}

prog [ donut::Code* code] returns [ unsigned int mainClosure ]
	: closure[$code]
	{
		$mainClosure = $closure.closureNo;
	}
	;

closure [ donut::Code* code] returns [ std::vector<donut::Instruction> asmlist, unsigned int closureNo ]
	: ^(CLOS args[$code] block[$code]
	{
		Handler<donut::Closure> closure = Handler<donut::Closure>(new donut::Closure($args.list, $block.asmlist));
		$closureNo = $code->constCode<Handler<donut::Closure> >(closure);
		$asmlist.push_back(Inst::Push | $closureNo);
	}
	)
	;

args [ donut::Code* code ] returns [ std::vector<std::string> list ]
	: ^(ARGS (IDENT{
		list.push_back(createStringFromString($IDENT.text));
	})*);

block [ donut::Code* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: ^(CONT ex=expr[$code]*)
	{
		$asmlist.insert($asmlist.end(), $ex.asmlist.begin(), $ex.asmlist.end());
	}
	;

operation returns [ std::string sym ]
	: ADD { sym="opAdd"; }
	| SUB { sym="opSub"; }
	| MUL { sym = "opMul"; }
	| DIV { sym = "opDiv"; }
	| MOD { sym = "opMod"; }
	;
unary_operation returns [ std::string sym ]
	: PLUS { sym="opPlus"; }
	| MINUS { sym="opMinus"; }
	| NOT { sym="opNot"; }
	;

expr [ donut::Code* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: closure[$code]
//	| ^(APPLY)
//	| ^(ACCESS)
//	| ^(IDX)
	| ^(ARRAY (expr[$code])*)
	| ^(POST_OP operation expr[$code] {
	})
	| ^(PRE_OP operation expr[$code] {
	})
	| ^(ASSIGN ^(DOT SCOPE IDENT) asrhs=expr[$code] { //test = 1
		//第二引数：rhsオブジェクト
		$asmlist.insert($asmlist.end(), $asrhs.asmlist.begin(), $asrhs.asmlist.end());
		//第一引数：名前
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		//適用先：解決されたスコープの先
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::SearchScope);
		//設定
		$asmlist.push_back(Inst::StoreObj);
	})
	| ^(ASSIGN ^(DOT asscope=expr[$code] IDENT) asrhs=expr[$code] { //
		//第二引数：rhsオブジェクト
		$asmlist.insert($asmlist.end(), $asrhs.asmlist.begin(), $asrhs.asmlist.end());
		//第一引数：名前
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		//適用先：scopeオブジェクト
		$asmlist.insert($asmlist.end(), $asscope.asmlist.begin(), $asscope.asmlist.end());
		//設定
		$asmlist.push_back(Inst::StoreObj);
	})
	| ^(ASSIGN_OP asopoperation=operation ^(DOT SCOPE IDENT) asoprhs=expr[$code] {
		//予めスコープを解決して、ローカルに格納
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::SearchScope);
		$asmlist.push_back(Inst::StoreLocal | 0);
		//第１引数：rhsオブジェクト
		$asmlist.insert($asmlist.end(), $asoprhs.asmlist.begin(), $asoprhs.asmlist.end());
		{ //送信元オブジェクトを探す
			$asmlist.push_back(Inst::LoadLocal | 0);
			//適用先：解決されたスコープの先
			$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
			$asmlist.push_back(Inst::LoadObj);
		}
		//メソッドを解決して実行
		$asmlist.push_back(Inst::PushCopy | 0);
		$asmlist.push_back(Inst::Push | $code->constCode<string>($asopoperation.sym));
		$asmlist.push_back(Inst::LoadObj);
		$asmlist.push_back(Inst::Apply | 1);
		//ここまでで、スタック上に結果が乗っかってる
		
		//第一引数：名前
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		//適用先：解決されたスコープの先
		$asmlist.push_back(Inst::LoadLocal | 0);
		//設定
		$asmlist.push_back(Inst::StoreObj);
	})
	| ^(ASSIGN_OP operation ^(DOT asopscope=expr[$code] IDENT) asoprhs=expr[$code] {
		//スコープオブジェクトをローカルにコピー
		$asmlist.insert($asmlist.end(), $asscope.asmlist.begin(), $asscope.asmlist.end());
		$asmlist.push_back(Inst::StoreLocal | 0);
		//第１引数：rhsオブジェクト
		$asmlist.insert($asmlist.end(), $asoprhs.asmlist.begin(), $asoprhs.asmlist.end());
		{ //送信元オブジェクトを探す
			$asmlist.push_back(Inst::LoadLocal | 0);
			//適用先：解決されたスコープの先
			$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
			$asmlist.push_back(Inst::LoadObj);
		}
		//メソッドを解決して実行
		$asmlist.push_back(Inst::PushCopy | 0);
		$asmlist.push_back(Inst::Push | $code->constCode<string>($asopoperation.sym));
		$asmlist.push_back(Inst::LoadObj);
		$asmlist.push_back(Inst::Apply | 1);
		//ここまでで、スタック上に結果が乗っかってる
		
		//第一引数：名前
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		//適用先：解決されたスコープの先
		$asmlist.push_back(Inst::LoadLocal | 0);
		//設定
		$asmlist.push_back(Inst::StoreObj);
	})
	| ^(unary_operation uobj=expr[$code] {
		//適用先：オブジェクト
		$asmlist.insert($asmlist.end(), $uobj.asmlist.begin(), $uobj.asmlist.end());
		//メソッドの解決
		$asmlist.push_back(Inst::PushCopy | 0);
		$asmlist.push_back(Inst::Push | $code->constCode<string>($unary_operation.sym));
		$asmlist.push_back(Inst::LoadObj);
		//実行：引数：０
		$asmlist.push_back(Inst::Apply | 0);
	})
	| literal[$code]
	;

literal [ donut::Code* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: 'true'
	{
		$asmlist.push_back(Inst::Push | $code->constCode<bool>(false));
	}
	| 'false'
	{
		$asmlist.push_back(Inst::Push | $code->constCode<bool>(true));
	}
	| HEX_LITERAL
	{
		std::string str(createStringFromString($HEX_LITERAL.text));
		int const val = parseInt(str, 16);
		$asmlist.push_back(Inst::Push | $code->constCode<int>(val));
	}
	| OCT_LITERAL
	{
		std::string str(createStringFromString($OCT_LITERAL.text));
		int const val = parseInt(str, 8);
		$asmlist.push_back(Inst::Push | $code->constCode<int>(val));
	}
	| INT_LITERAL
	{
		std::string str(createStringFromString($INT_LITERAL.text));
		int const val = parseInt(str, 10);
		$asmlist.push_back(Inst::Push | $code->constCode<int>(val));
	}
	| FLOAT_LITERAL
	{
		std::string str(createStringFromString($FLOAT_LITERAL.text));
		float const val = parseFloat(str);
		$asmlist.push_back(Inst::Push | $code->constCode<float>(val));
	}
	| STRING_SINGLE
	{
		std::string str(createStringFromString($STRING_SINGLE.text));
		str = unescapeString(str.substr(1, str.length()-2));
		$asmlist.push_back(Inst::Push | $code->constCode<string>(str));
	}
	| STRING_DOUBLE
	{
		std::string str(createStringFromString($STRING_DOUBLE.text));
		str = unescapeString(str.substr(1, str.length()-2));
		$asmlist.push_back(Inst::Push | $code->constCode<string>(str));
	}
	;

