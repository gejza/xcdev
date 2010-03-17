%top{
/*
   File name:  parser.h
   Date:       2010/02/05 18:07
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
}
%{
%}

%option 8bit reentrant default
%option batch
%option nounput
%option noyywrap
%option verbose
%option warn
%option debug

%x COMMENT
%x PRE

DIGIT   [0-9]
NAME    [A-Za-z]+
WS      [\t ]+
STR1    \"[^\"]*\"
STR2    '[^']*'

%%


^%include{WS}{STR1}.*$  return 1;
^%include{WS}{STR2}.*$  return 1;
^%{NAME}                return 1;

%%

/*HtmlFile::HtmlFile(const char* filename)
    : m_in(NULL), m_scanner(NULL), m_line(1), m_echo(T_DICT)
{
    yylex_init(&m_scanner);
    if (filename)
    {
        m_in = fopen(filename, "r");
        if (!m_in)
            throw std::runtime_error(std::string("file ") + filename + " not found");
        yyrestart(m_in, m_scanner);
    }
    //yyset_lineno(1, m_scanner);
    //yyset_debug(1, m_scanner);
}

HtmlFile::~HtmlFile()
{
    if (m_scanner)
        yylex_destroy(m_scanner);
    if (m_in)
        ::fclose(m_in);
}

int HtmlFile::getLine() const
{
    return m_line;
}

int HtmlFile::lex(HtmlFile::Value& val)
{
    int t = yylex(m_scanner);
    val.str = yyget_text(m_scanner);
    val.length = yyget_leng(m_scanner);
    return t;
}

*/
