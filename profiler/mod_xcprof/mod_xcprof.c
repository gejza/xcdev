
#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_log.h"
#include "http_main.h"
#include "http_protocol.h"
#include "http_request.h"
#include "util_script.h"
#include "http_connection.h"

#include "apr_strings.h"

#include <stdio.h>

#include "xcprof/profiler.h"
#include "xc/error.h"

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Data declarations.                                                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#define STATUS_TO_ERR(s) ((s) ? udump_strerror(s):NULL)

typedef struct _cfg_t
{
	// shared data
	int enable;
} cfg_t;

typedef struct _xcpm_cfg_t
{
	// shared data
	int enable;
	xc_profiler_t prof;

} xcpm_cfg_t;

static symboltable_t table = {0};

/*
 * Declare ourselves so the configuration routines can find and know us.
 * We'll fill it in at the end of the module.
 */
module AP_MODULE_DECLARE_DATA xcprof_module;

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* These routines are strictly internal to this module, and support its     */
/* operation.  They are not referenced by any external portion of the       */
/* server.                                                                  */
/*                                                                          */
/*--------------------------------------------------------------------------*/
__inline int symboltable_isopen(const symboltable_t* table)
{
	return (table->uuid) ? 1:0;
}

/*
 * Locate our directory configuration record for the current request.
 */
static cfg_t *our_dconfig(const request_rec *r)
{
    return (cfg_t *) ap_get_module_config(r->per_dir_config, &xcprof_module);
}

static xcpm_cfg_t *our_sconfig(const request_rec *r)
{
    return (xcpm_cfg_t *) ap_get_module_config(r->server->module_config, &xcprof_module); 
}

/*--------------------------------------------------------------------------*/
/* We prototyped the various syntax for command handlers (routines that     */
/* are called when the configuration parser detects a directive declared    */
/* by our module) earlier.  Now we actually declare a "real" routine that   */
/* will be invoked by the parser when our "real" directive is               */
/* encountered.                                                             */
/*                                                                          */
/* If a command handler encounters a problem processing the directive, it   */
/* signals this fact by returning a non-NULL pointer to a string            */
/* describing the problem.                                                  */
/*                                                                          */
/* The magic return value DECLINE_CMD is used to deal with directives       */
/* that might be declared by multiple modules.  If the command handler      */
/* returns NULL, the directive was processed; if it returns DECLINE_CMD,    */
/* the next module (if any) that declares the directive is given a chance   */
/* at it.  If it returns any other value, it's treated as the text of an    */
/* error message.                                                           */
/*--------------------------------------------------------------------------*/

/*
 * command-related code.
 */
static const char *cmd_profiler_enable(cmd_parms *cmd, xcpm_cfg_t *mconfig, int arg)
{
    mconfig->enable = arg;
    return NULL;
}

