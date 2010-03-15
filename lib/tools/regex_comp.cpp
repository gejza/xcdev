#include <stdio.h>
#include <cctype>
#include <string.h>
#include <string>
#include <algorithm>
#include <pcre.h>

#define NAME "regex-comp"
#define ID_FILE "_AUTO_REGEX_%s_H_"
#define DEFAULT_FILENAME "table.re"
#define DEFAULT_TYPE "char"
#define DEFAULT_PREFFIX ""
#define DEFAULT_DECL "static "
#define DEFAULT_H_SUFFIX ".h"
#define DEFAULT_HXX_SUFFIX ".h"
#define DEFAULT_C_SUFFIX ".c"
#define DEFAULT_CXX_SUFFIX ".cpp"

struct Option
{
    unsigned int flags;
    char one_char;
    const char* long_name;
    const char* help_text;
	int data;
};

struct FileConfig
{
	const char* decl;
	bool define;
	const char* header;
};

struct Config
{
	// document options
	bool cpp;
	bool comment;
	const char* ns;
	const char* prefix;
	const char* type;
	const char* filename;
	// used options
	const Option* opts[50];
	size_t nopts;
	// compile
	int compile_options;
	// files
	FileConfig head;
	std::string head_fn;
	FileConfig impl;
	std::string impl_fn;
    bool to_file;
    bool def_file;
	// supp
	bool verbose;
	bool nowarn;
	int max_string;
    bool compile;

	Config();
	int Args(int argc, const char* argv[]);
	void Configure();
	void Action(const Option& opt);
	void Action(const Option& opt, const char* param);
};

class Regex
{
    std::string name;
    std::string patt;
    int size;
    unsigned char* data;
	bool pointer_type;
public:
    Regex(const Config& config, const char* _name, const char* _patt);
    void print(FILE* out, const Config& config, const FileConfig& flags);
    void OutputData(FILE* out, bool to_string);
};

//////////////////////////////////////////////////////////

namespace {

/*************************************************
*                Usage function                  *
*************************************************/
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

	ParamString   = 0x10000,
};

const char* category[] =
{
    0,
    0, //"Miscellaneous", /* = 1 */
	"Regular expresions", /* = 2 */
    "Generated code",/* = 3 */
    "Files",         /* = 4 */
    "Debugging",     /* = 5 */
};

