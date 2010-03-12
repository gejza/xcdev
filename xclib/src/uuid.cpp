
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/sysinfo.h>
#endif
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "xc/uuid.h"
#include "xc/error.h"

	/* uuid_compare --  Compare two UUID's "lexically" and return */
#define CHECK(f1, f2) if ((f1) != (f2)) return (f1) < (f2) ? 1 : -1;
	int xc::uuid::compare(const xc::uuid& id1, const xc::uuid& id2)
	{
		CHECK(id1.data1, id2.data1);
		CHECK(id1.data2, id2.data2);
		CHECK(id1.data3, id2.data3);
		return memcmp(id1.data4, id2.data4, sizeof(id1.data4));
	}
#undef CHECK	
#if 0
/* set the following to the number of 100ns ticks of the actual
   resolution of your system's clock */
#define UUIDS_PER_TICK 1024

/* Set the following to a calls to get and release a global lock */
#define LOCK
#define UNLOCK

typedef unsigned long long unsigned64;
typedef unsigned long   unsigned32;
typedef unsigned short  unsigned16;
typedef unsigned char   unsigned8;
typedef unsigned char   byte;

typedef unsigned64 xc_uuid_time_t;
typedef struct {
    char nodeID[6];
} uuid_node_t;

/* data type for UUID generator persistent state */
typedef struct {
    xc_uuid_time_t  ts;       /* saved timestamp */
    uuid_node_t  node;     /* saved node ID */
    unsigned16   cs;       /* saved clock sequence */
} uuid_state;

static void get_ieee_node_identifier(uuid_node_t *node);
static void get_system_time(xc_uuid_time_t *xc_uuid_time);
static void get_random_info(char seed[16]);
static void get_current_time(xc_uuid_time_t *timestamp);
static unsigned16 true_random(void);

/* various forward declarations */
static void format_uuid_v1(xc_uuid_t *uuid, unsigned16 clockseq,
    xc_uuid_time_t timestamp, uuid_node_t node);

/********* Public functions ************/
/* uuid_create -- generator a UUID */
int xc_uuid_generate(xc_uuid_t *uuid)
{
     xc_uuid_time_t timestamp;
     unsigned16 clockseq;
     uuid_node_t node;

     /* acquire system-wide lock so we're alone */
     LOCK;
     /* get time, node ID, saved state from non-volatile storage */
     get_current_time(&timestamp);
     get_ieee_node_identifier(&node);
        
     clockseq = true_random();

     UNLOCK;

     /* stuff fields into the UUID */
     format_uuid_v1(uuid, clockseq, timestamp, node);
     return XC_OK;
}

/* uuid_compare --  Compare two UUID's "lexically" and return */
#define CHECK(f1, f2) if ((f1) != (f2)) return (f1) < (f2) ? 1 : -1;
int xc_uuid_compare(const xc_uuid_t *u1, const xc_uuid_t *u2)
{
    CHECK(u1->data1, u2->data1);
    CHECK(u1->data2, u2->data2);
    CHECK(u1->data3, u2->data3);
    return memcmp(u1->data4, u2->data4, sizeof(u1->data4));
}
#undef CHECK

/* print a UUID */
void xc_uuid_str(const xc_uuid_t *u, char * buff)
{
    int i;
    char* p = buff;

    p += sprintf(p, "%8.8lx-%4.4x-%4.4x-", u->data1, u->data2, u->data3);
    for (i = 0; i < 9; i++) {
        p += sprintf(p, "%2.2x", u->data4[i]);
    }
    while (*buff) {
		if (*buff >= 'a' && *buff <= 'z')
			*buff -= 'a' - 'A';
		buff++;
    }
}

/**
 * Copy from src to desc
 */
void xc_uuid_copy(xc_uuid_t *dest, const xc_uuid_t *src)
{
	memcpy(dest, src, sizeof(xc_uuid_t));
}


/********* Private functions ************/

/* system dependent call to get IEEE node ID.
   This sample implementation generates a random node ID. */
static void get_ieee_node_identifier(uuid_node_t *node)
{
    static uuid_node_t saved_node;
    char seed[16];

    get_random_info(seed);
    seed[0] |= 0x01;
    memcpy(&saved_node, seed, sizeof saved_node);
    
    *node = saved_node;
}

