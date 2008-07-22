#pragma once

// stavovy parser
class IParser
{
public:
	virtual void AddObj(int type, const char* name) = 0;
	virtual void Option(const char* name) = 0;
	virtual void Cmd(const char* name) = 0;
	virtual void Arg(bool b) = 0;
	virtual void Arg(const char* s) = 0;

	virtual void End() = 0;
};

int yyparse(IParser& parser);
bool OpenFlex(const char* path);
void CloseFlex();

union YYSTYPE;
#define YY_DECL int yylex(union YYSTYPE* val)
YY_DECL;
extern int yylineno;