static const char *cmd_symbol_table(cmd_parms *cmd, void *mconfig, const char *fn)
{
	int res;
    const char *err = ap_check_cmd_context(cmd, GLOBAL_ONLY);
    if (err != NULL) {
        return err;
    }
	// check open
	/*if (symboltable_isopen(&table))
		return "SymbolTable already open.";
	
	res = symboltable_init(&table, fn, SYMTAB_REVERSE|SYMTAB_FULLLOAD);
	return STATUS_TO_ERR(res);*/
	return NULL;
}

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Now we declare our content handlers, which are invoked when the server   */
/* encounters a document which our module is supposed to have a chance to   */
/* see.  (See mod_mime's SetHandler and AddHandler directives, and the      */
/* mod_info and mod_status profilers, for more details.)                     */
/*                                                                          */
/* Since content handlers are dumping data directly into the connection     */
/* (using the r*() routines, such as rputs() and rprintf()) without         */
/* intervention by other parts of the server, they need to make             */
/* sure any accumulated HTTP headers are sent first.  This is done by       */
/* calling send_http_header().  Otherwise, no header will be sent at all,   */
/* and the output sent to the client will actually be HTTP-uncompliant.     */
/*--------------------------------------------------------------------------*/
static int x_handler(request_rec *r)
{
    xcpm_cfg_t *dcfg;
	symid_t id = 0;
	char symbol[1024];

    if (strcmp(r->handler, "symboltable-dump")) {
        return DECLINED;
    }

    dcfg = our_dconfig(r);
    /*
     * We're about to start sending content, so we need to force the HTTP
     * headers to be sent at this point.  Otherwise, no headers will be sent
     * at all.  We can set any we like first, of course.  **NOTE** Here's
     * where you set the "Content-type" header, and you do so by putting it in
     * r->content_type, *not* r->headers_out("Content-type").  If you don't
     * set it, it will be filled in with the server's default type (typically
     * "text/plain").  You *must* also ensure that r->content_type is lower
     * case.
     *
     * We also need to start a timer so the server can know if the connexion
     * is broken.
     */
    ap_set_content_type(r, "text/html");
    /*
     * If we're only supposed to send header information (HEAD request), we're
     * already there.
     */
    if (r->header_only) {
        return OK;
    }

    /*
     * Now send our actual output.  Since we tagged this as being
     * "text/html", we need to embed any HTML.
     */
    ap_rputs(DOCTYPE_HTML_3_2, r);
    ap_rputs("<HTML>\n", r);
    ap_rputs(" <HEAD>\n", r);
    ap_rputs("  <TITLE>mod_profiler Symbol table dump\n", r);
    ap_rputs("  </TITLE>\n", r);
    ap_rputs(" </HEAD>\n", r);
    ap_rputs(" <BODY>\n", r);
    ap_rputs("  <H1><SAMP>mod_profiler</SAMP> Symbol table dump\n", r);
    ap_rputs("  </H1>\n", r);
    ap_rputs("  <P>\n", r);
    ap_rprintf(r, "  Apache HTTP Server version: \"%s\"\n",
            ap_get_server_banner());
    ap_rputs("  <BR>\n", r);
    ap_rprintf(r, "  Server built: \"%s\"\n", ap_get_server_built());
    ap_rputs("  </P>\n", r);;
    
	/*while (symboltable_translate(&table, ++id, symbol, sizeof(symbol)))
	{
		symbol[1023] = 0;
		ap_rprintf(r, "<p>%d - %s</p>\n", (int)id, symbol);
	}*/

    /*ap_rprintf(r, "  <H2>Static callbacks so far:</H2>\n  <OL>\n%s  </OL>\n",
            trace);
    ap_rputs("  <H2>Request-specific callbacks so far:</H2>\n", r);
    ap_rprintf(r, "  <OL>\n%s  </OL>\n", apr_table_get(r->notes, TRACE_NOTE));
    ap_rputs("  <H2>Environment for <EM>this</EM> call:</H2>\n", r);
    ap_rputs("  <UL>\n", r);
    ap_rprintf(r, "   <LI>Applies-to: <SAMP>%s</SAMP>\n   </LI>\n", dcfg->loc);
    ap_rprintf(r, "   <LI>\"Example\" directive declared here: %s\n   </LI>\n",
            (dcfg->local ? "YES" : "NO"));
    ap_rprintf(r, "   <LI>\"Example\" inherited: %s\n   </LI>\n",
            (dcfg->congenital ? "YES" : "NO"));*/


    ap_rputs("  </UL>\n", r);
    ap_rputs(" </BODY>\n", r);
    ap_rputs("</HTML>\n", r);
    /*
     * We're all done, so cancel the timeout we set.  Since this is probably
     * the end of the request we *could* assume this would be done during
     * post-processing - but it's possible that another handler might be
     * called and inherit our outstanding timer.  Not good; to each its own.
     */
    /*
     * We did what we wanted to do, so tell the rest of the server we
     * succeeded.
     */
    return OK;
}

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Now let's declare routines for each of the callback phase in order.      */
/* (That's the order in which they're listed in the callback list, *not     */
/* the order in which the server calls them!  See the command_rec           */
/* declaration near the bottom of this file.)  Note that these may be       */
/* called for situations that don't relate primarily to our function - in   */
/* other words, the fixup handler shouldn't assume that the request has     */
/* to do with "profiler" stuff.                                              */
/*                                                                          */
/* With the exception of the content handler, all of our routines will be   */
/* called for each request, unless an earlier handler from another module   */
/* aborted the sequence.                                                    */
/*                                                                          */
/* Handlers that are declared as "int" can return the following:            */
/*                                                                          */
/*  OK          Handler accepted the request and did its thing with it.     */
/*  DECLINED    Handler took no action.                                     */
/*  HTTP_mumble Handler looked at request and found it wanting.             */
/*                                                                          */
/* What the server does after calling a module handler depends upon the     */
/* handler's return value.  In all cases, if the handler returns            */
/* DECLINED, the server will continue to the next module with an handler    */
/* for the current phase.  However, if the handler return a non-OK,         */
/* non-DECLINED status, the server aborts the request right there.  If      */
/* the handler returns OK, the server's next action is phase-specific;      */
/* see the individual handler comments below for details.                   */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*
 * This function gets called to create a per-server configuration
 * record.  It will always be called for the "default" server.
 *
 * The return value is a pointer to the created module-specific
 * structure.
 */
