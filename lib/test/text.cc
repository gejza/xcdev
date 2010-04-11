/*
   File name:  text.cc
   Date:       2010/03/17 03:05
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>

#include <xc/text.h>

void test_color()
{
    for (int col=29; col < 40; col++) {
        xc::colored_t c(stdout, col);
        printf("barva pisma %d\n", col);
    }
}

void test_regex(const char* exp, const char** str)
{
    printf("test regex: %s\n", exp);
    xc::regex_t r(exp);
    for (int i=0; str[i]; i++) {
        printf("%s: %s\n", str[i], r == str[i] ? "match":"no");
    }
}

int xc_test_main(int argc, const char* argv[])
{
    // test human
    printf("Human string: %s\n", xc::human("multiline\nstring with \r"
                " chars and very very big woooooooooooooooooooooooord", 20).c_str());
    test_color();
    const char* t1[] = { "aaa", "123", "1a2", "a1", 0x0 };
    test_regex("[:isalpha:]", t1);
    test_regex("^[0-9]+", t1);
    return 0;
}




