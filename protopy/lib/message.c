#include <Python.h>
#include <apr_general.h>
#include <apr_strings.h>

#include "message.h"
#include "binparser.h"

static PyObject* proto_message_getattr(PyObject* obj, PyObject* att_name) {
    PyObject* result = PyObject_GenericGetAttr(obj, att_name);
    if (result) {
        return result;
    }
    if (!PyErr_ExceptionMatches(PyExc_AttributeError)) {
        return NULL;
    }
    PyErr_Clear();
    pymessage_t* m = (pymessage_t*)obj;

    if (!m->payload->t) {
        proto_parse_message(m);
    }
    // PyObject* result;
    // switch (m->payload->t) {
    //     case vt_uint32:
    //         result = PyLong_FromUnsignedLong(m->payload->val.uint32);
    //         break;
    // }
    
    PyErr_Format(
        PyExc_AttributeError,
        "field %A is not defined", att_name);
    return NULL;
}

static void proto_message_free(PyObject* message) {
    pymessage_t* m = (pymessage_t*)message;
    free(m->payload);
    Py_DECREF(m->parser);
}

static PyObject* proto_message_repr(PyObject* message) {
    pymessage_t* m = (pymessage_t*)message;

    if (!m->payload->t) {
        proto_parse_message(m);
    }
    Py_RETURN_NOTIMPLEMENTED;
}

static PySequenceMethods _proto_message_as_seq = {
    (lenfunc)0,  // sq_length
    (binaryfunc)0, // sq_concat
    (ssizeargfunc)0, // sq_repeat
    (ssizeargfunc)0, // sq_item
};

static PyTypeObject _proto_message_type = {
  PyVarObject_HEAD_INIT(&PyType_Type, 0)
  "protopy.wrapped.ProtoMessage",     // tp_name
  sizeof(pymessage_t),               // tp_basicsize
  0,                                   // tp_itemsize
  proto_message_free,   // tp_dealloc
  0,                                   // tp_print
  0,                                   // tp_getattr
  0,                                   // tp_setattr
  0,                                   // tp_compare
  proto_message_repr,                                   // tp_repr
  0,                                   // tp_as_number
  &_proto_message_as_seq,              // tp_as_sequence *
  0,                                   // tp_as_mapping
  0,                                   // tp_hash
  0,                                   // tp_call
  0,                                   // tp_str
  proto_message_getattr,  // tp_getattro
  0,                                   // tp_setattro
  0,                                   // tp_as_buffer
  Py_TPFLAGS_DEFAULT,  // tp_flags
  "ProtoMessage wrapper class that holds "
  "a reference to C proto_message",  // tp_doc
  0,                                   // tp_traverse
  0,                                   // tp_clear
  0,                                   // tp_richcompare
  0,                                   // tp_weaklistoffset
  0,                                   // tp_iter
  0,                                   // tp_iternext
  0,                                   // tp_methods
  0,                                   // tp_members
  0,                                   // tp_getset
  0,                                   // tp_base
  0,                                   // tp_dict
  0,                                   // tp_descr_get
  0,                                   // tp_descr_set
  0,                                   // tp_dictoffset
  0,                                   // tp_init
  0,                                   // tp_alloc
  0,                   // tp_new
};

PyTypeObject* proto_message_type = &_proto_message_type;

PyObject* proto_message_from_bytes(PyObject* self, PyObject* args) {
    PyObject* parser;
    const char* ptype;
    Py_buffer bytes;
    if (!PyArg_ParseTuple(args, "Oyy*", &parser, &ptype, &bytes)) {
        return NULL;
    }
    PyObject* result = proto_message_type->tp_alloc(proto_message_type, 0);
    Py_INCREF(result);
    message_t* m = malloc(sizeof(message_t));
    m->t = false;
    str_t* unparsed = malloc(sizeof(str_t));
    char* s = malloc(bytes.len * sizeof(char));
    memcpy(s, bytes.buf, bytes.len * sizeof(char));
    unparsed->s = s;
    unparsed->n = bytes.len;
    m->val.bytes = *unparsed;
    ((pymessage_t*)result)->payload = m;
    ((pymessage_t*)result)->parser = parser;
    Py_INCREF(parser);
    return result;
}
