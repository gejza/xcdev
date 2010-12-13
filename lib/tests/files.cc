/*
   File name:  files.cc
   Date:       2010/03/25 10:29
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <wordexp.h>
#include <dirent.h>
#include <fnmatch.h>

#include <xc/file.h>
#include <xc/fd.h>
#include <xc/dir.h>

int filt(const struct dirent* de)
{
    return fnmatch("*.cc", de->d_name, 0) == 0;
}

void test_scan()
{
    printf("--- %s ---\n", __FUNCTION__);
    struct dirent **namelist;
    int n;
    n = scandir(".", &namelist, filt, alphasort);
    if (n < 0)
        perror("scandir");
    else {
        while(n--) {
            printf("%s\n", namelist[n]->d_name);
            free(namelist[n]);
        }
        free(namelist);
    }
}

void test_ls()
{
    printf("--- %s ---\n", __FUNCTION__);
        wordexp_t p;
        char **w;
        int i;
        wordexp("${prefix}/var/run/mfis/mfisd.pid", &p, 0);
        w = p.we_wordv;
        for (i=0; i<p.we_wordc; i++)
                printf("%s\n", w[i]);
        wordfree(&p);
}

int xc_test_main(int argc, const char* argv[])
{
    test_ls();
    test_scan();
    return 0;
}



