%option noyywrap

%{

#include <string.h>
#include "struct.h"
#include "collection.h"
#include "prf.h"
#include "parser.tab.h"


    /* Variables declared in the lexer for error tracking */
    extern int line_num;
    //extern int num_error;

    /* extern declaration of function yyerror */
    extern int yyerror(const char* errmsg);

%}
    /*=========================================================================
      TOKEN DEFINITIONS
      =========================================================================*/
DIGIT    [0-9]
ID       [a-zA-Z_][a-zA-Z0-9_]*


    /*=========================================================================
      TOKENS
      =========================================================================*/

%option noyywrap


%%

"\r\n"            { ++line_num; }
"\n"              { ++line_num; }
[ \t\f\v]+        { /* Ignore whitespace. */ }


"("		  { return LPAR; }
")"		  { return RPAR; }
"["               { return LSQUARE; }
"]"               { return RSQUARE; }
";"               { return SEMI; }
"*"               { return ALL; }
","               { return COMMA; }
"set"              { return SET; }
"A"            { return A; }
"show"             { return SHOW; }
"check"             { return CHECK; }
"RECTANGLE" { yylval.access_type=RECTANGLE; return ACC_TYPE;}
"TRANSP_RECTANGLE" { yylval.access_type=TRANSP_RECTANGLE; return ACC_TYPE;}
"ROW" { yylval.access_type=ROW; return ACC_TYPE;}
"COLUMN" { yylval.access_type=COLUMN; return ACC_TYPE;}
"MAIN_DIAG" { yylval.access_type=MAIN_DIAG; return ACC_TYPE;}
"SECONDARY_DIAG" { yylval.access_type=SECONDARY_DIAG; return ACC_TYPE;}
{ID}              { yylval.svalue=strdup(yytext); return IDENTIFIER; }
{DIGIT}+          { yylval.intval = atoi( yytext );
    return(NUMBER); }

.                 { yyerror("Error: unexpected token");
	//num_error++;
	return (-1); /* invalid token */
    }
