#pragma once

struct xcstring
{
	enum Type
	{
		String,
		Path,
		Wildcart,
		Regex,
	} type;
	const char* str;
};

struct xcnumber
{
	enum Type
	{
		Number,
		Real,
	} type;
	union {
	  long long n;
	  double r;
	};
};

// stavovy parser
class IParser
{
public:
	virtual void AddObj(int type, const xcstring& name) = 0;
	virtual void Option(const char* name) = 0;
	virtual void Cmd(const char* name) = 0;
	/// Arguments
	virtual void Arg(bool v) = 0;
	virtual void Arg(const xcstring& v) = 0;
	virtual void Arg(const xcnumber& n) = 0;

	virtual void End() = 0;
	/// Set to error state
	virtual void Error() = 0;
};

int yyparse(IParser& parser);
bool OpenFlex(const char* path);
void CloseFlex();

union YYSTYPE;
#define YY_DECL int yylex(union YYSTYPE* yyv)
YY_DECL;
extern int yylineno;


