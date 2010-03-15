
#include <stdio.h>
#include <pcre.h>
#include <string.h>
#include "regex_option.h"
#include "regex_config.h"

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
    { ParamString|OptSkeleton|4, 'S', "skel=FILE", "specify skeleton file" },
    //{ OptDebug|5, 'd', "debug", "enable debug mode" },
    { OptPerformace|5, 'p', "perf-report", "write performance report to stderr" },
    { OptWarning|5, 'w', "nowarn", "do not generate warnings" },
    { OptVerbose|5, 'v', "verbose", "write summary of scanner statistics to stdout" },
    { 0 },
};

/*************************************************
*                Usage function                  *
*************************************************/
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