Option options[] = {
    { 1, 0, "", "terminate options" },
	{ OptHelp|1, '?', NULL, NULL },
    { OptHelp|1, 'h', "help", "display this help and exit" },
    { OptVersion|1, 'V', "version", "print version information and exit" },
    { OptCompile|1, 'c', "compile", "only compile regex and print to output" },
    { ParamString|OptFormat|1, 'f', "format=FORMAT", "format of output arrays (array or string)" },
    { OptCompileFlag|2, 0, "anchored", "force pattern anchoring", PCRE_ANCHORED },
	{ OptCompileFlag|2, 0, "auto-callout", "compile automatic callouts", PCRE_AUTO_CALLOUT },
	{ OptCompileFlag|2, 0, "bsr-anycrlf", "\\R matches only CR, LF, or CRLF", PCRE_BSR_ANYCRLF },
	{ OptCompileFlag|2, 0, "bsr-unicode", "\\R matches all Unicode line endings", PCRE_BSR_UNICODE },
	{ OptCompileFlag|2, 0, "caseless", "do caseless matching", PCRE_CASELESS },
	{ OptCompileFlag|2, 0, "dollar-endonly", "$ not to match newline at end", PCRE_DOLLAR_ENDONLY },
	{ OptCompileFlag|2, 0, "dotall", ". matches anything including NL", PCRE_DOTALL },
	{ OptCompileFlag|2, 0, "dupnames", "allow duplicate names for subpatterns", PCRE_DUPNAMES },
	{ OptCompileFlag|2, 0, "extended", "ignore whitespace and # comments", PCRE_EXTENDED },
	{ OptCompileFlag|2, 0, "firstline", "force matching to be before newline", PCRE_FIRSTLINE },
	{ OptCompileFlag|2, 0, "javascript-compat", "JavaScript compatibility", PCRE_JAVASCRIPT_COMPAT },
	{ OptCompileFlag|2, 0, "multiline", "^ and $ match newlines within data", PCRE_MULTILINE },
	{ ParamString|OptCompileNL|2, 0, "newline=SEQ", "set newline sequence (ANY, ANYCRLF, CR, CRLF, LF)" },
	{ OptCompileFlag|2, 0, "no-auto-capture", "disable numbered capturing paren-theses "
									"(named ones available)", PCRE_NO_AUTO_CAPTURE },
	{ OptCompileFlag|2, 0, "ungreedy", "invert greediness of quantifiers", PCRE_UNGREEDY },
	{ OptCompileFlag|2, 0, "utf8", "run in UTF-8 mode", PCRE_UTF8 },
	{ OptCompileFlag|2, 0, "no-utf8-check",
				"do not check the pattern for UTF-8 validity "
				"(only relevant if utf8 is set)", PCRE_NO_UTF8_CHECK },


    { OptCPlusPlus|3, '+', "c++", "generate C++ source code" },
    { ParamString|OptType|3, 't', "type=STRING", "set type of data (default: " DEFAULT_TYPE ")" },
    { ParamString|OptPrefix|3, 'P', "prefix=STRING", "use STRING as prefix instead of \""
        DEFAULT_PREFFIX "\"" },
    { ParamString|OptNamespace|3, 'N', "namespace=STRING", "declare data in namespace STRING" },
    { OptExports|3, 'E', "export-symbols", "define symbols with __declspec(dllexport)" },
    { OptComment|3, 'C', "nocomment", "do not include comments" },
    { ParamString|OptOutput|4, 'o', "outfile=FILE", "specify output filename (default: \"" DEFAULT_FILENAME "\" )" },
    { ParamString|OptHeader|4, 0, "header-file=FILE", "set C/C++ header file name" },
    { ParamString|OptSource|4, 0, "source-file=FILE", "create a C/C++ file for definition tables" },
    //{ OptDebug|5, 'd', "debug", "enable debug mode" },
    { OptPerformace|5, 'p', "perf-report", "write performance report to stderr" },
    { OptWarning|5, 'w', "nowarn", "do not generate warnings" },
    { OptVerbose|5, 'v', "verbose", "write summary of scanner statistics to stdout" },
    { 0 },
};

void version(void)
{
	fprintf(stderr, "Compiler PCRE version%s\n", pcre_version());
	exit(1);
}

void usage(void)
{
	fprintf(stderr, "Usage: regex-com [-c] [-");
    for (Option* op = options; op->flags != 0; op++)
    {
        if (op->one_char && op->one_char != 'c' && !(op->flags & ParamString))
            fprintf(stderr, "%c", op->one_char);
    }
    fprintf(stderr, "]");
    /*for (Option* op = options; op->flags != 0; op++)
    {
        if (op->one_char && (op->flags & ParamString))
            fprintf(stderr, " [-%c<name>]", op->one_char);
    }*/
    fprintf(stderr, " [-o<name>] [-f<format>] [long options] [FILE|PATTERN1 PATTERN2 ...]\n");
    fprintf(stderr, "Type `" NAME " --help' for more information and the long "
  "options.\n");
	exit(1);
}

/*************************************************
*                Help function                   *
*************************************************/
void help(void)
{
	printf("Usage: " NAME " [-c] [OPTION]... [FILE|PATTERN1 PATTERN2 ...]\n");
	printf("Search for PATTERN in each FILE or standard input.\n");
	printf("PATTERN must be present if neither -e nor -f is used.\n");
	printf("\"-\" can be used as a file name to mean STDIN.\n");

	printf("Example: " NAME " matches.txt\n");
	printf("         " NAME " -c '^(b+?|a){1,2}?c'\n\n");
	printf("Options:\n");

    int last_catg = 0;
	for (Option* op = options; op->flags != 0; op++)
    {
	    int n;
        int catg = op->flags & 0xf;
        if (last_catg != catg && category[catg])
            printf("\n%s:\n", category[catg]);
	    if (op->one_char > 0)
            printf("  -%c%c", op->one_char, op->long_name ? ',':' ');
        else
            printf("     ");
        if (op->long_name)
            n = 28 - printf(" --%s", op->long_name);
        else
            n = 28;
	    if (n < 1) n = 1;
        if (op->help_text)
            printf("%.*s%s", n, "                                 ", op->help_text);
        printf("\n");
        last_catg = catg;
	}
	printf("\nCompile on PCRE (Perl-compatible regular expression library) version %s\n", pcre_version());

    printf("\nDefault filenames:\n");
    printf("ANSI C: " DEFAULT_FILENAME DEFAULT_H_SUFFIX ", ");
    printf(DEFAULT_FILENAME DEFAULT_C_SUFFIX "\n");
    printf("C++:    " DEFAULT_FILENAME DEFAULT_HXX_SUFFIX ", ");
    printf(DEFAULT_FILENAME DEFAULT_CXX_SUFFIX "\n");

	printf("\nWhen reading patterns from a file instead of using a command line option,\n");
	printf("trailing white space is removed and blank lines are ignored.\n");

	printf("\nWith no FILEs, read standard input. If fewer than two FILEs given, assume -h.\n");
	printf("Exit status is 0 if any matches, 1 if no matches, and 2 if trouble.\n");

	exit(1);
}

