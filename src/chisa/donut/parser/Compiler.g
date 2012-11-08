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
#include <algorithm>
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
	: ^(CLOS vars[$code] block[$code]
	{
		Handler<donut::Closure> closure = Handler<donut::Closure>(new donut::Closure($vars.list, $block.asmlist));
		$closureNo = $code->constCode<Handler<donut::Closure> >(closure);
		$asmlist.push_back(Inst::Push | $closureNo);
	}
	)
	;

vars [ donut::Code* code ] returns [ std::vector<std::string> list ]
	: ^(VARS (IDENT{
		list.push_back(createStringFromString($IDENT.text));
	})*);

block [ donut::Code* code ] returns [ std::vector<donut::Instruction> asmlist ]
@after{
	//最後のpopは削除する（＝値がひとつだけ残る）
	$asmlist.erase($asmlist.end()-1);
}
	: ^(CONT (ex=expr[$code]
	{
		$asmlist.insert($asmlist.end(), $ex.asmlist.begin(), $ex.asmlist.end());
		$asmlist.push_back(Inst::Pop | 0);
	})*)
	;

operation returns [ std::string sym ]
	: ADD { sym="opAdd"; }
	| SUB { sym="opSub"; }
	| MUL { sym = "opMul"; }
	| DIV { sym = "opDiv"; }
	| MOD { sym = "opMod"; }
	| AND { sym = "opAnd"; }
	| OR { sym = "opOr"; }
	| CLT { sym = "opLt"; }
	| CGT { sym = "opGt"; }
	| CLE { sym = "opLe"; }
	| CGE { sym = "opGe"; }
	| CEQ { sym = "opEq"; }
	| CNE { sym = "opNe"; }
	;

unary_operation returns [ std::string sym ]
	: PLUS { sym="opPlus"; }
	| MINUS { sym="opMinus"; }
	| NOT { sym="opNot"; }
	;

