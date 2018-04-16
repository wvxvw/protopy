#include <stdio.h>
#include <string.h>
#include <Python.h>
#include "binparser.h"

int main(int argc, char** argv) {
    /* if (argc < 2) { */
    /*     fprintf(stderr, "Proto payload required as first argument\n"); */
    /*     return 1; */
    /* } */
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }
    Py_SetProgramName(program);  /* optional but recommended */
    Py_Initialize();
    
    /* parse_state state; */
    /* state.in = argv[1]; */
    /* state.available = strlen(argv[1]); */
    /* state.pos = 0; */
    /* state.buffer_pos = 0; */
    /* size_t parsed = parse(&state); */
    /* printf("parsed bytes: %zu\n", parsed); */
    Py_Finalize();
    PyMem_RawFree(program);
    return 0;
}
