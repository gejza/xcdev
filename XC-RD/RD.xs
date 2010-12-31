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
#include <xc/rd/cdb.h>

class Make
{
public:
	Make(char * filename)
		: _out(filename) {
	}

	~Make() {
	}

	void insert(int ns, char * key, char * value) {
		_out.insert(ns, key, value);
	}

	void insert(int ns, char * key, SV* value) {
		//_out.insert(ns, key, value);
		/*for (i=0; i<=av_len(array); i++) {
			      SV** elem = av_fetch(array, i, 0);
				      if (elem != NULL)
						        sum += SvNV(*elem);
								  }*/
	}
private:
	xc::rd::CDBMake_t _out;
};

class Lookup
{
public:
	Lookup(const char * filename)
		: _db(filename) {
	}

	~Lookup() {
	}

	const char * lookup(int ns, const char * key) {
		xc::data_t val;
		if (_db.lookup(ns, key, val)) {
			return (const char*)val.data();
		}
		//_out.insert(ns, key, value);
		return "Destruction is a way of life for me.\n";
	}

private:
	xc::rd::ConstDB_t _db;
};

MODULE = XC::RD	PACKAGE = XC::RD::Make

Make *
Make::new(char * filename)

void
Make::DESTROY()

void
Make::insert(int ns, char * key, SV* value)

MODULE = XC::RD PACKAGE = XC::RD::Lookup

Lookup *
Lookup::new(const char * filename)

void
Lookup::DESTROY()

const char *
Lookup::lookup(int ns, const char * key)

