#include <stdio.h>
#include <cctype>
#include <string.h>
#include <string>
#include <algorithm>
#include <stdarg.h>
#include <pcre.h>
#include "regex_option.h"
#include "regex_config.h"

class ostream
{
public:
    virtual ~ostream() {}
    void print(const char* str)
    {
        write(str, strlen(str));
    }
    void print_line(const char* str)
    {
        write(str, strlen(str));
        write("\n", 1);
    }
    void printf(const char* fmt, ...)
    {
        const int max = 1024;
        char str[max+2];
        va_list args;
        va_start( args, fmt );
        size_t w = vsnprintf(str, max, fmt, args);
        va_end( args );
        write(str, w);
    }
    virtual void write(const void* buff, size_t size) = 0;
    virtual bool is_tty()
    {
        return false;
    }
};

class istream
{
public:
    virtual bool read_line(char* buff, size_t max)
    {
        if (!fgets(buff, max, stdin))
            return false;
        int e = strlen(buff) - 1;
        while (e >= 0 && (buff[e] == '\n' || buff[e] == '\r'))
        {
            buff[e] = '\0';
            e--;
        }
        return true;
    }
};

class stream_stdout : public ostream
{
public:
    virtual void write(const void* buff, size_t size)
    {
        fwrite(buff, 1, size, stdout);
    }
    virtual bool is_tty()
    {
        return isatty(fileno(stdout));
    }
};

class stream_stdin : public istream
{
};

class ostream_file : public ostream
{
    FILE* fd;
public:
    ostream_file(const char* filename)
    {
        fd = fopen(filename, "wt");
        if (!fd)
        {
            fprintf(stderr, "error: failed open file %s\n", filename);
            exit(3);
        }
    }
    virtual ~ostream_file()
    {
        fclose(fd);
    }
    virtual void write(const void* buff, size_t size)
    {
        fwrite(buff, 1, size, fd);
    }
};

class istream_file : public istream
{
    FILE* fd;
public:
    istream_file(const char* filename)
    {
        fd = fopen(filename, "rt");
        if (!fd)
        {
            fprintf(stderr, "error: failed open file %s\n", filename);
            exit(3);
        }
    }
    virtual ~istream_file()
    {
        fclose(fd);
    }
    virtual bool read_line(char* buff, size_t max)
    {
        if (!fgets(buff, max, fd))
            return false;
        int e = strlen(buff) - 1;
        while (e >= 0 && (buff[e] == '\n' || buff[e] == '\r'))
        {
            buff[e] = '\0';
            e--;
        }
        return true;
    }
};

class skeleton;

