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
	
}

@includes {
#undef __cplusplus
#undef LT
}

source : program EOF -> program;

program : exprlist -> ^(CLOS ARGS exprlist);

exprlist : expr ((';')+ expr)* (';')? -> ^(CONT expr*);

expr
	: 'func' '(' varlist ')' '{' exprlist '}' -> ^(CLOS varlist exprlist)
	| expr6;

expr6 : (a=expr5->$a)
	( '='  b=expr -> ^(ASSIGN $a $b)
	| '+=' b=expr -> ^(ASSIGN_OP ADD $a $b)
	| '-=' b=expr -> ^(ASSIGN_OP SUB $a $b)
	| '*=' b=expr -> ^(ASSIGN_OP MUL $a $b)
	| '/=' b=expr -> ^(ASSIGN_OP DIV $a $b)
	| '%=' b=expr -> ^(ASSIGN_OP MOD $a $b)
	)?;


expr5 : (a=expr4->$a) ('||' b=expr4 -> ^(OR $a $b))*;
expr4 : (a=expr3->$a) ('&&' b=expr3 -> ^(AND $a $b))*;

expr3 : (a=expr2->$a) 
	( '<' b=expr2 -> ^(CLT $a $b)
	| '>' b=expr2 -> ^(CGT $a $b)
	| '==' b=expr2 -> ^(CEQ $a $b)
	| '!=' b=expr2 -> ^(CNE $a $b)
	| '<=' b=expr2 -> ^(CLE $a $b)
	| '>=' b=expr2 -> ^(CGE $a $b)
	)*;

expr2 : (a=expr1->$a)
	( '+' b=expr1 -> ^(ADD $a $b)
	| '-' b=expr1 -> ^(SUB $a $b)
	)*;

expr1  : (a=term->$a)
	( '*' b=term -> ^(MUL $a $b)
	| '/' b=term -> ^(DIV	$a $b)
	| '%' b=term -> ^(MOD $a $b)
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
	( '++' -> ^(POST_OP ADD $p)
	| '--'	-> ^(POST_OP SUB $p)
	| '.' n=name -> ^(DOT $p $n)
	| '[' arl=arglist ']' -> ^(IDX $p $arl)
	| '(' apl=arglist ')' -> ^(APPLY $p $apl)
	)*
	;
primary
	: literal
	| array
	| name -> ^(DOT SCOPE name)
	| '(' expr ')' -> expr
	;


name : IDENT;

varlist : (name ((',') name)*)? -> ^(VARS name*);

arglist : (expr ((',') expr)*)? -> ^(ARGS expr*);

array : '[' arglist ']' -> ^(ARRAY arglist);

literal
	: numeric_literal
	| boolean_literal
	| string_literal
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
	'0' (OCT_DIGIT)+;

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

