%pure-parser
%error-verbose
%parse-param { IParser& parser }
  //%lex-param   { Parser& parser }

%{
#include <stdio.h>
#include "iparser.h"
int yyerror(IParser& parser, char *);
%}

%union {
  bool        b;
  const char *str;
}

%token	TSolution TFolder TProject 
%token	TFilter TFile TConfiguration
%token	<str> TName
%token  <b> TBool
%type   <str> name
%destructor { free ((void*)$$); } TName
%%

input:      /* empty */
           | defs
           ;
defs       : def
           | defs def
           ;
def        : solution      
           | project    
           | obj_cnt
           ;
           
solution   : '<' TSolution name '>' '\n' { parser.AddObj(0, $3); }
                solution_c
             '<' '/' TSolution '>' '\n' { parser.End(); }
           | '<' TSolution name '>' '\n'
             '<' '/' TSolution '>' '\n' { parser.End(); }
           ;
solution_c : solution_d
           | solution_c solution_d
           ;
solution_d : folder
           | project
           | obj_cnt
           ;
           
folder     : '<' TFolder name '>' '\n' { parser.AddObj(0, $3); }
                folder_c
             '<' '/' TFolder '>' '\n' { parser.End(); }
           | '<' TFolder name '>' '\n'
             '<' '/' TFolder '>' '\n' { parser.AddObj(0, $3);parser.End(); }
           ;
folder_c   : folder_d
           | folder_c folder_d
           ;
folder_d   : obj_cnt
           | folder
           | project
           ;
           
project    : '<' TProject name '>' '\n' { parser.AddObj(0, $3); }
                project_c
             '<' '/' TProject '>' '\n' { parser.End(); }
           | '<' TProject name '>' '\n'
             '<' '/' TProject '>' '\n' { parser.AddObj(0, $3);parser.End(); }
           ;
project_c  : project_d
           | project_c project_d
           ;
project_d  : obj_cnt
           | filter
           | file
           ;
           
filter     : '<' TFilter name '>' '\n' { parser.AddObj(0, $3); }
                filter_c
             '<' '/' TFilter '>' '\n' { parser.End(); }
           | '<' TFilter name '>' '\n' 
             '<' '/' TFilter '>' '\n' { parser.AddObj(0, $3);parser.End(); }
           ;
filter_c   : filter_d
           | filter_c filter_d
           ;
filter_d   : obj_cnt
           | filter
           | file
           ;
           
file       : '<' TFile name '>' '\n' { parser.AddObj(0, $3); }
                file_c
             '<' '/' TFile '>' '\n' { parser.End(); }
           ;
file_c     :
		   | file_l
           ;
file_l     : file_d
           | file_l file_d
           ;
file_d     : obj_cnt
           ;
           
conf       : '<' TConfiguration name '>' '\n' { parser.AddObj(0, $3); }
                conf_c
             '<' '/' TConfiguration '>' '\n' { parser.End(); }
           | '<' TConfiguration name '>' '\n' { parser.AddObj(0, $3); }
             '<' '/' TConfiguration '>' '\n' { parser.End(); }
           ;
conf_c     : conf_d
           | conf_c conf_d
           ;
conf_d     : cnt
           ;
           
obj_cnt    : conf
           | cnt
           ;
cnt        : '\n'
           | cmd
           | option
           ;   
cmd        : TName { parser.Cmd($1); parser.End(); }
           | TName { parser.Cmd($1); } args { parser.End(); }
           ;
option     : TName '=' { parser.Option($1); } arg { parser.End(); }
           ;
args       : arg
           | args arg
           ;
arg        : TName       { parser.Arg($1); }
           | TBool       { parser.Arg($1); }
           ;
name       : TName       { $$ = $1; }
           ;

%%

int yyerror(IParser& parser, char *chybka)
{
  printf("%d: %s\n", yylineno, chybka);
  return 0;
}


