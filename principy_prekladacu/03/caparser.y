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
#define YYLLOC_DEFAULT(res,rhs,N)	(res=(N)?YYRHSLOC(rhs, 1):YYRHSLOC(rhs, 0))

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
%token<cecko::gt_cmpe>		CMPE		"==or !="
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

%type<CIName>				identifier

%type<casem::DeclaratorTypeFragment>				function-declarator
%type<casem::DeclaratorTypeFragment>				direct-declarator
%type<casem::DeclaratorTypeFragment>				array-declarator
%type<casem::DeclaratorTypeFragment>				declarator
%type<casem::DeclaratorTypeFragment>				init-declarator
%type<casem::DeclaratorTypeFragmentList>			init-declarator-list

%type<casem::DeclaratorTypeFragmentList>		parameter-type-list
%type<casem::DeclaratorTypeFragmentList>		parameter-list

%type<casem::DeclaratorTypeFragment>			declaration-specifiers

%type<casem::DeclaratorTypeFragment>			declaration-specifier
%type<casem::DeclaratorTypeFragment>			specifier-qualifier-list

%type<casem::DeclaratorTypeFragment>			type-qualifier-list
%type<casem::DeclaratorTypeFragment>			pointer
%type<casem::DeclaratorTypeFragment>			parameter-declaration

%type<casem::StorageSpecifier>	  		storage-class-specifier
%type<casem::DeclaratorTypeFragment>	type-specifier-qualifier
%type<casem::TypeQualifier>				type-qualifier

%type<CKTypeRefPack>					type-specifier
%type<CKTypedefConstSafeObs>			typedef-name

%type<casem::DeclaratorTypeFragment>	direct-abstract-declarator
%type<casem::DeclaratorTypeFragment>	function-abstract-declarator
%type<casem::DeclaratorTypeFragment>	abstract-declarator



%type<casem::DeclaratorTypeFragment> 	function-definition-body

%type<cecko::CKIRConstantIntObs>		constant

%type<cecko::CKIRConstantIntObs>		constant-expression
%type<cecko::CKIRConstantIntObs>		expression
%type<cecko::CKIRConstantIntObs>		assignment-expression
%type<cecko::CKIRConstantIntObs>		logical-OR-expression
%type<cecko::CKIRConstantIntObs>		primary-expression
%type<cecko::CKIRConstantIntObs>		postfix-expression
%type<cecko::CKIRConstantIntObs>		unary-expression
%type<cecko::CKIRConstantIntObs>		cast-expression
%type<cecko::CKIRConstantIntObs>		multiplicative-expression
%type<cecko::CKIRConstantIntObs>		additive-expression
%type<cecko::CKIRConstantIntObs>		relational-expression
%type<cecko::CKIRConstantIntObs>		equality-expression
%type<cecko::CKIRConstantIntObs>		logical-AND-expression

%type<cecko::CKEnumTypeSafeObs>			enum-specifier
%type<cecko::CKEnumTypeSafeObs>			enum-body
%type<casem::EnumeratorList> 			enumerator-list
%type<casem::EnumElement> 				enumerator
%type<cecko::CKEnumTypeSafeObs>			enum-start
%type<CIName> 							enumeration-constant

%type<cecko::CKStructTypeSafeObs> 				struct-or-union-specifier
%type<cecko::CKStructTypeSafeObs> 				struct-start
%type<cecko::CKStructTypeSafeObs> 				struct-body

%type<casem::DeclaratorTypeFragmentList> 		member-declaration-list
%type<casem::DeclaratorTypeFragmentList> 		member-declaration

%type<casem::DeclaratorTypeFragment>  			member-declarator
%type<casem::DeclaratorTypeFragmentList> 		member-declarator-list



/////////////////////////////////

%%

/////////////////////////////////

struct-or-union-specifier:
	struct-body RCUR					
	| struct-start		
	;

struct-body:
	struct-start LCUR member-declaration-list		{ $$=ctx->define_struct_type_open(($1)->get_name(), @1);  process_struct(ctx, $$, $3);   }

struct-start:
	struct-or-union identifier						{ $$=ctx->declare_struct_type($2, @2);}
	;

struct-or-union:
	STRUCT
	;

member-declaration-list:
	member-declaration								{ $$=$1;  }
	| member-declaration-list member-declaration	{ $$=$1;  for(auto d : $2) $$.push_back(d); }
	;

