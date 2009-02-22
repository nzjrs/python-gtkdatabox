#include "util.h"

gfloat *
_helper_unwrap_float_pylist(PyObject *py_list)
{
    int size, i;
    gfloat *list = NULL;

    size = PyList_Size (py_list);
    if (size == 0) {
        PyErr_SetString(PyExc_ValueError, "List == 0");
        return NULL;
    }

    list = g_new0 (gfloat, size);
    for (i = 0; i < size; i++) {
        PyObject *py_float;

	    py_float = PyList_GetItem (py_list, i);
        if (PyFloat_Check(py_float))
        {
            list[i] = (float)PyFloat_AsDouble(py_float);
        }
        else 
        {
            g_free(list);
            PyErr_SetString(PyExc_ValueError, "Must be Float");
            list = NULL;
            break;
        }
    }
    return list;
}

