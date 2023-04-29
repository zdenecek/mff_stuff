%{

// allow access to YY_DECL macro
#include "ckbisonflex.hpp"
#include "casem.hpp"


#include INCLUDE_WRAP(BISON_HEADER)

%}

/* NEVER SET %option outfile INTERNALLY - SHALL BE SET BY CMAKE */

%option noyywrap nounput noinput
%option batch never-interactive reentrant
%option nounistd 

/* AVOID backup perf-report - DO NOT CREATE UNMANAGEABLE BYPRODUCT FILES */


DIGIT		 	[0-9]
HEXDIGIT		[0-9a-fA-F]
ID_CHAR			[_a-zA-Z0-9]
ID				[_a-zA-Z]{ID_CHAR}*
NONZERODIGIT	[1-9]	

DECNUM			{DIGIT}+{ID_CHAR}*
HEXNUM			0[xX]{ID_CHAR}*


%x string
%x comment
%x m_comment
%x character
%%

%{
int inline_counter = 0;
bool too_long;
bool malformed;
%}

break		return cecko::parser::make_BREAK(ctx->line());
const		return cecko::parser::make_CONST(ctx->line());
continue	return cecko::parser::make_CONTINUE(ctx->line());
do			return cecko::parser::make_DO(ctx->line());
else		return cecko::parser::make_ELSE(ctx->line());
enum		return cecko::parser::make_ENUM(ctx->line());
for			return cecko::parser::make_FOR(ctx->line());
goto		return cecko::parser::make_GOTO(ctx->line());
if			return cecko::parser::make_IF(ctx->line());
return		return cecko::parser::make_RETURN(ctx->line());
sizeof		return cecko::parser::make_SIZEOF(ctx->line());
struct		return cecko::parser::make_STRUCT(ctx->line());
typedef		return cecko::parser::make_TYPEDEF(ctx->line());
void		return cecko::parser::make_VOID(ctx->line());
while		return cecko::parser::make_WHILE(ctx->line());
"char"		return cecko::parser::make_ETYPE(cecko::gt_etype::CHAR, ctx->line());
int			return cecko::parser::make_ETYPE(cecko::gt_etype::INT, ctx->line());
_Bool		return cecko::parser::make_ETYPE(cecko::gt_etype::BOOL, ctx->line());

{ID}		{
				if(ctx->is_typedef(yytext))
					return cecko::parser::make_TYPEIDF(yytext, ctx->line());
				else
					return cecko::parser::make_IDF(yytext, ctx->line());
			}


{HEXNUM}+	{
				auto r = cecko::parser::make_INTLIT(casem::parse_int(yytext + 2, 16, malformed, too_long), ctx->line());
				if(malformed) ctx->message(cecko::errors::BADINT, ctx->line(), yytext);
				if(too_long) ctx->message(cecko::errors::INTOUTRANGE, ctx->line(), yytext);
				return r;
			}
{DECNUM}+	{
				auto r = cecko::parser::make_INTLIT(casem::parse_int(yytext ,10,  malformed, too_long ), ctx->line());
				if(malformed) ctx->message(cecko::errors::BADINT, ctx->line(), yytext);
				if(too_long) ctx->message(cecko::errors::INTOUTRANGE, ctx->line(), yytext);
				return r;
			}

\/\/				BEGIN(comment);
<comment>\n			ctx->incline(); BEGIN(INITIAL); 
<comment>.  		;
<comment><<EOF>>	BEGIN(INITIAL);

\/\*				inline_counter++; BEGIN(m_comment);  
<m_comment>\/\*		inline_counter++;
<m_comment>\*\/		inline_counter--; if(inline_counter == 0) BEGIN(INITIAL); 
<m_comment>\n		ctx->incline();		
<m_comment>.		;	
<m_comment><<EOF>>	{
						ctx->message(cecko::errors::EOFINCMT, ctx->line());
						BEGIN(INITIAL);	
					}
\*\/				ctx->message(cecko::errors::UNEXPENDCMT, ctx->line()); 


<character,string>\\[abfnrtv] 		casem::append_escaped_special(yytext[1]);
<character,string>\\[\'\"\?\\] 		casem::append(yytext[1]);
<character,string>\\x{HEXDIGIT}+	{
										if(std::strlen(yytext + 2) > 2) ctx->message(cecko::errors::BADESCAPE, ctx->line(), yytext);  
										casem::append_escaped_hex(yytext + 2);		
									}
<character,string>\\.				{
										ctx->message(cecko::errors::BADESCAPE, ctx->line(), yytext);  
										casem::append(yytext[1]);
									}
<character,string>\\				ctx->message(cecko::errors::BADESCAPE, ctx->line(), yytext);  

\'\'				{
						ctx->message(cecko::errors::EMPTYCHAR, ctx->line());
						return cecko::parser::make_INTLIT(0, ctx->line());
					}