member-declaration:
	specifier-qualifier-list member-declarator-list SEMIC	{ DeclaratorTypeFragmentList r;	for(auto d : $2) { d.add($1);  r.push_back(d); }  $$=r;  }
	| specifier-qualifier-list SEMIC						{ DeclaratorTypeFragmentList d; d.push_back($1); $$=d;}
	;

member-declarator-list:
	member-declarator										{ DeclaratorTypeFragmentList d; d.push_back($1); $$=d; }
	| member-declarator-list COMMA member-declarator		{ $$=$1; $$.push_back($3); }
	;

member-declarator:
	declarator								{ $$=$1;  }
	;

enum-specifier:
	enum-body RCUR				{ $$=$1; }
	| enum-start				{ $$=$1; }	
	;

enum-start:
	ENUM identifier 			{ $$=ctx->declare_enum_type($2, @2);}
	;

enum-body:
	enum-start	LCUR enumerator-list					{  $$=ctx->define_enum_type_open(($1)->get_name(), @2); process_enum(ctx, $1, $3);  }
	| 	enum-start LCUR enumerator-list COMMA			{  $$=ctx->define_enum_type_open(($1)->get_name(), @2); process_enum(ctx, $1, $3); }

enumerator-list:
	enumerator											{ EnumeratorList list; list.push_back($1); $$=list; }	
	| enumerator-list COMMA enumerator					{ ($1).push_back($3); $$=$1; }
	;

enumerator:
	enumeration-constant								{ $$=EnumElement($1, @1); }
	| enumeration-constant ASGN constant-expression		{ $$=EnumElement($1, $3, @1); }
	;

enumeration-constant:
	identifier
	;

identifier:
	IDF
	;
	
string-literal:
	STRLIT
	;

constant:
	INTLIT		{ $$=ctx->get_int32_constant($1); }
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
	function-definition-body { _enter_function(ctx, $1, @1); } function-definition-tail
	;

function-definition-body:
	declaration-specifiers declarator	{ ($2).add($1); $$=$2; }
	;

function-definition-tail:
	LCUR block-item-list exit-function RCUR
	| LCUR exit-function RCUR
	;

exit-function:
	%empty 	{ ctx->exit_function();}
	;

declaration:
	no-leading-attribute-declaration
	;

no-leading-attribute-declaration:
	no-leading-attribute-declaration-core SEMIC  
	| declaration-specifiers  SEMIC
	;

no-leading-attribute-declaration-core: 
	declaration-specifiers init-declarator-list 	{ casem::process_declarations(ctx, $1, $2, @1); }
	;

declaration-specifiers:
	declaration-specifier declaration-specifiers 	{ ($2).add($1); $$=$2; }
	| declaration-specifier 						{ $$=$1; }
	;

declaration-specifier:
	storage-class-specifier							{ $$=DeclaratorTypeFragment(ctx, $1); $$.location = @1; }
	| type-specifier-qualifier						{ $$=$1; }
	;

init-declarator-list:
	init-declarator 								{ 	casem::DeclaratorTypeFragmentList list;
														list.push_back($1);
														$$=list; }
	| init-declarator-list COMMA init-declarator	{	($1).push_back($3);
														$$=$1; }
	;

init-declarator:
	declarator										{ $$=$1; }
	;

storage-class-specifier:
	TYPEDEF											{ $$=TYPE_DEF;  }
	;

type-specifier:
	VOID						{ $$=casem::pack_type(ctx->get_void_type()); }
	| ETYPE						{ $$=casem::pack_type(casem::get_etype_type(ctx, $1)); }
	| struct-or-union-specifier	{ $$=casem::pack_type($1); }
	| enum-specifier			{ $$=casem::pack_type($1); }
	| typedef-name				{ $$=($1)->get_type_pack();  }
	;


specifier-qualifier-list:
	type-specifier-qualifier								{ $$=$1; }
	| type-specifier-qualifier specifier-qualifier-list		{ ($2).add($1); $$=$2; }
	;

type-specifier-qualifier:
	type-specifier		{ $$=casem::DeclaratorTypeFragment(ctx, $1); $$.location = @1;}
	| type-qualifier	{ $$=casem::DeclaratorTypeFragment(ctx, $1); $$.location = @1; }
	;



