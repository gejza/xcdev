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

#include <xc/rd/out_cdb.h>
#include <xc/rd/cdb.h>

void serialize(SV* value)
{
    switch (SvTYPE(value)) {
    case SVt_PV:
        printf("String: %s\n", SvPVX(value));
        //write((const char*)SvPVX(value));
        break;
    case SVt_NV:
        printf("Double: %lf\n", SvNVX(value));
        //_out.insert(ns, key, SvPVX(value));
        break;
    case SVt_IV:
        printf("Num: %d\n", SvIVX(value));
        //_out.insert(ns, key, SvPVX(value));
        break;
    case SVt_PVHV:
        printf("Hash\n");
        {
            HV *hv = (HV*)value;
            HE *he;
            hv_iterinit(hv);
            while((he = hv_iternext(hv)) != NULL) {
                serialize(hv_iterval(hv, he));
                //serialize(hv_iterkey(he, &len), hv_iterval(hv, he));
            }
        }
        //insert(ns, key, SvRV(value));
        break;
    case SVt_PVAV:
        {
            AV *av = (AV*)value;
            I32 len = av_len(av);
        printf("Array %d\n", len);
            for (I32 i=0; i <= len;i++)
                serialize(*av_fetch(av, i, 0));
        }
        //insert(ns, key, SvRV(value));
        break;
    case SVt_RV:
        serialize(SvRV(value));
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
        //int t = SvTYPE(SvRV(value));
        printf("%s = ", key);
        I32 len;
        switch (SvTYPE(value)) {
        case SVt_PV:
            printf("String: %s\n", SvPVX(value));
		    _out.insert(ns, key, SvPVX(value));
            break;
        case SVt_NV:
            printf("Double: %lf\n", SvNVX(value));
		    //_out.insert(ns, key, SvPVX(value));
            break;
        case SVt_IV:
            printf("Num: %d\n", SvIVX(value));
		    //_out.insert(ns, key, SvPVX(value));
            break;
        case SVt_PVHV:
            printf("Hash\n");
            //insert(ns, key, SvRV(value));
            break;
        case SVt_PVAV:
            //insert(ns, key, SvRV(value));
            break;
        case SVt_RV:
            printf("Ref %x\n", SvTYPE(SvRV(value)));
            serialize(SvRV(value));
            break;
        default:
            printf("Type %x\n", SvTYPE(value));
        };
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

