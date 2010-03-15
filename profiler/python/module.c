
#include <Python.h>
#include <frameobject.h>
#include "pyutils.h"

extern PyTypeObject SymbolTableType;
extern PyTypeObject ProfilerType;
extern PyTypeObject WatchType;
extern PyMethodDef profiler_methods[];

static int trace(PyObject *obj, PyFrameObject *frame, int what, PyObject *arg)
{
	return 0;
}

PyMODINIT_FUNC initxcprof(void)
{
    PyObject* m;

	if (PyType_Ready(&WatchType) < 0)
        return;
    if (PyType_Ready(&SymbolTableType) < 0)
        return;
    if (PyType_Ready(&ProfilerType) < 0)
        return;

    m = Py_InitModule3("xcprof", profiler_methods,
                       "Basic profiler method.");

    Py_INCREF(&SymbolTableType);
    PyModule_AddObject(m, "SymbolTable", (PyObject *)&SymbolTableType);
    Py_INCREF(&ProfilerType);
    PyModule_AddObject(m, "Profiler", (PyObject *)&ProfilerType);

	//PyEval_SetTrace( trace, NULL);
	//GV_INC_TS(CallerInfo, cc);
}