static void *x_create_server_config(apr_pool_t *p, server_rec *s)
{
    xcpm_cfg_t *cfg;

    cfg = (xcpm_cfg_t *) apr_pcalloc(p, sizeof(xcpm_cfg_t));
    cfg->enable = 0;
    return (void *) cfg;
}

/*
 * This function gets called to create a per-directory configuration
 * record.  This will be called for the "default" server environment, and for
 * each directory for which the parser finds any of our directives applicable.
 * If a directory doesn't have any of our directives involved (i.e., they
 * aren't in the .htaccess file, or a <Location>, <Directory>, or related
 * block), this routine will *not* be called - the configuration for the
 * closest ancestor is used.
 *
 * The return value is a pointer to the created module-specific
 * structure.
 */
static void *x_create_dir_config(apr_pool_t *p, char *dirspec)
{
    cfg_t *cfg;

	cfg = (cfg_t *) apr_pcalloc(p, sizeof(cfg_t));
    cfg->enable = 0;

    return (void *) cfg;
}

/*
 * This function gets called to merge two configuration
 * records. This is typically done to cope with things like virtual hosts and
 * the default server configuration  The routine has the responsibility of
 * creating a new record and merging the contents of the other two into it
 * appropriately.  If the module doesn't declare a merge routine, the more
 * specific existing record is used exclusively.
 *
 * The routine MUST NOT modify any of its arguments!
 *
 * The return value is a pointer to the created module-specific structure
 * containing the merged values.
 */
static void *x_merge_config(apr_pool_t *p, cfg_t *server1_conf,
                                         cfg_t *server2_conf)
{
    cfg_t *merged_config = (cfg_t *) apr_pcalloc(p, sizeof(cfg_t));
    /*
     * Our inheritance rules are our own, and part of our module's semantics.
     * Basically, just note whence we came.
     */
    merged_config->enable = server1_conf->enable || server2_conf->enable;
    return (void *) merged_config;
}

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Which functions are responsible for which hooks in the server.           */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static void error_hook(int status, const char* msg)
{
	ap_log_error(APLOG_MARK, APLOG_ERR, 0, NULL, "%s: %s", udump_strerror(status), msg);
}

static int init_profiler(apr_pool_t *pc, apr_pool_t *p, apr_pool_t *pt, server_rec *s)
{
	xcpm_cfg_t* cfg = (xcpm_cfg_t*)ap_get_module_config(s->module_config, &xcprof_module);
    xcprof_init(&cfg->prof);
    return 0;
}

static void init_child(apr_pool_t *p, server_rec *s)
{
	xcpm_cfg_t* cfg = (xcpm_cfg_t*)ap_get_module_config(s->module_config, &xcprof_module);
	xcprof_child_init();
}

static int x_post_config(apr_pool_t *pconf, apr_pool_t *plog,
                          apr_pool_t *ptemp, server_rec *s)
{
    return OK;
}

static int header_dump(void *rec, const char *key, const char *value)
{
	PROF_STRING(key, value);
	return 1;
}