static void get_system_time(xc_uuid_time_t *xc_uuid_time)
{
#ifdef _WIN32
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	*xc_uuid_time = t.QuadPart + 0x01B21DD213814000LL;
#else
    struct timeval tp;
    gettimeofday(&tp, (struct timezone *)0);

    /* Offset between UUID formatted times and Unix formatted times.
       UUID UTC base time is October 15, 1582.
       Unix base time is January 1, 1970.*/
    *xc_uuid_time = ((unsigned64)tp.tv_sec * 10000000)
        + ((unsigned64)tp.tv_usec * 10)
        + 0x01B21DD213814000LL;
#endif
}

/* random info */
static void get_random_info(char seed[16])
{
#ifndef _WIN32
    struct {
        struct timeval t;
        struct sysinfo s;
        char hostname[257];
    } r;
    
    char t = 0xff;
    int is = 0, i;
    const char * pr = (const char*)&r;

    sysinfo(&r.s);
    gettimeofday(&r.t, (struct timezone *)0);
    gethostname(r.hostname, 256);
#else
    char t = 0xff;
    int is = 0, i;
	SYSTEM_INFO r;
    const char * pr = (const char*)&r;
	GetSystemInfo(&r);
#endif

    memset(seed, 0, 16);
    for (i=0; i < sizeof(r); i++) {
        t = pr[i] ^ t;
        seed[is] = t ^ seed[is];
        is++;
        if (is >= 16) is = 0;
    }
    
}

/* format_uuid_v1 -- make a UUID from the timestamp, clockseq,
                     and node ID */
static void format_uuid_v1(xc_uuid_t* uuid, unsigned16 clock_seq,
                    xc_uuid_time_t timestamp, uuid_node_t node)
{
    /* Construct a version 1 uuid with the information we've gathered
       plus a few constants. */

    /*uuid->time_low = (unsigned long)(timestamp & 0xFFFFFFFF);
    uuid->time_mid = (unsigned short)((timestamp >> 32) & 0xFFFF);
    uuid->time_hi_and_version =

        (unsigned short)((timestamp >> 48) & 0x0FFF);
    uuid->time_hi_and_version |= (1 << 12);
    uuid->clock_seq_low = clock_seq & 0xFF;
    uuid->clock_seq_hi_and_reserved = (clock_seq & 0x3F00) >> 8;
    uuid->clock_seq_hi_and_reserved |= 0x80;
    memcpy(&uuid->node, &node, sizeof uuid->node);*/
}

/* get-current_time -- get time as 60-bit 100ns ticks since UUID epoch.
   Compensate for the fact that real clock resolution is
   less than 100ns. */
static void get_current_time(xc_uuid_time_t *timestamp)
{
    static int inited = 0;
    static xc_uuid_time_t time_last;
    static unsigned16 uuids_this_tick;
    xc_uuid_time_t time_now;

    if (!inited) {
        get_system_time(&time_last);
        uuids_this_tick = UUIDS_PER_TICK;
        inited = 1;
    }

    for ( ; ; ) {
        get_system_time(&time_now);

        /* if clock reading changed since last UUID generated, */
        if (time_last != time_now) {
            /* reset count of uuids gen'd with this clock reading */
            uuids_this_tick = 0;
            time_last = time_now;
            break;
        }
        if (uuids_this_tick < UUIDS_PER_TICK) {
            uuids_this_tick++;
            break;
        }

        /* going too fast for our clock; spin */
    }
    /* add the count of uuids to low order bits of the clock reading */
    *timestamp = time_now + uuids_this_tick;
}

/* true_random -- generate a crypto-quality random number.
   **This sample doesn't do that.** */
static unsigned16 true_random(void)
{
    static int inited = 0;
    xc_uuid_time_t time_now;

    if (!inited) {
        get_system_time(&time_now);
        time_now = time_now / UUIDS_PER_TICK;
        srand((unsigned int)
               (((time_now >> 32) ^ time_now) & 0xffffffff));
        inited = 1;
    }

    return rand();
}



#endif
