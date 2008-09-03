%option yylineno
%option stack 
%{
#include <stdlib.h>
#include <string.h>
#include "iparser.h"
#include "parser.tab.hpp"
#include "parser.h"

inline int yywrap() { return 1; } 
#define YY_SKIP_YYWRAP 1

// helpers
namespace {
void SetString(xcstring& str, 
               const char* text,
               xcstring::Type type=xcstring::String)
{
	str.type = type;
	str.str = strdup((const char*)yytext);
	//printf("lex: '%s'\n", yytext);
}

void ParseHex(xcnumber& num, const char* text)
{
}
void ParseNumber(xcnumber& num, const char* text)
{
}
void ParseReal(xcnumber& num, const char* text)
{
}

}

#define STR(type) SetString (yyv->s, (const char*)yytext, xcstring::##type)
#define NUM(type) Parse ## type (yyv->n, (const char*)yytext)
%}

ws       [ \t\r]
comment	 #.*
alpha    [A-Za-z_]
spec     [\-0-9]
dig      [-+]?[0-9]+
hex	 	 "0x"[0-9A-Fa-f]+
name     {alpha}({alpha}|{spec})*
cart     [^/\"\<\>\n*?\ =]|"\\".
wcart    {cart}|([\*\?])
path     {cart}({cart}|\/)*
wild     {wcart}({wcart}|\/)*
real1    [-+]?{dig}+\.({dig}+)?f?
real2    [-+]?{dig}*\.{dig}+f?
real	 {real1}|{real2}
string   \"([^\"\n]|(\\.))*\"

%x  TAG

%%
{comment}	return '\n';
<*>{ws}+	/* ignored */

"<"			yy_push_state (TAG); return '<';
"On"        yyv->b = true; return TBool;
"Off"       yyv->b = false; return TBool;
"true"      yyv->b = true; return TBool;
"false"     yyv->b = false; return TBool;

<TAG>">"		   yy_pop_state (); return '>';
<TAG>"Solution"	   return TSolution;
<TAG>"Folder"	   return TFolder;
<TAG>"Project"	   return TProject;
<TAG>"Filter"	   return TFilter;
<TAG>"File"		   return TFile;
<TAG>"Conf"		   return TConfiguration;
<TAG>{name}		   STR(String); return TName;
<TAG>{path}        STR(Path); return TPath;
<TAG>{ws}"/"{path} SetString (yyv->s, (const char*)yytext+1, xcstring::Path); 
                   return TPath;
<TAG>{wild}        STR(Wildcart); return TWildcart;
<TAG>{ws}"/"{wild} SetString (yyv->s, (const char*)yytext+1, xcstring::Wildcart);
                   return TWildcart;
<TAG>.			   return (int) (yytext[0]);

{real}      NUM(Real); return TReal;
{hex}       NUM(Hex); return TNumber;
{dig}       NUM(Number); return TNumber;
{name}		STR(String); return TName;
"/"?{path}  STR(Path); return TPath;
"/"?{wild}  STR(Wildcart); return TWildcart;

<*>{string}  STR(String); return TString;
<*>${string} STR(Regex);  return TRegex;
<*>@{string} STR(Wildcart); return TWildcart;

\n			return '\n';
.			return (int) (yytext[0]);
%%

bool OpenFlex(const char* path)
{
	FILE* f = fopen(path, "r");
	if (!f) return false;
	yyrestart(f);
	return true;
}

void CloseFlex()
{
}
