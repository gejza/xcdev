/*
   File name:  cbdump.cc
   Date:       2010/12/06 02:31
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "StdAfx.h"

//#include "cb.h"
#include "../include/xc/rd/cdb.h"

void d(const xc::rd::ns_t ns, const xc::data_t& key,
                    const xc::data_t& value)
{
	printf("ns=%d key=%s data=%s\n",
			ns, xc::human(key, 20).c_str(), xc::human(value, 60).c_str());

}

int main(int argc, const char* argv[])
{
    xc::debug::debug_enable();
    xc::log::add_stderr("ALL");

    if (argc <= 1)
    {
        fprintf(stderr, "Usage: %s <db>\n", argv[0]);
        return EXIT_FAILURE;
    }

    try {
        xc::rd::ConstDB_t lookup(argv[1]);
        lookup.dump(d);

    } catch (const xc::error_t& e) {
        std::cerr << "Error: " << e.message() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "Std exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

    return 0;
}
