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
#include <cstddef>
#include <algorithm>
#include <cinamo/String.h>
#include <donut/source/Source.h>
#include <donut/source/Closure.h>
#include "../ParseUtil.h"

using namespace cinamo;
using namespace donut;
using std::string;
using std::nullptr_t;
using donut::unescapeString;
using cinamo::parseAs;

typedef pANTLR3_COMMON_TOKEN Token;
}
@includes {
#undef __cplusplus
}

prog returns [ Handler<donut::Source> code ]
@init {
	$code = Handler<donut::Source>( new donut::Source() );
}
	: closure[$code.get()]
	{
		$code->setEntrypointID( $closure.closureNo );
	}
	;

closure [ donut::Source* code] returns [ std::vector<donut::Instruction> asmlist, unsigned int closureNo ]
	: ^(CLOS vars[$code] block[$code]
	{
		Handler<donut::Closure> closure = Handler<donut::Closure>(new donut::Closure($vars.list, $block.asmlist));
		$closureNo = $code->constCode<Handler<donut::Closure> >(closure);
		$asmlist.push_back(Inst::Push | $closureNo);
	}
	)
	;

vars [ donut::Source* code ] returns [ std::vector<std::string> list ]
	: ^(VARS (IDENT{
		list.push_back(createStringFromString($IDENT.text));
	})*);