expr [ donut::Code* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: lt=literal[$code] { $asmlist.swap($lt.asmlist); }
	| app=apply[$code] { $asmlist.swap($app.asmlist); }
	| idx=index[$code] { $asmlist.swap($idx.asmlist); }
	| ^(POST_OP postop=operation ^(DOT SCOPE IDENT) {
		//設定先を取得
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::SearchScope);
		$asmlist.push_back(Inst::StoreLocal | 0);
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::LoadLocal | 0);
			//演算オブジェクト
			$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
			$asmlist.push_back(Inst::LoadObj);
			$asmlist.push_back(Inst::StoreLocal | 0);
			//操作を実行
			$asmlist.push_back(Inst::PushCopy | 0);
			$asmlist.push_back(Inst::Push | $code->constCode<string>($postop.sym));
			$asmlist.push_back(Inst::LoadObj);
			$asmlist.push_back(Inst::Push | $code->constCode<int>(1));
			$asmlist.push_back(Inst::Apply | 1);
		//設定
		$asmlist.push_back(Inst::StoreObj);
		$asmlist.push_back(Inst::Pop | 0);
		//退避してたのを戻す
		$asmlist.push_back(Inst::LoadLocal | 0);
	})
	| ^(POST_OP postop=operation ^(DOT postexpr=expr[$code] IDENT) {
		//設定先を取得
		$asmlist.insert($asmlist.end(), $postexpr.asmlist.begin(), $postexpr.asmlist.end());
		$asmlist.push_back(Inst::StoreLocal | 0);
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::LoadLocal | 0);
			//演算オブジェクト
			$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
			$asmlist.push_back(Inst::LoadObj);
			$asmlist.push_back(Inst::StoreLocal | 0);
			//操作を実行
			$asmlist.push_back(Inst::PushCopy | 0);
			$asmlist.push_back(Inst::Push | $code->constCode<string>($postop.sym));
			$asmlist.push_back(Inst::LoadObj);
			$asmlist.push_back(Inst::Push | $code->constCode<int>(1));
			$asmlist.push_back(Inst::Apply | 1);
		//設定
		$asmlist.push_back(Inst::StoreObj);
		$asmlist.push_back(Inst::Pop | 0);
		//退避してたのを戻す
		$asmlist.push_back(Inst::LoadLocal | 0);
	})
	| ^(PRE_OP preop=operation ^(DOT SCOPE IDENT) {
		//設定先を取得
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::SearchScope);
		$asmlist.push_back(Inst::StoreLocal | 0);
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::LoadLocal | 0);
			//演算オブジェクト
			$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
			$asmlist.push_back(Inst::LoadObj);
			//操作を実行
			$asmlist.push_back(Inst::PushCopy | 0);
			$asmlist.push_back(Inst::Push | $code->constCode<string>($preop.sym));
			$asmlist.push_back(Inst::LoadObj);
			$asmlist.push_back(Inst::Push | $code->constCode<int>(1));
			$asmlist.push_back(Inst::Apply | 1);
		//設定
		$asmlist.push_back(Inst::StoreObj);
	})
	| ^(PRE_OP preop=operation ^(DOT preexpr=expr[$code] IDENT) {
		//設定先を取得
		$asmlist.insert($asmlist.end(), $preexpr.asmlist.begin(), $preexpr.asmlist.end());
		$asmlist.push_back(Inst::StoreLocal | 0);
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::LoadLocal | 0);
			//演算オブジェクト
			$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
			$asmlist.push_back(Inst::LoadObj);
			//操作を実行
			$asmlist.push_back(Inst::PushCopy | 0);
			$asmlist.push_back(Inst::Push | $code->constCode<string>($preop.sym));
			$asmlist.push_back(Inst::LoadObj);
			$asmlist.push_back(Inst::Push | $code->constCode<int>(1));
			$asmlist.push_back(Inst::Apply | 0);
		//設定
		$asmlist.push_back(Inst::StoreObj);
	})
	| ^(ASSIGN ^(DOT SCOPE IDENT) asrhs=expr[$code] {
		//適用先：解決されたスコープの先
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::SearchScope);
		//第一引数：名前
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		//第二引数：rhsオブジェクト
		$asmlist.insert($asmlist.end(), $asrhs.asmlist.begin(), $asrhs.asmlist.end());
		//設定
		$asmlist.push_back(Inst::StoreObj);
	})
	| ^(ASSIGN ^(DOT asscope=expr[$code] IDENT) asrhs=expr[$code] { //
		//適用先：scopeオブジェクト
		$asmlist.insert($asmlist.end(), $asscope.asmlist.begin(), $asscope.asmlist.end());
		//第一引数：名前
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		//第二引数：rhsオブジェクト
		$asmlist.insert($asmlist.end(), $asrhs.asmlist.begin(), $asrhs.asmlist.end());
		//設定
		$asmlist.push_back(Inst::StoreObj);
	})
	| ^(ASSIGN_OP asopoperation=operation ^(DOT SCOPE IDENT) asoprhs=expr[$code] {
		//設定先
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::SearchScope);
		$asmlist.push_back(Inst::StoreLocal | 0);
		//名前
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
			//操作先のオブジェクトを取得
			$asmlist.push_back(Inst::LoadLocal | 0);
			$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
			$asmlist.push_back(Inst::LoadObj);
			//メソッドを取得
			$asmlist.push_back(Inst::PushCopy | 0);
			$asmlist.push_back(Inst::Push | $code->constCode<string>($asopoperation.sym));
			$asmlist.push_back(Inst::LoadObj);
			//第１引数：rhsオブジェクト
			$asmlist.insert($asmlist.end(), $asoprhs.asmlist.begin(), $asoprhs.asmlist.end());
			//実行
			$asmlist.push_back(Inst::Apply | 1);
		//設定
		$asmlist.push_back(Inst::StoreObj);
	})
	| ^(ASSIGN_OP operation ^(DOT asopscope=expr[$code] IDENT) asoprhs=expr[$code] {
		//設定先
		$asmlist.insert($asmlist.end(), $asscope.asmlist.begin(), $asscope.asmlist.end());
		$asmlist.push_back(Inst::StoreLocal | 0);
		//名前
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
			//操作先のオブジェクトを取得
			$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
			$asmlist.push_back(Inst::LoadObj);
			//メソッドを取得
			$asmlist.push_back(Inst::LoadLocal | 0);
			$asmlist.push_back(Inst::PushCopy | 0);
			$asmlist.push_back(Inst::Push | $code->constCode<string>($asopoperation.sym));
			$asmlist.push_back(Inst::LoadObj);
			//第１引数：rhsオブジェクト
			$asmlist.insert($asmlist.end(), $asoprhs.asmlist.begin(), $asoprhs.asmlist.end());
			//実行
			$asmlist.push_back(Inst::Apply | 1);
		//設定
		$asmlist.push_back(Inst::StoreObj);
	})
	| ^(unary_operation uobj=expr[$code] {
		$asmlist.insert($asmlist.end(), $uobj.asmlist.begin(), $uobj.asmlist.end());
		//メソッドの解決
		$asmlist.push_back(Inst::PushCopy | 0);
		$asmlist.push_back(Inst::Push | $code->constCode<string>($unary_operation.sym));
		$asmlist.push_back(Inst::LoadObj);
		//実行：引数：０
		$asmlist.push_back(Inst::Apply | 0);
	})
	| ^(biop=operation blhs=expr[$code] brhs=expr[$code] {
		//適用先：lhs
		$asmlist.insert($asmlist.end(), $blhs.asmlist.begin(), $blhs.asmlist.end());
		//メソッド解決
		$asmlist.push_back(Inst::PushCopy);
		$asmlist.push_back(Inst::Push | $code->constCode<string>($biop.sym));
		$asmlist.push_back(Inst::LoadObj);
		//第１引数：rhsオブジェクト
		$asmlist.insert($asmlist.end(), $brhs.asmlist.begin(), $brhs.asmlist.end());
		//実行：引数：１
		$asmlist.push_back(Inst::Apply | 1);
	})
	| ^(DOT dexpr=expr[$code] IDENT)
	{
		$asmlist.insert($asmlist.end(), $dexpr.asmlist.begin(), $dexpr.asmlist.end());
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::LoadObj);
	}
	| ^(DOT SCOPE IDENT)
	{
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::SearchScope);
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::LoadObj);
	}
	;