bool eq_to_eq(const char* s1, const char* s2)
{
	while (1)
	{
		if (*s1 != *s2)
			return false;
		if (!*s1 || *s1 == '=')
			return true;
		s1++, s2++;
	}
}

const Option& long_opt(const char* str)
{
	for (Option* op = options; op->flags != 0; op++)
    {
	    if (op->long_name)
		{
			if (op->flags & ParamString)
			{
				if (eq_to_eq(op->long_name, str))
					return *op;
			}
			else if (strcmp(op->long_name, str) == 0)
				return *op;
		}
	}
	fprintf(stderr, "ERROR: Unknown option '--%s'\n", str);
	usage();
	return options[0];
}

const Option& short_opt(char c)
{
	for (Option* op = options; op->flags != 0; op++)
    {
	    if (op->one_char == c)
			return *op;
	}
	fprintf(stderr, "ERROR: Unknown option '-%c'\n", c);
	usage();
	return options[0];
}

#ifndef _WIN32 
#include <sys/stat.h>
/************* Test for regular file in Unix **********/
bool isregfile(const char *filename)
{
	struct stat statbuf;
	if (stat(filename, &statbuf) < 0)
		return 1;        /* In the expectation that opening as a file will fail */
	return (statbuf.st_mode & S_IFMT) == S_IFREG;
}
/************* Test stdout for being a terminal in Unix **********/
bool is_stdout_tty(void)
{
    return isatty(fileno(stdout));
}

#elif 0

/************* Test for regular file in Win32 **********/

int
isdirectory(char *filename)
{
DWORD attr = GetFileAttributes(filename);
if (attr == INVALID_FILE_ATTRIBUTES)
  return 0;
return ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0) ? '/' : 0;
} 

/* I don't know how to do this, or if it can be done; assume all paths are
regular if they are not directories. */

int isregfile(char *filename)
{
return !isdirectory(filename);
}


/************* Test stdout for being a terminal in Win32 **********/

/* I don't know how to do this; assume never */

static BOOL
is_stdout_tty(void)
{
return FALSE;
} 

#endif

#if 0 //ndef HAVE_STRERROR
/*************************************************
*     Provide strerror() for non-ANSI libraries  *
*************************************************/

/* Some old-fashioned systems still around (e.g. SunOS4) don't have strerror()
in their libraries, but can provide the same facility by this simple
alternative function. */

extern int   sys_nerr;
extern char *sys_errlist[];

char * strerror(int n)
{
	if (n < 0 || n >= sys_nerr) return "unknown error number";
	return sys_errlist[n];
}
#endif /* HAVE_STRERROR */ 

///////////////////////////////////////////////
// print contrl code

void print_sec(FILE* out)
{
    fprintf(out,
        "\n#if (defined (PCRE_MAJOR) && PCRE_MAJOR != %d) \\\n"
        "    || (defined (PCRE_MINOR) && PCRE_MINOR != %d)\n"
        "#  error \"Incompatible version of PCRE v%d.%d for predcompiled regexp. Try recompile it.\"\n"
        "#endif\n\n", PCRE_MAJOR, PCRE_MINOR, PCRE_MAJOR, PCRE_MINOR);
}

void print_info(FILE* out, const Config& config)
{
    fprintf(out,"\n/*****************************************\n");
    fprintf(out, " * PCRE ver%s\n", pcre_version());
    fprintf(out, " * Compiled options:\n");
	for (size_t i=0;i < config.nopts;i++)
		fprintf(out, " * #%s   (%s)\n", config.opts[i]->long_name, config.opts[i]->help_text); 
    fprintf(out," *\n *****************************************/\n\n");
}

