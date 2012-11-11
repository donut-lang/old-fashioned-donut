grammar Donut;

options {
	output=AST;
	language = C;
}

tokens {
	ARRAY;
	VARS;
	ARGS;
	CLOS;
	CONT;
	IDX;
	APPLY;
	
	ASSIGN;
	ASSIGN_OP;
	POST_OP;
	PRE_OP;
	
	ADD;
	SUB;
	MUL;
	DIV;
	MOD;

	PLUS;
	MINUS;
	NOT;

	SCOPE;
	DOT;
	
	AND;
	OR;
	
	CLT;
	CGT;
	CLE;
	CGE;
	CEQ;
	CNE;
	
	OBJECT;
	PAIR;
	
	IF;
	FOR;
}

@includes {
#undef __cplusplus
#undef LT
}

source : program EOF -> program;

program : exprlist -> ^(CLOS VARS exprlist);

exprlist : expr ((';')+ expr)* (';')? -> ^(CONT expr*);

expr
	: 'func' '(' varlist ')' '{' exprlist '}' -> ^(CLOS varlist exprlist)
	| 'if' '(' expr ')' '{' a=exprlist '}' 'else' '{' b=exprlist '}' -> ^(IF expr $a $b)
	| 'for' '(' fa=expr? ';' fb=expr? ';' fc=expr? ')' '{' fd=exprlist '}' -> ^(FOR ^(CONT $fa?) ^(CONT $fb?) ^(CONT $fc?) $fd)
	| 'while' '(' fb=expr? ')' '{' fd=exprlist '}' -> ^(FOR ^(CONT) ^(CONT $fb) ^(CONT) $fd)
	| expr6;

expr6 : (a=expr5->$a)
	( '='  b=expr -> ^(ASSIGN $expr6 $b)
	| '+=' b=expr -> ^(ASSIGN_OP ADD $expr6 $b)
	| '-=' b=expr -> ^(ASSIGN_OP SUB $expr6 $b)
	| '*=' b=expr -> ^(ASSIGN_OP MUL $expr6 $b)
	| '/=' b=expr -> ^(ASSIGN_OP DIV $expr6 $b)
	| '%=' b=expr -> ^(ASSIGN_OP MOD $expr6 $b)
	)?;


expr5 : (a=expr4->$a) ('||' b=expr4 -> ^(OR $expr5 $b))*;
expr4 : (a=expr3->$a) ('&&' b=expr3 -> ^(AND $expr4 $b))*;

expr3 : (a=expr2->$a) 
	( '<' b=expr2 -> ^(CLT $expr3 $b)
	| '>' b=expr2 -> ^(CGT $expr3 $b)
	| '==' b=expr2 -> ^(CEQ $expr3 $b)
	| '!=' b=expr2 -> ^(CNE $expr3 $b)
	| '<=' b=expr2 -> ^(CLE $expr3 $b)
	| '>=' b=expr2 -> ^(CGE $expr3 $b)
	)*;

expr2 : (a=expr1->$a)
	( '+' b=expr1 -> ^(ADD $expr2 $b)
	| '-' b=expr1 -> ^(SUB $expr2 $b)
	)*;

expr1  : (a=term->$a)
	( '*' b=term -> ^(MUL $expr1 $b)
	| '/' b=term -> ^(DIV $expr1 $b)
	| '%' b=term -> ^(MOD $expr1 $b)
	)*;

term
	: '++' term -> ^(PRE_OP ADD term)
	| '--' term -> ^(PRE_OP SUB term)
	| '+' term -> ^(PLUS term)
	| '-' term -> ^(MINUS term)
	| '!' term -> ^(NOT term)
	| postfix
	;
postfix
	: (p=primary->$p)
	( '++' -> ^(POST_OP ADD $postfix)
	| '--'	-> ^(POST_OP SUB $postfix)
	| '.' n=name -> ^(DOT $postfix $n)
	| '[' arl=arglist ']' -> ^(IDX $postfix $arl)
	| '(' apl=arglist ')' -> ^(APPLY $postfix $apl)
	)*
	;
primary
	: literal
	| array
	| name -> ^(DOT SCOPE name)
	| '(' expr ')' -> expr
	| object
	;


name : IDENT;

varlist : (name ((',') name)*)? -> ^(VARS name*);

arglist : exprlist_for_literal -> ^(ARGS exprlist_for_literal);
array : '[' exprlist_for_literal ','? ']' -> ^(ARRAY exprlist_for_literal);
exprlist_for_literal : (expr (',' expr)*)? -> expr*;

object : '{' (object_pair (',' object_pair)* (',')?)? '}' -> ^(OBJECT object_pair*);
object_pair : name '=>' expr -> ^(PAIR name expr);

literal
	: numeric_literal
	| boolean_literal
	| string_literal
	| 'null'
	;

boolean_literal
	: 'true'
	| 'false';

numeric_literal
	: HEX_LITERAL
	| OCT_LITERAL
	| INT_LITERAL
	| FLOAT_LITERAL;

string_literal : STRING_SINGLE | STRING_DOUBLE;

//---------------------------------------------------------------------------------------------------------------------

IDENT:
	LETTER (LETTER | DIGIT)*;

fragment
LETTER:
	'A'..'Z'
	| 'a'..'z'
	| '_'
	| '@'
	| '$'
	;

STRING_SINGLE: '\'' STRING_SINGLE_ELEMENT* '\'';

fragment
STRING_SINGLE_ELEMENT: ~('\''|'\r'|'\n');

STRING_DOUBLE: '"' STRING_DOUBLE_ELEMENT* '"';

fragment
STRING_DOUBLE_ELEMENT: ESC_SEQ | ~('\\'|'"'|'\r'|'\n');

fragment
ESC_SEQ
	:'\\'
		( 'b'
		| 't'
		| 'n'
		| 'f'
		| 'r'
		| '"'
		| '\''
		| '\\'
		)?
	;

HEX_LITERAL:
	'0' ('x'|'X') HEX_DIGIT+;

OCT_LITERAL :
	'0' (OCT_DIGIT)*;

INT_LITERAL : NONZERO_DIGIT DIGIT*;

FLOAT_LITERAL
    :   ('0'..'9')+ '.' ('0'..'9')* EXPONENT?
    |   '.' ('0'..'9')+ EXPONENT?
    |   ('0'..'9')+ EXPONENT
    ;

fragment
DIGIT :
  '0'..'9';
fragment
NONZERO_DIGIT :
  '1'..'9';
fragment EXPONENT
	: ('e'|'E') ('+'|'-')? ('0'..'9')+ ;
fragment HEX_DIGIT
	: ('0'..'9'|'A'..'F'|'a'..'f');

fragment OCT_DIGIT
	: '0'..'7';

COMMENT: '//' (~('\r'|'\n'))* {$channel=HIDDEN;};
WS: (' '|'\t'|'\r'|'\n')+ {$channel=HIDDEN;} ; // ignore whitespace