index [ donut::Code* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: ^(IDX obj=expr[$code] ^(ARGS ex=exprlist[$code]))
	{
		$asmlist.insert($asmlist.end(), $obj.asmlist.begin(), $obj.asmlist.end());
		$asmlist.push_back(Inst::PushCopy);
		$asmlist.push_back(Inst::Push | $code->constCode<string>("opIdxGet"));
		$asmlist.push_back(Inst::LoadObj);
		$asmlist.insert($asmlist.end(), $ex.asmlist.begin(), $ex.asmlist.end());
		$asmlist.push_back(Inst::Apply | $ex.count);
	}
	;

apply [ donut::Code* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: ^(APPLY ^(DOT SCOPE IDENT) ^(ARGS ex=exprlist[$code]))
	{
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::SearchScope);
		$asmlist.push_back(Inst::PushCopy);
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::LoadObj);
		$asmlist.insert($asmlist.end(), $ex.asmlist.begin(), $ex.asmlist.end());
		$asmlist.push_back(Inst::Apply | $ex.count);
	}
	| ^(APPLY ^(DOT obj=expr[$code] IDENT) ^(ARGS exprlist[$code]))
	{
		$asmlist.insert($asmlist.end(), $obj.asmlist.begin(), $obj.asmlist.end());
		$asmlist.push_back(Inst::PushCopy);
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::LoadObj);
		$asmlist.insert($asmlist.end(), $ex.asmlist.begin(), $ex.asmlist.end());
		$asmlist.push_back(Inst::Apply | $ex.count);
	}
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
	| array[$code] { $asmlist.swap($array.asmlist); }
	| object[$code] { $asmlist.swap($object.asmlist); }
	| closure[$code] { $asmlist.swap($closure.asmlist); }
	;

object [ donut::Code* code ] returns [ std::vector<donut::Instruction> asmlist ]
@init {
	int size=0;
}
	: ^(OBJECT (v=object_pair[$code]{
		$asmlist.insert($asmlist.end(), $v.asmlist.begin(), $v.asmlist.end());
		size+=1;
	})*)
	{
		$asmlist.push_back(Inst::ConstructObject | size);
	}
	;

object_pair [ donut::Code* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: ^(PAIR IDENT v=expr[$code])
	{
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.insert($asmlist.end(), $v.asmlist.begin(), $v.asmlist.end());
	}
	;

array [ donut::Code* code ] returns [ std::vector<donut::Instruction> asmlist ]
@init {
	int array_count=0;
}
	: ^(ARRAY exprlist[$code]) {
		$asmlist.swap($exprlist.asmlist);
		$asmlist.push_back(Inst::ConstructArray | $exprlist.count);
	}
	;

exprlist [ donut::Code* code ] returns [ std::vector<donut::Instruction> asmlist, int count ]
@init {
	$count = 0;
}
	: (v=expr[$code] {
		$asmlist.insert($asmlist.end(), $v.asmlist.begin(), $v.asmlist.end());
		$count+=1;
	}
	)*;