////////////////////////////////////////////////////////

void print_hex(FILE* out, const unsigned char* d, int size)
{
    size_t col = 0;
    for (int i = 0;i < size; i++)
    {
        if (i)
        {
            if (col > 10)
            {
                col = 0;
                fprintf(out, ",\n    ");
            }
            else
                fprintf(out, ", ");
        }
        col++;
        fprintf(out, "%3d", (int)d[i]);
    }
}

void print_str(FILE* out, const unsigned char* d, int size)
{
    bool last_n = false;
    for (int i = 0;i < size; i++)
    {
        if (d[i] < 128 && isprint(d[i])
                && !isdigit(d[i]))
        {
            switch (d[i])
            {
            case '%':
                fprintf(out, "%%%%"); break;
            case '\\':
                fprintf(out, "\\\\"); break;
            case '\"':
                fprintf(out, "\\\""); break;
            default:
                fprintf(out, "%c", d[i]); break;
            };
            last_n = false;
        }
        else
        {
            fprintf(out, "\\%d", (int)d[i]);
            last_n = true;
        }
    }
}

}

////////////////////////////////////////////

Regex::Regex(const Config& config, const char* _name, const char* _patt)
    : name(_name), patt(_patt)
{
    const char* err;
    int eoff;

	if (config.verbose)
		fprintf(stderr, "Compiling %s pattern with flags %x\n", _name, config.compile_options);
    pcre* re = pcre_compile(patt.c_str(), config.compile_options, &err, &eoff, NULL);
    if (!re)
    {
		fprintf(stderr, "ERROR: Error with compiling '%s' at column %d: %s\n", patt.c_str(), eoff, err);
        exit(1);
    }
    int rc = pcre_fullinfo(re, NULL, PCRE_INFO_SIZE, &size);
    if (rc < 0)
    {
        exit(2);
    }
    data = (unsigned char*)re;
	pointer_type = size < config.max_string;
	if (config.verbose)
		fprintf(stderr, "Bytecode for regex %s has size %d, select as %s\n", _name, size, pointer_type ? "pointer":"array");
}

void Regex::print(FILE* out, const Config& config, const FileConfig& flags)
{
    if (config.comment)
    {
        if (patt.size() > 80)
        {
            fprintf(out, "// ");
            fwrite(patt.c_str(), 1, 77, out);
            fprintf(out, "...\n");
        }
        else
            fprintf(out, "// %s\n", patt.c_str());
    }

    if (pointer_type)
        fprintf(out, "%sconst %s* ", flags.decl, config.type);
    else
        fprintf(out, "%sconst %s ", flags.decl, config.type);

	if (config.ns)
	{
		if (config.cpp)
		{
			if (flags.header)
				fprintf(out, "%s::", config.ns);
		}
		else
			fprintf(out, "_%s_", config.ns);
	}
	fprintf(out, "%s%s", config.prefix, name.c_str());
	
    if (pointer_type)
    {
        if (flags.define)
        {
            fprintf(out, " = \"");
            print_str(out, data, size);
            fprintf(out, "\";\n\n");
        }
        else
            fprintf(out, ";\n");
    }
    else
    {
        if (flags.define)
        {
            fprintf(out, "[%d] =\n  { ", size);
            print_hex(out, data, size);
            fprintf(out, "\n  };\n\n");
        }
        else
            fprintf(out, "[%d];\n", size);
    }
}

void Regex::OutputData(FILE* out, bool to_string)
{
    if (to_string)
    {
        print_str(out, data, size);
    }
    else
    {
        print_hex(out, data, size);
    }
}

///////////////////////////////////////
struct Normalizer
{
	char operator()(char c)
	{
		if (c >= '0' && c <= '9')
			return c;
		if (c >= 'A' && c <= 'Z')
			return c;
		if (c >= 'a' && c <= 'z')
			return c - ('a'-'A');
		return '_';
	}
	static const std::string Get(const char* ns, const char* name);
};


const std::string Normalizer::Get(const char* ns, const char* name)
{
	std::string fn;
	if (ns)
	{
		fn += ns; fn += "_";
	}
	fn += name ? name:DEFAULT_FILENAME;
	std::transform(fn.begin(), fn.end(), fn.begin(), Normalizer());
	return fn;
}

