
#include <Python.h>
#include "xc/symboltable.h"

////////////////
typedef struct {
    PyObject_HEAD
    symboltable_t table;
} SymbolTableObject_t;

static PyObject* table_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    SymbolTableObject_t *self;

    self = (SymbolTableObject_t *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}

static int table_init(SymbolTableObject_t *self, PyObject *args, PyObject *kwds)
{
    const char *fn;

    if (!PyArg_ParseTuple(args, "s", &fn))
        return 0;

	symboltable_init(&self->table, fn, 0);
    return 0;
}

static void table_dealloc(SymbolTableObject_t* self)
{
    symboltable_close(&self->table);
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* get(SymbolTableObject_t* self, PyObject *args)
{
    const char *symbol;
    int id;
    if (!PyArg_ParseTuple(args, "s", &symbol))
        return NULL;
    id = symboltable_get(&self->table, symbol);
    return Py_BuildValue("i", id);
}

static PyMethodDef symboltable_methods[] = {
    {"get", (PyCFunction)get, METH_VARARGS,
     "Return symbol id"
    },
    {NULL}  /* Sentinel */
};

PyTypeObject SymbolTableType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "SymbolTable", /*tp_name*/
    sizeof(SymbolTableObject_t), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)table_dealloc, /*tp_dealloc*/
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
    "SymbolTableObject_t objects",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    symboltable_methods,             /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)table_init,      /* tp_init */
    0,                         /* tp_alloc */
    table_new,                 /* tp_new */
};




