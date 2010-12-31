#ifdef __cplusplus
extern "C" {
#endif

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#ifdef __cplusplus
}
#endif

#include <fcntl.h>
#include <string>
#include <iostream>

#include <xc/rd/out_cdb.h>

class Make
{
public:
	Make(char * filename)
		: _out(filename) {
		std::cout << "I'm constructin'\n";
	}

	~Make() {
		std::cout << "Destruction is a way of life for me.\n";
	}

	void insert(int ns, char * key, char * value) {
		_out.insert(ns, key, value);
	}

private:
	xc::rd::CDBMake_t _out;
};

MODULE = XC::RD::Make		PACKAGE = XC::RD::Make

Make *
Make::new(char * filename)

void
Make::DESTROY()

void
Make::insert(int ns, char * key, char * value)
