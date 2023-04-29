%language "c++"
%require "3.4"
// NEVER SET THIS INTERNALLY - SHALL BE SET BY CMAKE: %defines "../private/caparser.hpp"
// NEVER SET THIS INTERNALLY - SHALL BE SET BY CMAKE: %output "../private/caparser.cpp"
%locations
%define api.location.type {cecko::loc_t}
%define api.namespace {cecko}
%define api.value.type variant
%define api.token.constructor
%define api.parser.class {parser}
%define api.token.prefix {TOK_}
//%define parse.trace
%define parse.assert
%define parse.error verbose

%code requires
{
// this code is emitted to caparser.hpp

#include "ckbisonflex.hpp"

// adjust YYLLOC_DEFAULT macro for our api.location.type
#define YYLLOC_DEFAULT(res,rhs,N)	(res = (N)?YYRHSLOC(rhs, 1):YYRHSLOC(rhs, 0))

#include "ckgrptokens.hpp"
#include "ckcontext.hpp"
#include "casem.hpp"
}

%code
{
// this code is emitted to caparser.cpp

YY_DECL;

using namespace casem;
}

%param {yyscan_t yyscanner}		// the name yyscanner is enforced by Flex
%param {cecko::context * ctx}

%start translation_unit

%token EOF		0				"end of file"

%token						LBRA		"["
%token						RBRA		"]"
%token						LPAR		"("
%token						RPAR		")"
%token						DOT			"."
%token						ARROW		"->"
%token<cecko::gt_incdec>	INCDEC		"++ or --"
%token						COMMA		","
%token						AMP			"&"
%token						STAR		"*"
%token<cecko::gt_addop>		ADDOP		"+ or -"
%token						EMPH		"!"
%token<cecko::gt_divop>		DIVOP		"/ or %"
%token<cecko::gt_cmpo>		CMPO		"<, >, <=, or >="
%token<cecko::gt_cmpe>		CMPE		"== or !="
%token						DAMP		"&&"
%token						DVERT		"||"
%token						ASGN		"="
%token<cecko::gt_cass>		CASS		"*=, /=, %=, +=, or -="
%token						SEMIC		";"
%token						LCUR		"{"
%token						RCUR		"}"
%token						COLON		":"

%token						TYPEDEF		"typedef"
%token						VOID		"void"
%token<cecko::gt_etype>		ETYPE		"_Bool, char, or int"
%token						STRUCT		"struct"
%token						ENUM		"enum"
%token						CONST		"const"
%token						IF			"if"
%token						ELSE		"else"
%token						DO			"do"
%token						WHILE		"while"
%token						FOR			"for"
%token						GOTO		"goto"
%token						CONTINUE	"continue"
%token						BREAK		"break"
%token						RETURN		"return"
%token						SIZEOF		"sizeof"

%token<CIName>				IDF			"identifier"
%token<CIName>				TYPEIDF		"type identifier"
%token<int>					INTLIT		"integer literal"
%token<CIName>				STRLIT		"string literal"

/////////////////////////////////

%%

/////////////////////////////////

identifier:
	IDF
	;
	
string-literal:
	STRLIT
	;

constant:
	INTLIT
	;

translation_unit:
	external-declaration translation_unit 
	| %empty
	;

external-declaration:
	function-definition
	| declaration
	;

function-definition:
	declaration-specifiers declarator compound-statement
	;

primary-expression:
	identifier
	| constant
	| string-literal
	| LPAR expression RPAR
	;

postfix-expression:
	primary-expression
	| postfix-expression LBRA expression RBRA
	| postfix-expression LPAR argument-expression-list RPAR
	| postfix-expression LPAR                          RPAR
	| postfix-expression DOT identifier
	| postfix-expression ARROW identifier
	| postfix-expression INCDEC
	;

argument-expression-list:
	assignment-expression
	| argument-expression-list COMMA assignment-expression
	;

unary-expression:
	postfix-expression
	| INCDEC unary-expression
	| unary-operator cast-expression
	| SIZEOF LPAR type-name RPAR
	;

unary-operator: 
	AMP
	| STAR
	| ADDOP
	| EMPH

cast-expression:
	unary-expression
	;

multiplicative-expression:
	cast-expression
	| multiplicative-expression STAR cast-expression
	| multiplicative-expression DIVOP cast-expression
	;

additive-expression:
	multiplicative-expression
	| additive-expression ADDOP multiplicative-expression
	;

relational-expression:
	additive-expression
	| relational-expression CMPO additive-expression
	;

equality-expression:
	relational-expression
	| equality-expression CMPE relational-expression
	;

logical-AND-expression:
	equality-expression
	| logical-AND-expression DAMP equality-expression
	;
	
logical-OR-expression:
	logical-AND-expression
	| logical-OR-expression DVERT logical-AND-expression
	;

assignment-expression:
	logical-OR-expression
	| unary-expression assignment-operator assignment-expression
	;