static int x_begin(request_rec *r)
{
	xcpm_cfg_t* scfg = our_sconfig(r);
    cfg_t *cfg = our_dconfig(r);
	if (scfg->enable || cfg->enable)
	{
		xcprof_begin(&scfg->prof, r->method);

		// dump headers
		apr_table_do(header_dump, 0, r->headers_in, NULL);
		PROF_STRING("Url", r->uri);
		PROF_SYMBOL("Protocol", r->protocol);
		PROF_STRING("RemoteHost", ap_get_remote_host(r->connection,
                                                         r->per_dir_config,
                                                         REMOTE_NAME, NULL));
		PROF_STRING("RemoteIP", r->connection->remote_ip);
	}

	// dump headers
    return DECLINED;
}

static int x_end(request_rec *r)
{
	xcpm_cfg_t* scfg = our_sconfig(r);
    cfg_t *cfg = our_dconfig(r);
	if (scfg->enable || cfg->enable)
	{
		PROF_INT("Status", r->status);
		PROF_STRING("StatusMessage", r->status_line);
		xcprof_end(&scfg->prof);
	}
    return OK;
}


/*
 * Each function our module provides to handle a particular hook is
 * specified here.  The functions are registered using
 * ap_hook_foo(name, predecessors, successors, position)
 * where foo is the name of the hook.
 *
 * The args are as follows:
 * name         -> the name of the function to call.
 * predecessors -> a list of modules whose calls to this hook must be
 *                 invoked before this module.
 * successors   -> a list of modules whose calls to this hook must be
 *                 invoked after this module.
 * position     -> The relative position of this module.  One of
 *                 APR_HOOK_FIRST, APR_HOOK_MIDDLE, or APR_HOOK_LAST.
 *                 Most modules will use APR_HOOK_MIDDLE.  If multiple
 *                 modules use the same relative position, Apache will
 *                 determine which to call first.
 *                 If your module relies on another module to run first,
 *                 or another module running after yours, use the
 *                 predecessors and/or successors.
 *
 * The number in brackets indicates the order in which the routine is called
 * during request processing.  Note that not all routines are necessarily
 * called (such as if a resource doesn't have access restrictions).
 * The actual delivery of content to the browser [9] is not handled by
 * a hook; see the handler declarations below.
 */
static void register_hooks(apr_pool_t *p)
{
	error_sethandler(error_hook);

    ap_hook_open_logs(init_profiler,NULL,NULL,APR_HOOK_MIDDLE);
    ap_hook_child_init(init_child,NULL,NULL,APR_HOOK_MIDDLE);
    ap_hook_post_config(x_post_config, NULL, NULL, APR_HOOK_MIDDLE);

	// monitor
    ap_hook_post_read_request(x_begin, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_log_transaction(x_end, NULL, NULL, APR_HOOK_MIDDLE);

	// info
    ap_hook_handler(x_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* All of the routines have been declared now.  Here's the list of          */
/* directives specific to our module, and information about where they      */
/* may appear and how the command parser should pass them to us for         */
/* processing.  Note that care must be taken to ensure that there are NO    */
/* collisions of directive names between modules.                           */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*
 * List of directives specific to our module.
 */
static const command_rec xcpm_cmds[] =
{
    AP_INIT_FLAG("EnableProfiler", cmd_profiler_enable, NULL, OR_ALL,
      "\"On\" to enable profiler, \"Off\" to disable"),

	AP_INIT_TAKE1("SymbolTable", cmd_symbol_table, NULL, RSRC_CONF,
     "the filename of the symbol table"), 

    {NULL}
};
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Finally, the list of callback routines and data structures that provide  */
/* the static hooks into our module from the other parts of the server.     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*
 * Module definition for configuration.  If a particular callback is not
 * needed, replace its routine name below with the word NULL.
 */
module AP_MODULE_DECLARE_DATA xcprof_module =
{
    STANDARD20_MODULE_STUFF,
    x_create_dir_config,    /* per-directory config creator */
    x_merge_config,         /* dir config merger */
    x_create_server_config, /* server config creator */
    0,                      /* server config merger */
    xcpm_cmds,          /* command table */
    register_hooks,         /* set up other request processing hooks */
};