type-qualifier:
	CONST						{ $$=CONSTANT; }							
	;

declarator:
	pointer direct-declarator	{ ($2).add($1);  $$=$2; }
	| direct-declarator			{ $$=$1; }
	;

direct-declarator:
	identifier 					  { $$ = DeclaratorTypeFragment(ctx); 
									$$.name=$1;
									$$.location = @1; }
	| LPAR declarator RPAR      { $$=$2; }
	| array-declarator			{ $$=$1; }
	| function-declarator		{ $$=$1; }
	;

array-declarator:
	direct-declarator LBRA assignment-expression RBRA 	{ ($1).array($3); $$=$1; }
	;

function-declarator:
	direct-declarator LPAR parameter-type-list RPAR   { ($1).function($3); $$=$1; }
	;

pointer:
	STAR type-qualifier-list 			{ ($2).pointer(); $$=$2; }
	| STAR 								{ $$=DeclaratorTypeFragment(ctx); $$.pointer(); $$.location=@1;  }				
	| STAR type-qualifier-list pointer	{ ($3).add($2); ($3).pointer(); $$=$3; }
	| STAR pointer						{ ($2).pointer(); $$=$2; }
	;

type-qualifier-list:
	type-qualifier								{ $$=DeclaratorTypeFragment(ctx, $1); $$.location = @1; }
	| type-qualifier-list type-qualifier		{ ($1).add_qualifier($2); $$=$1; }
	;

parameter-type-list:
	parameter-list									{ $$=$1; }
	;

parameter-list:
	parameter-declaration							{ casem::DeclaratorTypeFragmentList list; list.push_back($1); $$=list; }
	| parameter-list COMMA parameter-declaration 	{ ($3).location=@3; ($1).push_back($3); $$=$1; }
	;

parameter-declaration:
	declaration-specifiers declarator				{ ($2).add($1); $$=$2; }
	| declaration-specifiers abstract-declarator	{ ($2).add($1); $$=$2;  }
	| declaration-specifiers						{ $$=$1; }
	;

type-name:
	specifier-qualifier-list abstract-declarator
	| specifier-qualifier-list
	;

abstract-declarator:
	pointer									{ $$=$1; }
	| pointer direct-abstract-declarator	{ ($2).add($1); $$=$2; }
	|         direct-abstract-declarator	{ $$=$1; }
	;

direct-abstract-declarator:
	LPAR abstract-declarator RPAR	{ $$=$2; }
	| array-abstract-declarator		{ /*TODO*/ $$=DeclaratorTypeFragment(); }
	| function-abstract-declarator	{ $$=$1; }
	;

array-abstract-declarator:
	direct-abstract-declarator LBRA assignment-expression RBRA
	| LBRA assignment-expression RBRA
	;

function-abstract-declarator:
	direct-abstract-declarator LPAR parameter-type-list RPAR { ($1).function($3); $$=$1; }
	|                          LPAR parameter-type-list RPAR {  $$=DeclaratorTypeFragment(ctx); $$.location = @1; }
	;

typedef-name:
	TYPEIDF					{ $$=ctx->find_typedef($1); }
	;

compound-statement:
	LCUR { ctx->enter_block(); } block-item-list { ctx->exit_block(); } RCUR
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
	RETURN SEMIC 							{ ctx->builder()->CreateRetVoid();}
	| RETURN assignment-expression SEMIC 	{ ctx->builder()->CreateRet($2);}
	;

primary-expression:
	identifier		             { $$=ctx->get_int32_constant(42);}
	| constant					 { $$=$1;}
	| string-literal			 { $$=ctx->get_int32_constant(42);}
	| LPAR expression RPAR       { $$=$2;}
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
	postfix-expression					{ $$=$1;}
	| INCDEC unary-expression			{ $$=$2; }
	| unary-operator cast-expression	{ $$=$2;}
	| SIZEOF LPAR type-name RPAR		{ $$=ctx->get_int32_constant(42);}
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
	logical-OR-expression											{ $$=$1;}
	| unary-expression assignment-operator assignment-expression	{ $$=ctx->get_int32_constant(42);}
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
	logical-OR-expression			{ $$=$1;}
	;

/////////////////////////////////

%%

namespace cecko {

	void parser::error(const location_type& l, const std::string& m)
	{
		ctx->message(cecko::errors::SYNTAX, l, m);
	}

}