assignment-operator: 
	CASS
	| ASGN
	;

expression:
	assignment-expression
	;

expression_opt:
	assignment-expression
	| %empty
	;

constant-expression:
	logical-OR-expression
	;

declaration:
	no-leading-attribute-declaration
	;

no-leading-attribute-declaration:
	declaration-specifiers init-declarator-list SEMIC
	| declaration-specifiers  SEMIC
	;

declaration-specifiers:
	declaration-specifier declaration-specifiers
	| declaration-specifier 
	;

declaration-specifier:
	storage-class-specifier
	| type-specifier-qualifier
	;

init-declarator-list:
	init-declarator
	| init-declarator-list COMMA init-declarator
	;

init-declarator:
	declarator
	;

storage-class-specifier:
	TYPEDEF
	;

type-specifier:
	VOID
	| ETYPE
	| struct-or-union-specifier
	| enum-specifier
	| typedef-name
	;

struct-or-union-specifier:
	struct-or-union identifier LCUR member-declaration-list RCUR
	| struct-or-union identifier
	;

struct-or-union:
	STRUCT
	;

member-declaration-list:
	member-declaration
	| member-declaration-list member-declaration
	;

member-declaration:
	specifier-qualifier-list member-declarator-list SEMIC
	| specifier-qualifier-list SEMIC
	;

specifier-qualifier-list:
	type-specifier-qualifier
	| type-specifier-qualifier specifier-qualifier-list
	;

type-specifier-qualifier:
	type-specifier
	| type-qualifier
	;

member-declarator-list:
	member-declarator
	| member-declarator-list COMMA member-declarator
	;

member-declarator:
	declarator
	;

enum-specifier:
	ENUM identifier LCUR enumerator-list RCUR
	| ENUM identifier LCUR enumerator-list COMMA RCUR
	| ENUM identifier
	;

enumerator-list:
	enumerator
	| enumerator-list COMMA enumerator
	;

enumerator:
	enumeration-constant
	| enumeration-constant ASGN constant-expression
	;

enumeration-constant:
	identifier
	;

type-qualifier:
	CONST							
	;

declarator:
	pointer direct-declarator
	| direct-declarator
	;

direct-declarator:
	identifier
	| LPAR declarator RPAR
	| array-declarator
	| function-declarator
	;

array-declarator:
	direct-declarator LBRA assignment-expression RBRA
	;

function-declarator:
	direct-declarator LPAR parameter-type-list RPAR
	;

pointer:
	STAR type-qualifier-list
	| STAR 
	| STAR type-qualifier-list pointer
	| STAR pointer
	;

type-qualifier-list:
	type-qualifier
	| type-qualifier-list type-qualifier
	;

parameter-type-list:
	parameter-list
	;

parameter-list:
	parameter-declaration
	| parameter-list COMMA parameter-declaration
	;

parameter-declaration:
	declaration-specifiers declarator
	| declaration-specifiers abstract-declarator
	| declaration-specifiers
	;

type-name:
	specifier-qualifier-list abstract-declarator
	| specifier-qualifier-list
	;

abstract-declarator:
	pointer
	| pointer direct-abstract-declarator
	|         direct-abstract-declarator
	;

direct-abstract-declarator:
	LPAR abstract-declarator RPAR
	| array-abstract-declarator
	| function-abstract-declarator
	;

array-abstract-declarator:
	direct-abstract-declarator LBRA assignment-expression RBRA
	| LBRA assignment-expression RBRA
	;

function-abstract-declarator:
	direct-abstract-declarator LPAR parameter-type-list RPAR
	|                          LPAR parameter-type-list RPAR
	;

typedef-name:
	TYPEIDF
	;

compound-statement:
	LCUR block-item-list RCUR
	| LCUR                 RCUR
	;

block-item-list:
	block-item
	| block-item-list block-item
	;
	
block-item:
	declaration
	| statement
	;

expression-statement:
	expression_opt SEMIC
	;

statement: 
	matched
	| unmatched
	;

other-statement:
	expression-statement
	| compound-statement
	| jump-statement
	;

matched: 
	IF LPAR expression RPAR matched ELSE matched
	| WHILE LPAR expression RPAR matched
	| DO statement WHILE LPAR expression RPAR SEMIC
	| FOR LPAR expression_opt SEMIC expression_opt SEMIC expression_opt RPAR matched
	| other-statement
	;

unmatched:
	IF LPAR expression RPAR statement
	| IF LPAR expression RPAR matched ELSE unmatched
	| WHILE LPAR expression RPAR unmatched
	| FOR LPAR expression_opt SEMIC expression_opt SEMIC expression_opt RPAR unmatched
	;

jump-statement:
	RETURN expression_opt SEMIC
	;
	
/////////////////////////////////

%%

namespace cecko {

	void parser::error(const location_type& l, const std::string& m)
	{
		ctx->message(cecko::errors::SYNTAX, l, m);
	}

}
