
#include <Python.h>
#include "pyutils.h"
#include "xcprof/profiler.h"
#include "xcprof/times.h"

#undef SRC_INFO
#if WRITE_SRC_INFO
#define SRC_INFO , python_getsrcname(CURR_FRAME), python_getcodepos(CURR_FRAME)
#else
#define SRC_INFO
#endif

void profiler_python_info(xc_profiler_t* profiler)
{
	xcprof_begin(profiler, "PythonInfo");
	xcprof_value_string("ProgramName", Py_GetProgramName() SRC_INFO);
	xcprof_value_string("ProgramFullPath", Py_GetProgramFullPath() SRC_INFO);
	xcprof_value_string("Version", Py_GetVersion() SRC_INFO);
	xcprof_value_string("Platform", Py_GetPlatform() SRC_INFO);
	xcprof_value_string("BuildInfo", Py_GetBuildInfo() SRC_INFO);
	xcprof_value_string("Compiler", Py_GetCompiler() SRC_INFO);
	xcprof_value_string("Copyright", Py_GetCopyright() SRC_INFO);
	xcprof_end(profiler);
}

//////////////////////////////////////

typedef struct {
    PyObject_HEAD
   	time_watch_t watch;
} WatchObject_t;

static PyObject* watch_timer_stop(WatchObject_t *self, PyObject *args)
{
    int num = 1;
    if (!PyArg_ParseTuple(args, "|i", &num))
        return NULL;
	watch_finish(&self->watch, num);
	return PyFloat_FromDouble(watch_one(&self->watch));
}

static PyMethodDef watch_methods[] = {
    {"stop", (PyCFunction)watch_timer_stop, METH_VARARGS, "Stop watch" },
    {NULL}  /* Sentinel */
};

PyTypeObject WatchType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "Watch", /*tp_name*/
    sizeof(WatchObject_t), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT, /*tp_flags*/
    "Time monitor",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    watch_methods,             /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                         /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,         /* tp_new */
};

/////////////////////////////////////////////
// Profiler object
typedef struct {
    PyObject_HEAD
   	xc_profiler_t prof;
} ProfilerObject_t;

static int prof_init(ProfilerObject_t *self, PyObject *args, PyObject *kwds)
{
    const char *fn = NULL;
	static char *kwlist[] = {"filename", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|s", kwlist, &fn))
		return -1; 

	xcprof_init(&self->prof);
	xcprof_child_init();
	profiler_python_info(&self->prof);
    return 0;
}

static void prof_dealloc(ProfilerObject_t* self)
{
	xcprof_child_destroy();
	xcprof_destroy(&self->prof);
    self->ob_type->tp_free((PyObject*)self);
}

////////////////////////////////////////
static PyObject* prof_begin(ProfilerObject_t *self, PyObject *args)
{
    const char *symbol;
    if (!PyArg_ParseTuple(args, "s", &symbol))
        return NULL;
	xcprof_begin(&self->prof, symbol);
    Py_RETURN_NONE;
}

static PyObject* prof_end(ProfilerObject_t *self)
{
	xcprof_end(&self->prof);
    Py_RETURN_NONE;
}

static PyObject* prof_dumpinfo(ProfilerObject_t *self)
{
	profiler_python_info(&self->prof);
    Py_RETURN_NONE;
}

static PyMethodDef prof_methods[] = {
    {"begin", (PyCFunction)prof_begin, METH_VARARGS, "Begin scan" },
    {"end", (PyCFunction)prof_end, METH_NOARGS, "End scan" },
    {"dumpinfo", (PyCFunction)prof_dumpinfo, METH_NOARGS, "Dump info about python" },
    {NULL}  /* Sentinel */
};

PyTypeObject ProfilerType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "Profiler", /*tp_name*/
    sizeof(ProfilerObject_t), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)prof_dealloc,          /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "ProfilerObject_t objects",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    prof_methods,             /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)prof_init,      /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,                 /* tp_new */
};

////////////////////////////////
static PyObject* dump_data(PyObject *self, PyObject *args)
{
    const char *symbol;
    PyObject *object;

    if (!PyArg_ParseTuple(args, "sO", &symbol, &object))
        return NULL;
	if (PyString_Check(object))
	{
		xcprof_value_string(symbol, PyString_AS_STRING(object) SRC_INFO);
	}
	else if (PyInt_CheckExact(object))
	{
		xcprof_value_int(symbol, (int)PyInt_AS_LONG(object) SRC_INFO);
	}
	else if (PyObject_IsInstance(object, (PyObject*)&WatchType))
	{
		WatchObject_t* w = (WatchObject_t*)object;
		if (w->watch.start && !w->watch.nums)
			watch_finish(&w->watch, 1);
		xcprof_value_watch(symbol, &w->watch SRC_INFO);
	}
	else
	{
		PyErr_Format(PyExc_TypeError, "Object '%s' failed dump", object->ob_type->tp_name);
		Py_DECREF(object);
		return NULL;
	}
	Py_DECREF(object);
    Py_RETURN_NONE;
}

static PyObject* dump_checkpoint(PyObject *self, PyObject *args)
{
    const char *symbol;
    if (!PyArg_ParseTuple(args, "s", &symbol))
        return NULL;

	// get file and line
	xcprof_value_checkpoint(symbol, 
		python_getsrcname(CURR_FRAME),
		python_getcodepos(CURR_FRAME),
		python_getfuncname(CURR_FRAME));

    Py_RETURN_NONE;
}

static PyObject* dump_traceback(PyObject *self, PyObject *args)
{
    const char *symbol;
	const PyFrameObject* frame;

    if (!PyArg_ParseTuple(args, "s", &symbol))
        return NULL;

	frame = CURR_FRAME;
	while (frame)
	{
		printf("Trace: %s(%d):%s\n", 
			python_getsrcname(frame),
			python_getcodepos(frame),
			python_getfuncname(frame));
		frame = frame->f_back;
	}

    Py_RETURN_NONE;
}

static PyObject* profiler_watch_start(PyObject *self, PyObject *args)
{
    WatchObject_t *watch = PyObject_New(WatchObject_t, &WatchType);
	PyObject_Init((PyObject *)watch, &WatchType);
    watch_start(&watch->watch);
    return (PyObject *)watch;
}

////////////////
// global methods
PyMethodDef profiler_methods[] = {

    {"dump", dump_data, METH_VARARGS, "Write data to dump."},
    {"watch", profiler_watch_start, METH_NOARGS, "Start new watch."},
    {"checkpoint", dump_checkpoint, METH_VARARGS, "Save checkpoint."},
    {"traceback", dump_traceback, METH_VARARGS, "Write traceback."},

    {NULL, NULL, 0, NULL}        /* Sentinel */
};


