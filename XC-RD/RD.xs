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
#include <xc/debug.h>

#include <xc/serialize.h>
#include <xc/unserialize.h>

#include <xc/rd/out_cdb.h>
#include <xc/rd/cdb.h>

const char* get_type(SV* value)
{
    switch (SvTYPE(value)) {
#define TOSTR(type) case type: return #type;
    TOSTR(SVt_NULL)	/* 0 */
    TOSTR(SVt_BIND)	/* 1 */
    TOSTR(SVt_IV)		/* 2 */
    TOSTR(SVt_NV)		/* 3 */
    TOSTR(SVt_RV)		/* 4 */
    TOSTR(SVt_PV)		/* 5 */
    TOSTR(SVt_PVIV)	/* 6 */
    TOSTR(SVt_PVNV)	/* 7 */
    TOSTR(SVt_PVMG)	/* 8 */
    TOSTR(SVt_PVGV)	/* 9 */
    TOSTR(SVt_PVLV)	/* 10 */
    TOSTR(SVt_PVAV)	/* 11 */
    TOSTR(SVt_PVHV)	/* 12 */
    TOSTR(SVt_PVCV)	/* 13 */
    TOSTR(SVt_PVFM)	/* 14 */
    TOSTR(SVt_PVIO)	/* 15 */
    default:
        return "unknown";
    };
}

void serialize(xc::buffer_t& out, const std::string& key, HV* hv)
{
    XC_DBG("XC::RD: Begin hash %s", key.c_str());
	xc::serialize_t obj;
	HE *he;
	I32 len;
	hv_iterinit(hv);
	while((he = hv_iternext(hv)) != NULL) {
        const char* k = hv_iterkey(he, &len);
        SV* val = hv_iterval(hv, he);
		obj.add(k, val);
	}
	serialize(out, key, obj);
    XC_DBG("XC::RD: End hash %s", key.c_str());
}

void serialize(xc::buffer_t& out, const std::string& key, AV* av)
{
    XC_DBG("XC::RD: Begin array %s", key.c_str());
    I32 len = av_len(av);
	xc::serialize_t obj;
    for (I32 i=0; i <= len;i++) {
        SV* val = *av_fetch(av, i, 0);
		obj.add(val);
    }
	serialize(out, key, obj);
    XC_DBG("XC::RD: End array %s", key.c_str());
}

void serialize(xc::buffer_t& out, const std::string& key, SV* value)
{
    if (SvROK(value)) {
        value = SvRV(value);
    }

    XC_DBG("XC::RD: %s=%s", key.c_str(), get_type(value));
    switch (SvTYPE(value)) {
    case SVt_PVMG:
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
    /*case :
        if (!SvUPGRADE(value, SVt_PVMG))*/
    default:
        printf("Type %s\n", get_type(value));
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
		return "";
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

