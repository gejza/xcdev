%option yylineno
%option stack 
%{
#include <stdlib.h>
#include <string.h>
#include "parser.tab.hpp"
#include "parser.h"
extern "C"  static int yywrap() { return 1; }
%}

string  \"[^\n"]+\"
ws      [ \t\r]+
comment	#.*
alpha   [A-Za-z_]
awild   {alpha}|[*\?]
pwild   {awild}|[/.]
dig     [-+]?[0-9]+
hex		"0x"[0-9A-Fa-f]+
name    {alpha}({alpha}|{dig}|[\-$:])*
wild    {pwild}({pwild}|{dig}|[\-])*
real1    [-+]?{dig}+\.({dig}+)?f?
real2    [-+]?{dig}*\.{dig}+f?
real	 {real1}|{real2}

%x  TAG

%%
{comment}	return '\n';
<*>{ws}+	/* ignored */
"<"			yy_push_state (TAG); return '<';
"On"        val->b = true; return TBool;
"Off"       val->b = false; return TBool;
"true"      val->b = true; return TBool;
"false"     val->b = false; return TBool;

<TAG>">"		yy_pop_state (); return '>';
<TAG>"Solution"	return TSolution;
<TAG>"Folder"	return TFolder;
<TAG>"Project"	return TProject;
<TAG>"Filter"	return TFilter;
<TAG>"File"		return TFile;
<TAG>"Conf"		return TConfiguration;
<TAG>{name}		val->str = strdup((const char*)yytext); return TName;
<TAG>.			return (int) (yytext[0]);

{name}		val->str = strdup((const char*)yytext); return TName;
{wild}		val->str = strdup((const char*)yytext); return TName;
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