struct FileConfig
{
	const char* decl;
	bool define;
	const char* header;
    skeleton* skel;
    FileConfig()
    {
        skel = NULL;
    }
    ~FileConfig();
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

class skeleton
{
    istream_file f;
    bool do_copy;
    int src_line;
public:
    skeleton(const char* filename) : f(filename)
    {
        do_copy = true;
        src_line = 0;
    }
    void out(ostream& o, const Config& config, const FileConfig& flags)
    {
        char line[2048];
        while (1)
        {
            if (!f.read_line(line, 2048))
                return;
            src_line++;
            if (line[0] == '%')
            {
                if (line[1] == '#')
                    continue; // comment
                if (config.verbose)
                    o.printf("// \%:%s\n", line + 1);

                if (line[1] == '%')
                {
                    return; // proc
                }
#define CMD(p) strncmp(line+1, p, strlen(p)) == 0
                else if (CMD("version"))
                {
                    o.printf( " * PCRE ver%s\n", pcre_version());
                }
                else if (CMD("options"))
                {
                    for (size_t i=0;i < config.nopts;i++)
                        o.printf( " * #%s   (%s)\n", config.opts[i]->long_name, config.opts[i]->help_text);
                }
                else if (CMD("if-header"))
                {
                    do_copy = flags.header == NULL;
                }
                else if (CMD("if-source"))
                {
                    do_copy = flags.header != NULL;
                }
                else if (CMD("endif"))
                {
                    do_copy = true;
                }
                else if (!config.nowarn)
                    fprintf(stderr, "warning: unknown skeleton directive (%d): %s\n", src_line, line);
            }
            else if (do_copy)
                o.print_line(line);
        }
    }
};
FileConfig::~FileConfig()
{
    if (skel)
        delete skel;
}

class Regex
{
    std::string name;
    std::string patt;
    int size;
    unsigned char* data;
	bool pointer_type;
public:
    Regex(const Config& config, const char* _name, const char* _patt);
    void print(ostream& out, const Config& config, const FileConfig& flags);
    void OutputData(ostream& out, bool to_string);
};

//////////////////////////////////////////////////////////

namespace {

void version(void)
{
	fprintf(stderr, "Compiler PCRE version%s\n", pcre_version());
	exit(1);
}

#ifndef _WIN32 
#include <sys/stat.h>
/************* Test for regular file in Unix **********/
bool isregfile(const char *filename)
{
	struct stat statbuf;
	if (stat(filename, &statbuf) < 0)
		return false;        /* In the expectation that opening as a file will fail */
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

void print_sec(ostream& out)
{
    out.printf(
        "\n#if (defined (PCRE_MAJOR) && PCRE_MAJOR != %d) \\\n"
        "    || (defined (PCRE_MINOR) && PCRE_MINOR != %d)\n"
        "#  error \"Incompatible version of PCRE v%d.%d for predcompiled regexp. Try recompile it.\"\n"
        "#endif\n\n", PCRE_MAJOR, PCRE_MINOR, PCRE_MAJOR, PCRE_MINOR);
}

////////////////////////////////////////////////////////

void print_hex(ostream& out, const unsigned char* d, int size)
{
    size_t col = 0;
    for (int i = 0;i < size; i++)
    {
        if (i)
        {
            if (col > 10)
            {
                col = 0;
                out.print(",\n    ");
            }
            else
                out.print(", ");
        }
        col++;
        out.printf( "%3d", (int)d[i]);
    }
}

void print_str(ostream& out, const unsigned char* d, int size)
{
    bool last_n = false;
    for (int i = 0;i < size; i++)
    {
        if (d[i] < 128 && isprint(d[i])
                && (!last_n || !isdigit(d[i])))
        {
            switch (d[i])
            {
            case '%':
                out.printf("%%%%"); break;
            case '\\':
                out.printf("\\\\"); break;
            case '\"':
                out.printf("\\\""); break;
            default:
                out.printf("%c", d[i]); break;
            };
            last_n = false;
        }
        else
        {
            out.printf("\\%o", (int)d[i]);
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

void Regex::print(ostream& out, const Config& config, const FileConfig& flags)
{
    if (config.comment)
    {
        if (patt.size() > 80)
        {
            out.print( "// ");
            out.write(patt.c_str(), 77);
            out.print( "...\n");
        }
        else
            out.printf( "// %s\n", patt.c_str());
    }

    if (pointer_type)
        out.printf( "%sconst %s* ", flags.decl, config.type);
    else
        out.printf( "%sconst %s ", flags.decl, config.type);

	if (config.ns)
	{
		if (config.cpp)
		{
			if (flags.header)
				out.printf( "%s::", config.ns);
		}
		else
			out.printf( "_%s_", config.ns);
	}
	out.printf( "%s%s", config.prefix, name.c_str());
	
    if (pointer_type)
    {
        if (flags.define)
        {
            out.printf( " = \"");
            print_str(out, data, size);
            out.printf( "\";\n\n");
        }
        else
            out.printf( ";\n");
    }
    else
    {
        if (flags.define)
        {
            out.printf( "[%d] =\n  { ", size);
            print_hex(out, data, size);
            out.printf( "\n  };\n\n");
        }
        else
            out.printf( "[%d];\n", size);
    }
}

void Regex::OutputData(ostream& out, bool to_string)
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

void BeginHeader(ostream& out, const Config& config, const FileConfig& flags)
{
	const std::string fn = Normalizer::Get(config.ns, config.filename);
    if (flags.skel)
        flags.skel->out(out, config, flags);

	/*out.printf( "#ifndef " ID_FILE "\n", fn.c_str() );
	out.printf( "#define " ID_FILE "\n", fn.c_str() );
	out.printf( "#pragma once\n");
	print_sec(out);
    */

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

	/*if (!config.cpp && !flags.define)
	{
		out.printf( "#ifdef __cplusplus\n"
			"extern \"C\" {\n"
			"#endif\n\n");
	}
	if (config.cpp && config.ns)
		out.printf( "namespace %s {\n\n", config.ns);
    */
}

void EndHeader(ostream& out, const Config& config, const FileConfig& flags)
{
	const std::string fn = Normalizer::Get(config.ns, config.filename);

    //
	//if (config.cpp && config.ns)
	//	out.printf( "\n} // namespace %s\n", config.ns);
	//if (!config.cpp && !flags.define)
	//{
	//	out.printf( "\n\n#ifdef __cplusplus\n"
	//				"}  /* extern \"C\" */\n"
	//				"#endif\n");
	//}
	//out.printf( "\n#endif // " ID_FILE "\n", fn.c_str() );
    if (flags.skel)
        flags.skel->out(out, config, flags);
}

void BeginImplement(ostream& out, const Config& config, const FileConfig& flags)
{
    if (flags.skel)
        flags.skel->out(out, config, flags);
	if (config.cpp && config.ns && flags.header)
		out.printf( "#include \"%s\"\n", flags.header);

	if (config.cpp && config.ns && !flags.header)
		out.printf( "namespace %s {\n\n", config.ns);
}

void EndImplement(ostream& out, const Config& config, const FileConfig& flags)
{
	if (config.cpp && config.ns && !flags.header)
		out.printf( "\n} // namespace %s\n", config.ns);
    if (flags.skel)
        flags.skel->out(out, config, flags);
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
    case OptSkeleton:
        head.skel = new skeleton(param);
        impl.skel = new skeleton(param);
        break;
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

void Compile(const Config& config, const char* name, const char* patt, ostream* head, ostream* imp)
{
    Regex x(config, name, patt);
	if (head)
		x.print(*head, config, config.head);
	if (imp)
	    x.print(*imp, config, config.impl);
}

void Compile(const Config& config, const char* patt, ostream& out)
{
    Regex x(config, "", patt);
    bool str = config.max_string > 0;
    out.print( str ? "\"":"{");
    x.OutputData(out, str);
    out.print( str ? "\",\n":"};\n");
}

void Process(const Config& config, const char* patt, ostream* head, ostream* imp)
{
    static int exp = 1;
    if (config.compile)
    {
        if (!head)
        {
            fprintf(stderr, "configure error: must specify output file\n");
            usage();
        }
        Compile(config, patt, *head);
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
	ostream* head_out = new stream_stdout;
	ostream* impl_out = NULL;
    istream* in = NULL;
	// read arguments
	int param = config.Args(argc, argv);
	config.Configure();

    if (config.verbose && !config.to_file && !head_out->is_tty())
        fprintf(stderr, "Export to file.\n");

	if (config.to_file)
	{
        if (head_out) delete head_out;
		head_out = new ostream_file(config.head_fn.c_str());
	    if (config.verbose)
		    fprintf(stderr, "Create header %s\n", config.head_fn.c_str());
    }
    if (config.def_file)
    {
		impl_out = new ostream_file(config.impl_fn.c_str());
	    if (config.verbose)
		    fprintf(stderr, "Create source %s\n", config.impl_fn.c_str());
	}
	if (!config.compile && impl_out)
		BeginImplement(*impl_out, config, config.impl);
	if (!config.compile && head_out)
		BeginHeader(*head_out, config, config.head);

    if (param >= argc)
    {
        in = new stream_stdin;
    }
    else if ((argc-param) == 1 && isregfile(argv[param]))
    {
        if (config.verbose)
            fprintf(stderr, "Read file %s.\n", argv[param]);
        in = new istream_file(argv[param]);
    }

    if (in)
    {
        char line[2048];
        while (in->read_line(line, 2048))
        {
            Process(config, line, head_out, impl_out);
        }
    }
    else
    {
        for (;param < argc; param++)
        {
            Process(config, argv[param], head_out, impl_out);
        }
    }

    if (in)
        delete in;

	if (!config.compile && head_out)
		EndHeader(*head_out, config, config.head);
	if (!config.compile && impl_out)
		EndImplement(*impl_out, config, config.impl);

	if (head_out)
		delete head_out;
	if (impl_out)
		delete impl_out;
    return 0;
}

