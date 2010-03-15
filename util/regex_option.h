
struct Option
{
    unsigned int flags;
    char one_char;
    const char* long_name;
    const char* help_text;
	int data;
};

enum
{
	OptMask        = 0xff00,
    OptHelp        = 0x0100,
    OptVersion     = 0x0200,
	OptCompile     = 0x0300,
	OptFormat      = 0x0400,
	OptCompileFlag = 0x1000,
	OptCompileNL   = 0x2000,
	OptOutput      = 0x3000,
	OptHeader      = 0x4000,
	OptSource      = 0x5000,
	OptCPlusPlus   = 0x6000,
	OptType        = 0xe000,
	OptPrefix      = 0x7000,
	OptNamespace   = 0x8000,
	OptExports     = 0x9000,
	OptComment     = 0xa000,
	OptVerbose     = 0xb000,
	OptWarning     = 0xc000,
	OptPerformace  = 0xd000,
    OptSkeleton    = 0xf000,

	ParamString   = 0x10000,
    ParamNumber   = 0x20000,
};

extern const char* category[];

extern Option options[];

void usage(void);
void help(void);

const Option& long_opt(const char* str);
const Option& short_opt(char c);