void BeginHeader(FILE* out, const Config& config, const FileConfig& flags)
{
	const std::string fn = Normalizer::Get(config.ns, config.filename);
	print_info(out, config);
	fprintf(out, "#ifndef " ID_FILE "\n", fn.c_str() );
	fprintf(out, "#define " ID_FILE "\n", fn.c_str() );
	fprintf(out, "#pragma once\n");
	print_sec(out);

	// dll export
#if defined(_WIN32)
#  ifndef aaaRE_EXP_DECL
#    define aaaRE_EXP_DECL  extern __declspec(dllimport)
#  endif
#  ifdef __cplusplus
#    ifndef aaaRECPP_EXP_DECL
#      define aaaRECPP_EXP_DECL  extern __declspec(dllimport)
#    endif
#    ifndef aaaRECPP_EXP_DEFN
#      define aaaRECPP_EXP_DEFN  __declspec(dllimport)
#    endif
#  endif
#endif 

/* By default, we use the standard "extern" declarations. */
#ifndef PCRE_EXP_DECL
#  ifdef __cplusplus
#    define PCRE_EXP_DECL  extern "C"
#  else
#    define PCRE_EXP_DECL  extern
#  endif
#endif

#ifdef __cplusplus
#  ifndef PCRECPP_EXP_DECL
#    define PCRECPP_EXP_DECL  extern
#  endif
#  ifndef PCRECPP_EXP_DEFN
#    define PCRECPP_EXP_DEFN
#  endif
#endif 

	if (!config.cpp && !flags.define)
	{
		fprintf(out, "#ifdef __cplusplus\n"
			"extern \"C\" {\n"
			"#endif\n\n");
	}
	if (config.cpp && config.ns)
		fprintf(out, "namespace %s {\n\n", config.ns);
}

void EndHeader(FILE* out, const Config& config, const FileConfig& flags)
{
	const std::string fn = Normalizer::Get(config.ns, config.filename);

	if (config.cpp && config.ns)
		fprintf(out, "\n} // namespace %s\n", config.ns);
	if (!config.cpp && !flags.define)
	{
		fprintf(out, "\n\n#ifdef __cplusplus\n"
					"}  /* extern \"C\" */\n"
					"#endif\n");
	}
	fprintf(out, "\n#endif // " ID_FILE "\n", fn.c_str() );
}

void BeginImplement(FILE* out, const Config& config, const FileConfig& flags)
{
	print_info(out, config);
	fprintf(out, "#include <pcre.h>\n");
	if (config.cpp && config.ns && flags.header)
		fprintf(out, "#include \"%s\"\n", flags.header);

	print_sec(out);

	if (config.cpp && config.ns && !flags.header)
		fprintf(out, "namespace %s {\n\n", config.ns);
}

void EndImplement(FILE* out, const Config& config, const FileConfig& flags)
{
	if (config.cpp && config.ns && !flags.header)
		fprintf(out, "\n} // namespace %s\n", config.ns);
}

///////////////////////////////////
Config::Config()
{
    compile = false;
	compile_options = 0;
	nopts = 0;
	// flags
	verbose = false;
	nowarn = false;
	cpp = false;
	comment = true;
	ns = NULL;
	type = DEFAULT_TYPE;
	prefix = DEFAULT_PREFFIX;
	filename = DEFAULT_FILENAME;
    def_file = false;
    to_file = false;
	max_string = 150;
	memset(&head, 0, sizeof(head));
	memset(&impl, 0, sizeof(impl));
	// header
	head.define = true;
	head.decl = DEFAULT_DECL;
	head.header = NULL;
}

int Config::Args(int argc, const char* argv[])
{
	int a = 1;
	for (; a < argc; a++)
	{
		const char* p = argv[a];
		if (*p++ != '-') // config option must start with '-'
			return a;
		if (*p == '-') // long option
		{
			p++;
			if (!*p) return a + 1;// '--' end config options
			// long option
			const Option& opt = long_opt(p);
			if (opt.flags & ParamString)
			{
				while (*p && *p != '=') p++;
				if (*p++ != '=')
				{
					fprintf(stderr, "ERROR: Parameter --%s requied value.\n", opt.long_name);
					usage();
				}
				Action(opt, p);
				continue;
			}
			Action(opt);
			continue;
		}
		// short option
		while (*p)
		{
			const Option& opt = short_opt(*p++);
			if (opt.flags & ParamString)
			{
				if (*p)
				{
					Action(opt, p);
					break;
				}
				// next parameter
				a++;
				if (a >= argc || *argv[a] == '-')
				{
					fprintf(stderr, "ERROR: Parameter -%c requied value.\n", opt.one_char);
					usage();
				}
				else
					Action(opt, argv[a]);
				break;
			}
			Action(opt);
		}
	}
	return a;
}

