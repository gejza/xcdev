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

#include <xc/error.h>

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
	serialize(out, key, obj);
}

void serialize(xc::buffer_t& out, const std::string& key, AV* av)
{
    I32 len = av_len(av);
	xc::serialize_t obj;
    for (I32 i=0; i <= len;i++)
		obj.add(*av_fetch(av, i, 0));
	serialize(out, key, obj);
}

void serialize(xc::buffer_t& out, const std::string& key, SV* value)
{
    switch (SvTYPE(value)) {
    case SVt_PV:
		XC_DBG("Serialize perl PV");
        serialize(out, key, SvPVX(value));
        break;
    case SVt_NV:
		XC_DBG("Serialize perl NV");
        serialize(out, key, SvNVX(value));
        break;
    case SVt_IV:
		XC_DBG("Serialize perl IV");
        serialize(out, key, SvIVX(value));
        break;
    case SVt_PVHV:
		XC_DBG("Serialize perl HV");
		serialize(out, key, (HV*)value);
        break;
    case SVt_PVAV:
		XC_DBG("Serialize perl AV");
		serialize(out, key, (AV*)value);
        break;
    case SVt_RV:
		XC_DBG("Serialize perl RV");
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

	void insert(int ns, char * key, SV* value) {
		try {
			xc::buffer_t v = xc::serialize(value);
			XC_DBG("Serialize to %s", xc::human(v.data(), v.size(), 30).c_str());
			printf("%s = %ld\n", key, v.size());
			xc::dump(xc::chunk(v));
			_out.insert(ns, key, xc::data_t(v));
    	} catch (const xc::error_t& e) {
        	croak("Exception: %s", e.message().c_str());
		}
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