\'					BEGIN(character); casem::char_start();
<character>\' 		BEGIN(INITIAL); return cecko::parser::make_INTLIT(casem::char_end(), ctx->line());
<character>\n		{
						BEGIN(INITIAL); 
						if(casem::is_char_too_long()) ctx->message(cecko::errors::MULTICHAR_LONG, ctx->line());
						ctx->message(cecko::errors::EOLINSTRCHR, ctx->line());
						ctx->incline();
						return cecko::parser::make_INTLIT(casem::char_end(), ctx->line() - 1);
					}	
<character><<EOF>>	{
						ctx->message(cecko::errors::EOFINSTRCHR, ctx->line()); 
						BEGIN(INITIAL);
						return cecko::parser::make_INTLIT(casem::char_end(), ctx->line());
					}
<character>.		casem::append(yytext[0]);

\"				BEGIN(string); casem::string_start();
<string>\"		BEGIN(INITIAL); return cecko::parser::make_STRLIT(casem::string_end(), ctx->line());
<string>\n		{
					BEGIN(INITIAL); 
					ctx->message(cecko::errors::EOLINSTRCHR, ctx->line()); 
					ctx->incline(); 
					return cecko::parser::make_STRLIT(casem::string_end(), ctx->line() - 1 );
				}
<string>.		casem::append(yytext[0]);
<string><<EOF>>	{
					ctx->message(cecko::errors::EOFINSTRCHR, ctx->line()); 
					BEGIN(INITIAL);
					return cecko::parser::make_STRLIT(casem::string_end(), ctx->line());
				}

\[ 				return cecko::parser::make_LBRA(ctx->line());
\] 				return cecko::parser::make_RBRA(ctx->line());
\( 				return cecko::parser::make_LPAR(ctx->line());
\) 				return cecko::parser::make_RPAR(ctx->line());
\{ 				return cecko::parser::make_LCUR(ctx->line());
\} 				return cecko::parser::make_RCUR(ctx->line());
\. 				return cecko::parser::make_DOT(ctx->line());
-> 				return cecko::parser::make_ARROW(ctx->line());
\+\+ 			return cecko::parser::make_INCDEC(cecko::gt_incdec::INC, ctx->line());
-- 				return cecko::parser::make_INCDEC(cecko::gt_incdec::DEC, ctx->line());
& 				return cecko::parser::make_AMP( ctx->line());
\* 				return cecko::parser::make_STAR( ctx->line());
\+ 				return cecko::parser::make_ADDOP(cecko::gt_addop::ADD, ctx->line());
- 				return cecko::parser::make_ADDOP(cecko::gt_addop::SUB, ctx->line());
! 				return cecko::parser::make_EMPH( ctx->line());
\/ 				return cecko::parser::make_DIVOP(cecko::gt_divop::DIV, ctx->line());
\% 				return cecko::parser::make_DIVOP(cecko::gt_divop::MOD, ctx->line());
\< 				return cecko::parser::make_CMPO(cecko::gt_cmpo::LT, ctx->line());
> 				return cecko::parser::make_CMPO(cecko::gt_cmpo::GT, ctx->line());
\<= 			return cecko::parser::make_CMPO(cecko::gt_cmpo::LE, ctx->line());
>= 				return cecko::parser::make_CMPO(cecko::gt_cmpo::GE, ctx->line());
== 				return cecko::parser::make_CMPE(cecko::gt_cmpe::EQ, ctx->line());
!= 				return cecko::parser::make_CMPE(cecko::gt_cmpe::NE, ctx->line());
&& 				return cecko::parser::make_DAMP(ctx->line());
\|\| 			return cecko::parser::make_DVERT(ctx->line());
= 				return cecko::parser::make_ASGN(ctx->line());
; 				return cecko::parser::make_SEMIC(ctx->line());
: 				return cecko::parser::make_COLON(ctx->line());
, 				return cecko::parser::make_COMMA(ctx->line());
\*= 			return cecko::parser::make_CASS(cecko::gt_cass::MULA, ctx->line());
\/= 			return cecko::parser::make_CASS(cecko::gt_cass::DIVA, ctx->line());
\%= 			return cecko::parser::make_CASS(cecko::gt_cass::MODA, ctx->line());
\+= 			return cecko::parser::make_CASS(cecko::gt_cass::ADDA, ctx->line());
-= 				return cecko::parser::make_CASS(cecko::gt_cass::SUBA, ctx->line());

\n				ctx->incline();
[ \t]			;
.				ctx->message(cecko::errors::UNCHAR, ctx->line(), yytext);
<<EOF>>			return cecko::parser::make_EOF(ctx->line());

%%

namespace cecko {

	yyscan_t lexer_init(FILE * iff)
	{
		yyscan_t scanner;
		yylex_init(&scanner);
		yyset_in(iff, scanner);
		return scanner;
	}

	void lexer_shutdown(yyscan_t scanner)
	{
		yyset_in(nullptr, scanner);
		yylex_destroy(scanner);
	}

}
