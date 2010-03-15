%pure-parser
%error-verbose
%parse-param { IParser& parser }
  //%lex-param   { Parser& parser }
%glr-parser

%{
#include <stdio.h>
#include "iparser.h"
int yyerror(IParser& parser, const char*);
%}

%union {
  bool b;
  xcstring s;
  xcnumber n;
}

%token	TSolution TFolder TProject 
%token	TFilter TFile TConfiguration
%token	<b> TBool
%token  <n> TNumber TReal 
%token  <s> TName TString TRegex TWildcart TPath
%type   <s> name 
%destructor { printf("d");free ((void*)$$.str); } TName TString TRegex TWildcart TPath name
%%

input      : defs
           ;
defs       : def
           | defs def
           ;
def        : solution      
           | project    
           | obj_cnt
           ;
           
solution   : solution_b solution_c solution_e
           | solution_b solution_e
solution_b : '<' TSolution name '>' '\n' { parser.AddObj(0, $3); }
solution_e : '<' '/' TSolution '>' '\n' { parser.End(); }
solution_c : solution_d
           | solution_c solution_d
solution_d : folder
           | project
           | obj_cnt
           ;
           
folder     : folder_b folder_c folder_e
           | folder_b folder_e
folder_b   : '<' TFolder name '>' '\n' { parser.AddObj(0, $3); }
folder_e   : '<' '/' TFolder '>' '\n' { parser.End(); }
folder_c   : folder_d
           | folder_c folder_d
folder_d   : obj_cnt
           | folder
           | project
           ;
           
project    : project_b project_c project_e
           | project_b project_e
           ;
project_b  : '<' TProject name '>' '\n' { parser.AddObj(0, $3); }
project_e  : '<' '/' TProject '>' '\n' { parser.End(); }
project_c  : project_d
           | project_c project_d
           ;
project_d  : obj_cnt
           | filter
           | file
           ;
           
filter     : filter_b filter_c filter_e
           | filter_b filter_e
           ;
filter_b   : '<' TFilter name '>' '\n' { parser.AddObj(0, $3); }
filter_e   : '<' '/' TFilter '>' '\n' { parser.End(); }
filter_c   : filter_d
           | filter_c filter_d
           ;
filter_d   : obj_cnt
           | filter
           | file
           ;
           
file       : file_b file_l file_e 
           | file_b file_e
           ;
file_b     : '<' TFile name '>' '\n' { parser.AddObj(0, $3); }
file_e     : '<' '/' TFile '>' '\n' { parser.End(); }
file_l     : obj_cnt
           | file_l obj_cnt
           ;
           
conf       : conf_b conf_c conf_e
           | conf_b conf_e
           ;
conf_b     : '<' TConfiguration name '>' '\n' { parser.AddObj(0, $3); }
conf_e     : '<' '/' TConfiguration '>' '\n' { parser.End(); }
conf_c     : conf_d
           | conf_c conf_d
           ;
conf_d     : cnt
           ;
           
obj_cnt    : conf
           | cnt
           ;
cnt        : '\n'
           | cmd    '\n'
           | option '\n'
           | error { parser.Error(); }
           ;   
cmd        : TName { parser.Cmd(($1).str); parser.End(); }
           | TName { parser.Cmd(($1).str); } args { parser.End(); }
           ;
option     : TName '=' { parser.Option(($1).str); } arg { parser.End(); }
           ;
args       : arg
           | args arg
           ;
arg        : TBool       { parser.Arg($1); }
           | TNumber     { parser.Arg($1); }
           | TReal       { parser.Arg($1); }
           | name        { parser.Arg($1); }
           ;
name       : TName       { $$ = $1; }
           | TString     { $$ = $1; }
           | TPath       { $$ = $1; }
           | TRegex      { $$ = $1; }
           | TWildcart   { $$ = $1; }
           ;

%%

int yyerror(IParser& parser, const char* msg)
{
  printf("%d: %s\n", yylineno, msg);
  return 0;
}