block [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
@after{
	if( $asmlist.empty() ){
		//null値がセットされる
			$asmlist.push_back(Inst::Push | $code->constCode<std::nullptr_t>(nullptr));
	}else{
		//最後のpopは削除する（＝値がひとつだけ残る）
		$asmlist.erase($asmlist.end()-1);
	}
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
	| BIT_OR {sym="opBitOr";}
	| BIT_AND {sym="opBitAnd";}
	;

unary_operation returns [ std::string sym ]
	: PLUS { sym="opPlus"; }
	| MINUS { sym="opMinus"; }
	| NOT { sym="opNot"; }
	;

expr [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: literal[$code] { $asmlist.swap($literal.asmlist); }
	| apply[$code] { $asmlist.swap($apply.asmlist); }
	| loop[$code] { $asmlist.swap($loop.asmlist); }
	| cond[$code] { $asmlist.swap($cond.asmlist); }
	| return_[$code] { $asmlist.swap($return_.asmlist); }
	| interrupt[$code] { $asmlist.swap($interrupt.asmlist); }
	| preop[$code] {$asmlist.swap($preop.asmlist);}
	| postop[$code] {$asmlist.swap($postop.asmlist);}
	| assignop[$code] {$asmlist.swap($assignop.asmlist);}
	| assign[$code] {$asmlist.swap($assign.asmlist);}
	| block[$code] {$asmlist.swap($block.asmlist);}
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
	| ^(DOT dexpr=expr[$code] ac=accessor[$code])
	{
		$asmlist.insert($asmlist.end(), $dexpr.asmlist.begin(), $dexpr.asmlist.end());
		$asmlist.insert($asmlist.end(), $ac.asmlist.begin(), $ac.asmlist.end());
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

preop [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: ^(PRE_OP operation ^(DOT SCOPE IDENT) {
		//設定先を取得
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::SearchScope);
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
			//演算オブジェクト
			$asmlist.push_back(Inst::PushCopy | 1); //Scope
			$asmlist.push_back(Inst::PushCopy | 1); //name
			$asmlist.push_back(Inst::LoadObj);
			//操作を実行
			$asmlist.push_back(Inst::PushCopy | 0);
			$asmlist.push_back(Inst::Push | $code->constCode<string>($operation.sym));
			$asmlist.push_back(Inst::LoadObj);
			$asmlist.push_back(Inst::Push | $code->constCode<int>(1));
			$asmlist.push_back(Inst::Apply | 1);
		//設定
		$asmlist.push_back(Inst::StoreObj);
	})
	| ^(PRE_OP operation ^(DOT preexpr=expr[$code] ac=accessor[$code]) {
		//設定先を取得
		$asmlist.insert($asmlist.end(), $preexpr.asmlist.begin(), $preexpr.asmlist.end());
		$asmlist.insert($asmlist.end(), $ac.asmlist.begin(), $ac.asmlist.end());
			//演算オブジェクト
			$asmlist.push_back(Inst::PushCopy | 1); //Scope
			$asmlist.push_back(Inst::PushCopy | 1); //name
			$asmlist.push_back(Inst::LoadObj);
			//操作を実行
			$asmlist.push_back(Inst::PushCopy | 0);
			$asmlist.push_back(Inst::Push | $code->constCode<string>($operation.sym));
			$asmlist.push_back(Inst::LoadObj);
			$asmlist.push_back(Inst::Push | $code->constCode<int>(1));
			$asmlist.push_back(Inst::Apply | 1);
		//設定
		$asmlist.push_back(Inst::StoreObj);
	});

postop [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: ^(POST_OP operation ^(DOT SCOPE IDENT) {
		//あとで値になる部分のnullを設定する
		$asmlist.push_back(Inst::Push | $code->constCode<nullptr_t>(0));
		//設定先を取得
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::SearchScope);
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
			//演算オブジェクト
			$asmlist.push_back(Inst::PushCopy | 1); //scope
			$asmlist.push_back(Inst::PushCopy | 1); //name
			$asmlist.push_back(Inst::LoadObj);
			$asmlist.push_back(Inst::ReplaceCopy | 3);
			//操作を実行
			$asmlist.push_back(Inst::PushCopy | 0);
			$asmlist.push_back(Inst::Push | $code->constCode<string>($operation.sym));
			$asmlist.push_back(Inst::LoadObj);
			$asmlist.push_back(Inst::Push | $code->constCode<int>(1));
			$asmlist.push_back(Inst::Apply | 1);
		//設定
		$asmlist.push_back(Inst::StoreObj);
		$asmlist.push_back(Inst::Pop | 0);
	})
	| ^(POST_OP operation ^(DOT postexpr=expr[$code] ac=accessor[$code]) {
		//あとで値になる部分のnullを設定する
		$asmlist.push_back(Inst::Push | $code->constCode<nullptr_t>(nullptr));
		//設定先を取得
		$asmlist.insert($asmlist.end(), $postexpr.asmlist.begin(), $postexpr.asmlist.end());
		$asmlist.insert($asmlist.end(), $ac.asmlist.begin(), $ac.asmlist.end());
			//演算オブジェクト
			$asmlist.push_back(Inst::PushCopy | 1); //scope
			$asmlist.push_back(Inst::PushCopy | 1); //ident
			$asmlist.push_back(Inst::LoadObj);
			$asmlist.push_back(Inst::ReplaceCopy | 3);
			//操作を実行
			$asmlist.push_back(Inst::PushCopy | 0);
			$asmlist.push_back(Inst::Push | $code->constCode<string>($operation.sym));
			$asmlist.push_back(Inst::LoadObj);
			$asmlist.push_back(Inst::Push | $code->constCode<int>(1));
			$asmlist.push_back(Inst::Apply | 1);
		//設定
		$asmlist.push_back(Inst::StoreObj);
		$asmlist.push_back(Inst::Pop | 0);
	})
	;

assignop [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: ^(ASSIGN_OP asopoperation=operation ^(DOT SCOPE IDENT) asoprhs=expr[$code] {
		//設定先
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::SearchScope);
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		//名前
			//操作先のオブジェクトを取得
			$asmlist.push_back(Inst::PushCopy | 1); //scope
			$asmlist.push_back(Inst::PushCopy | 1); //name
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
	| ^(ASSIGN_OP asopoperation=operation ^(DOT asopscope=expr[$code] ac=accessor[$code]) asoprhs=expr[$code] {
		//設定先
		$asmlist.insert($asmlist.end(), $asopscope.asmlist.begin(), $asopscope.asmlist.end());
		$asmlist.insert($asmlist.end(), $ac.asmlist.begin(), $ac.asmlist.end());
		//名前
			//操作先のオブジェクトを取得
			$asmlist.push_back(Inst::PushCopy | 1); //scope
			$asmlist.push_back(Inst::PushCopy | 1); //name
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
	;

assign [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: ^(ASSIGN ^(DOT SCOPE IDENT) asrhs=expr[$code] {
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
	| ^(ASSIGN ^(DOT asscope=expr[$code] ac=accessor[$code]) asrhs=expr[$code] { //
		//適用先：scopeオブジェクト
		$asmlist.insert($asmlist.end(), $asscope.asmlist.begin(), $asscope.asmlist.end());
		//第一引数：名前
		$asmlist.insert($asmlist.end(), $ac.asmlist.begin(), $ac.asmlist.end());
		//第二引数：rhsオブジェクト
		$asmlist.insert($asmlist.end(), $asrhs.asmlist.begin(), $asrhs.asmlist.end());
		//設定
		$asmlist.push_back(Inst::StoreObj);
	})
	;


accessor [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: IDENT
	{
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
	}
	| expr[$code]
	{
		$asmlist.insert($asmlist.end(), $expr.asmlist.begin(), $expr.asmlist.end());
		$asmlist.push_back(Inst::PushCopy);
		$asmlist.push_back(Inst::Push | $code->constCode<string>("toString"));
		$asmlist.push_back(Inst::LoadObj);
		$asmlist.push_back(Inst::Apply | 0);
	}
	;

apply [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: ^(APPLY ^(DOT SCOPE IDENT) ex=arglist[$code])
	{
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::SearchScope);
		$asmlist.push_back(Inst::PushCopy);
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.push_back(Inst::LoadObj);
		$asmlist.insert($asmlist.end(), $ex.asmlist.begin(), $ex.asmlist.end());
		$asmlist.push_back(Inst::Apply | $ex.count);
	}
	| ^(APPLY ^(DOT obj=expr[$code] ac=accessor[$code]) ex=arglist[$code])
	{
		$asmlist.insert($asmlist.end(), $obj.asmlist.begin(), $obj.asmlist.end());
		$asmlist.push_back(Inst::PushCopy);
		$asmlist.insert($asmlist.end(), $ac.asmlist.begin(), $ac.asmlist.end());
		$asmlist.push_back(Inst::LoadObj);
		$asmlist.insert($asmlist.end(), $ex.asmlist.begin(), $ex.asmlist.end());
		$asmlist.push_back(Inst::Apply | $ex.count);
	}
	| ^(APPLY closure[$code] ex=arglist[$code])
	{
		$asmlist.push_back(Inst::PushSelf);
		$asmlist.insert($asmlist.end(), $closure.asmlist.begin(), $closure.asmlist.end());
		$asmlist.insert($asmlist.end(), $ex.asmlist.begin(), $ex.asmlist.end());
		$asmlist.push_back(Inst::Apply | $ex.count);
	}
	;

cond [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: ^(COND ifcond=expr[$code] ift=block[$code]
		(iff=block[$code]
		{
			$asmlist.insert($asmlist.end(), $ifcond.asmlist.begin(), $ifcond.asmlist.end());
			$asmlist.push_back(Inst::BranchFalse | $ift.asmlist.size()+1);
			$asmlist.insert($asmlist.end(), $ift.asmlist.begin(), $ift.asmlist.end());
			$asmlist.push_back(Inst::Branch | $iff.asmlist.size());
			$asmlist.insert($asmlist.end(), $iff.asmlist.begin(), $iff.asmlist.end());
		}
		|ifc=cond[$code]
		{
			$asmlist.insert($asmlist.end(), $ifcond.asmlist.begin(), $ifcond.asmlist.end());
			$asmlist.push_back(Inst::BranchFalse | $ift.asmlist.size()+1);
			$asmlist.insert($asmlist.end(), $ift.asmlist.begin(), $ift.asmlist.end());
			$asmlist.push_back(Inst::Branch | $ifc.asmlist.size());
			$asmlist.insert($asmlist.end(), $ifc.asmlist.begin(), $ifc.asmlist.end());
		}
		)
	)
	;

loop [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: ^(FOR forstart=block[$code] forcond=block[$code] fornext=block[$code] forblock=block[$code])
	{
		$asmlist.insert($asmlist.end(), $forstart.asmlist.begin(), $forstart.asmlist.end());
		$asmlist.insert($asmlist.end(), $forcond.asmlist.begin(), $forcond.asmlist.end());
		$asmlist.push_back(Inst::BranchFalse | ($forblock.asmlist.size() + $fornext.asmlist.size()+3));
		$asmlist.push_back(Inst::Pop);
		$asmlist.insert($asmlist.end(), $forblock.asmlist.begin(), $forblock.asmlist.end());
		$asmlist.insert($asmlist.end(), $fornext.asmlist.begin(), $fornext.asmlist.end());
		$asmlist.push_back(Inst::Pop);
		$asmlist.push_back(Inst::Branch | ((-($forcond.asmlist.size()+$forblock.asmlist.size()+$fornext.asmlist.size()+4)) & 0xffff));
	}
	;

literal [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: TRUE_LITERAL
	{
		$asmlist.push_back(Inst::Push | $code->constCode<bool>(true));
	}
	| FALSE_LITERAL
	{
		$asmlist.push_back(Inst::Push | $code->constCode<bool>(false));
	}
	| NULL_LITERAL
	{
		$asmlist.push_back(Inst::Push | $code->constCode<nullptr_t>(nullptr));
	}
	| HEX_LITERAL
	{
		std::string str(createStringFromString($HEX_LITERAL.text));
		int const val = parseAs<int>(str, 16, 0);
		$asmlist.push_back(Inst::Push | $code->constCode<int>(val));
	}
	| OCT_LITERAL
	{
		std::string str(createStringFromString($OCT_LITERAL.text));
		int const val = parseAs<int>(str, 8, 0);
		$asmlist.push_back(Inst::Push | $code->constCode<int>(val));
	}
	| INT_LITERAL
	{
		std::string str(createStringFromString($INT_LITERAL.text));
		int const val = parseAs<int>(str, 10, 0);
		$asmlist.push_back(Inst::Push | $code->constCode<int>(val));
	}
	| FLOAT_LITERAL
	{
		std::string str(createStringFromString($FLOAT_LITERAL.text));
		float const val = parseAs<float>(str, NAN);
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
	| SELF { $asmlist.push_back(Inst::PushSelf); }
	;

return_ [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: ^(RETURN expr[$code])
	{
		$asmlist.insert($asmlist.end(), $expr.asmlist.begin(), $expr.asmlist.end());
		$asmlist.push_back(Inst::Return | 0);
	}
	;

interrupt [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: ^(INTERRUPT expr[$code])
	{
		$asmlist.insert($asmlist.end(), $expr.asmlist.begin(), $expr.asmlist.end());
		$asmlist.push_back(Inst::Interrupt | 0);
	}
	;

object [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
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

object_pair [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
	: ^(PAIR IDENT v=expr[$code])
	{
		$asmlist.push_back(Inst::Push | $code->constCode<string>(createStringFromString($IDENT.text)));
		$asmlist.insert($asmlist.end(), $v.asmlist.begin(), $v.asmlist.end());
	}
	;

array [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist ]
@init {
	int array_count=0;
}
	: ^(ARRAY  (v=expr[$code] {
			$asmlist.insert($asmlist.end(), $v.asmlist.begin(), $v.asmlist.end());
			array_count+=1;
		}
		)*) {
		$asmlist.push_back(Inst::ConstructArray | array_count);
	}
	;

arglist [ donut::Source* code ] returns [ std::vector<donut::Instruction> asmlist, int count ]
@init {
	$count=0;
}
	: ^(ARGS (v=expr[$code] {
		$asmlist.insert($asmlist.end(), $v.asmlist.begin(), $v.asmlist.end());
		$count+=1;
	})*)
	;