void Config::Action(const Option& opt)
{
    switch (opt.flags & OptMask)
	{
	case OptHelp:
		help(); break;
	case OptVersion:
		version(); break;
    case OptCompile:
        compile = true;
        break;
	case OptCompileFlag:
		compile_options |= opt.data;
		opts[nopts++] = &opt;
		break;
	case OptCPlusPlus:
		cpp = true; break;
	//OptExports     = 0x9000,
	case OptComment:
		comment = false; break;
	case OptVerbose:
		verbose = true; break;
	case OptWarning:
		nowarn = true; break;
	//OptPerformace  = 0xd000,
	default:
		if (opt.long_name)
			printf("Missing action for %s option.\n", opt.long_name);
		else
			printf("Missing action for -%c option.\n", opt.one_char);
		break;
	};
}

void Config::Action(const Option& opt, const char* param)
{
	if (!param || !*param)
	{
		fprintf(stderr, "ERROR: Empty value for parameter --%s.\n", opt.long_name);
		usage();
	}
    switch (opt.flags & OptMask)
	{
    case OptFormat:
        if (strcmp("string", param) == 0)
            max_string = 0xfffffff;
        else if (strcmp("array", param) == 0)
            max_string = 0x0;
		else {
			fprintf(stderr, "ERROR: Unknown value %s. (Akcepted: array, string)\n", param);
			usage();
		}
        break;
	case OptCompileNL:
		if (strcmp("ANY", param) == 0)
			compile_options |= PCRE_NEWLINE_ANY;
		else if (strcmp("ANYCRLF", param) == 0)
			compile_options |= PCRE_NEWLINE_ANYCRLF;
		else if (strcmp("CR", param) == 0)
			compile_options |= PCRE_NEWLINE_CR;
		else if (strcmp("CRLF", param) == 0)
			compile_options |= PCRE_NEWLINE_CRLF;
		else if (strcmp("LF", param) == 0)
			compile_options |= PCRE_NEWLINE_LF;
		else {
			fprintf(stderr, "ERROR: Unknown value %s. (Akcepted: ANY, ANYCRLF, CR, CRLF, LF)\n", param);
			usage();
		}
		opts[nopts++] = &opt;
		break;
	case OptOutput:
        if (is_stdout_tty())
            to_file = true;
		filename = param; break;
	case OptHeader:
        to_file = true;
		head_fn = param; break;
	case OptSource:
        def_file = true;
		impl_fn = param; break;
    case OptType:
        type = param; break;
	case OptPrefix:
		prefix = param; break;
	case OptNamespace:
		ns = param; break;
	default:
		if (opt.long_name)
			printf("Missing action for %s option.\n", opt.long_name);
		else
			printf("Missing action for -%c option.\n", opt.one_char);
		break;
	};
}

void Config::Configure()
{
	if (head_fn.empty())
	{
		head_fn = filename;
		head_fn += cpp ? DEFAULT_HXX_SUFFIX:DEFAULT_H_SUFFIX;
	}
    else
        to_file = true;

	if (impl_fn.empty())
	{
		impl_fn = filename;
		impl_fn += cpp ? DEFAULT_CXX_SUFFIX:DEFAULT_C_SUFFIX;
	}
    else
        def_file = true;

    if (!def_file)
        return;
	// set to generate cpp file
	if (strcmp(head.decl, "static ") == 0)
	{
		head.decl = "extern ";
		impl.decl = "";
	}
	impl.define = true;
	head.define = false;
	impl.header = head_fn.c_str();
}

void Compile(const Config& config, const char* name, const char* patt, FILE* head, FILE* imp)
{
    Regex x(config, name, patt);
	if (head)
		x.print(head, config, config.head);
	if (imp)
	    x.print(imp, config, config.impl);
}

