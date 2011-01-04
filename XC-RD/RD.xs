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

#include <xc/serialize.h>
#include <xc/unserialize.h>

#include <xc/rd/out_cdb.h>
#include <xc/rd/cdb.h>

void serialize(xc::buffer_t& out, const std::string& key, HV* hv)
{
	xc::serialize_t obj;
	HE *he;
	I32 len;
	hv_iterinit(hv);
	while((he = hv_iternext(hv)) != NULL) {
		obj.add(hv_iterkey(he, &len), hv_iterval(hv, he));
	}
	serialize(out, obj);
}

void serialize(xc::buffer_t& out, const std::string& key, AV* av)
{
    I32 len = av_len(av);
	xc::serialize_t obj;
    for (I32 i=0; i <= len;i++)
		obj.add(*av_fetch(av, i, 0));
	serialize(out, obj);
}

void serialize(xc::buffer_t& out, const std::string& key, SV* value)
{
    switch (SvTYPE(value)) {
    case SVt_PV:
        serialize(out, key, SvPVX(value));
        break;
    case SVt_NV:
        serialize(out, key, SvNVX(value));
        break;
    case SVt_IV:
        serialize(out, key, SvIVX(value));
        break;
    case SVt_PVHV:
		serialize(out, key, (HV*)value);
        break;
    case SVt_PVAV:
		serialize(out, key, (AV*)value);
        break;
    case SVt_RV:
        serialize(out, key, SvRV(value));
        break;
    default:
        printf("Type %x\n", SvTYPE(value));
    };
}

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
        switch (SvTYPE(value)) {
        case SVt_PV:
		    _out.insert(ns, key, SvPVX(value));
            break;
        case SVt_NV:
            //printf("Double: %lf\n", SvNVX(value));
		    //_out.insert(ns, key, SvPVX(value));
            break;
        case SVt_IV:
            //printf("Num: %d\n", SvIVX(value));
		    //_out.insert(ns, key, SvPVX(value));
            break;
        case SVt_PVAV:
        case SVt_PVHV:
		_out.insert(ns, key, xc::serialize(value));
		{ xc::buffer_t buff = xc::serialize(value);
		xc::unserialize_t un(buff);
		xc::dump(un); }
            break;
        case SVt_RV:
            insert(ns, key, SvRV(value));
            break;
        default:
            printf("Type %x\n", SvTYPE(value));
        };
        //int t = SvTYPE(SvRV(value));
        /*printf("%s = ", key);
        I32 len;
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
			xc::unserialize_t un(val);
			xc::dump(un);
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

