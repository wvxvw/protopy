#include <Python.h>
#include <apr_general.h>
#include <apr_strings.h>

#include "message.h"

static PyObject* proto_message_getattr(PyObject* obj, PyObject* att_name) {
    Py_RETURN_NONE;
}

static void proto_message_free(PyObject* message) {

}

static PyObject* proto_message_repr(PyObject* message) {
    pymessage_t* m = (pymessage_t*)message;

    switch (m->payload->t) {
        case vt_uint32:
            return PyUnicode_FromFormat("%d", m->payload->val.vuint32);
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }
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

PyObject* proto_message_from_bytes(PyObject* parser, PyObject* bytes) {
    PyObject* result = proto_message_type->tp_alloc(proto_message_type, 0);
    Py_INCREF(result);
    message_t* m = malloc(sizeof(message_t));
    m->t = vt_uint32;
    m->val.vuint32 = 42;
    ((pymessage_t*)result)->payload = m;
    return result;
}