void Compile(const Config& config, const char* patt, FILE* out)
{
    Regex x(config, "", patt);
    bool str = config.max_string > 0;
    fprintf(out, str ? "\"":"{");
    x.OutputData(out, str);
    fprintf(out, str ? "\",\n":"};\n");
}

void Process(const Config& config, const char* patt, FILE* head, FILE* imp)
{
    static int exp = 1;
    if (config.compile)
    {
        Compile(config, patt, head);
        return;
    }
    char name[256];
    if (*patt == '*' || *patt == '?')
    {
        patt++;
        int i = 0;
        while (isalpha(patt[i]) || patt[i] == '_') i++;
        memcpy(name, patt, i);
        name[i] = '\0';
        Compile(config, name, patt + i + 1, head, imp);
        return;
    }
    sprintf(name, "exp%d", exp++);
    Compile(config, name, patt, head, imp);
}

int main(int argc, const char* argv[])
{
	Config config;
	// header
	FILE* head_out = stdout;
	FILE* impl_out = NULL;
	// read arguments
	int param = config.Args(argc, argv);
	config.Configure();

    if (config.verbose && !config.to_file && !is_stdout_tty())
        fprintf(stderr, "Export to file.\n");

	if (config.to_file)
	{
		head_out = fopen(config.head_fn.c_str(), "wt");
	    if (config.verbose)
		    fprintf(stderr, "Create header %s\n", config.head_fn.c_str());
    }
    if (config.def_file)
    {
		impl_out = fopen(config.impl_fn.c_str(), "wt");
	    if (config.verbose)
		    fprintf(stderr, "Create source %s\n", config.impl_fn.c_str());
	}
	if (!config.compile && impl_out)
		BeginImplement(impl_out, config, config.impl);
	if (!config.compile && head_out)
		BeginHeader(head_out, config, config.head);

    //Compile(config, "xml_tag", "<\\?xml version=\"([^\"]*)\"\\?>", head_out, impl_out);
    //Regex br(config, "big_tag", "|\\$\\<\\.X\\+ix\\[d1b\\!H\\#\\?vV0vrK\\:ZH1\\=2M\\>iV\\;\\?aPhFB\\<\\*vW\\@QW\\@sO9\\}cfZA\\-i\\'w\\%%hKd6gt1UJP\\,15_\\#QY\\$M\\^Mss_U\\/\\]\\&LK9\\[5vQub\\^w\\[KDD\\<EjmhUZ\\?\\.akp2dF\\>qmj\\;2\\}YWFdYx\\.Ap\\]hjCPTP\\(n28k\\+3\\;o\\&WXqs\\/gOXdr\\$\\:r\\'do0\\;b4c\\(f_Gr\\=\\\"\\\\4\\)\\[01T7ajQJvL\\$W\\~mL_sS\\/4h\\:x\\*\\[ZN\\=KLs\\&L5zX\\/\\/\\>it\\,o\\:aU\\(\\;Z\\>pW\\&T7oP\\'2K\\^E\\:x9\\'c\\[\\%%z\\-\\,64JQ5AeH_G\\#KijUKghQw\\^\\\\vea3a\\?kka_G\\$8\\#\\`\\*kynsxzBLru\\'\\]k_\\[7FrVx\\}\\^\\=\\$blx\\>s\\-N\\%%j\\;D\\*aZDnsw\\:YKZ\\%%Q\\.Kne9\\#hP\\?\\+b3\\(SOvL\\,\\^\\;\\&u5\\@\\?5C5Bhb\\=m\\-vEh_L15Jl\\]U\\)0RP6\\{q\\%%L\\^_z5E\\'Dw6X\\b|BM");
    //Compile(config, "longer", "/a(?P<name1>b|c)d(?P<longername2>e)/BM", head_out, impl_out);
    if (param >= argc)
    {
        // read from stdio
        char patt[2048];
        while (gets(patt))
        {
            Process(config, patt, head_out, impl_out);
        }
    }
    else
    {
        for (;param < argc; param++)
        {
            Process(config, argv[param], head_out, impl_out);
        }
    }


	if (!config.compile && head_out)
		EndHeader(head_out, config, config.head);
	if (!config.compile && impl_out)
		EndImplement(impl_out, config, config.impl);

	if (head_out && head_out != stdout)
		fclose(head_out);
	if (impl_out && impl_out != stdout)
		fclose(impl_out);
    return 0;
}

