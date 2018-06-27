CC := gcc
YACC := yacc
LEX := lex
SOURCES := $(wildcard ./*.c)
TEST_SOURCES := $(wildcard ./test_*.c)
SOURCES := $(filter-out $(TEST_SOURCES), $(SOURCES))
YACC_SOURCES := $(wildcard ./*.y)
LEX_SOURCES := $(wildcard ./*.l)
APR_INCLUDES := /usr/include/apr-1.0
OBJECTS := $(SOURCES:.c=.o)
YACC_GENERATED := $(YACC_SOURCES:.y=.tab.c)
LEX_GENERATED := $(LEX_SOURCES:.l=.lex.c)
CFLAGS := -fno-diagnostics-color -Wall -Wno-unused-function -I. -fPIC -g3
PYCFLAGS := $(shell python3.6-config --cflags)
PYCLDFLAGS := $(shell python3.6-config --ldflags)
PROGRAM := protopy

default: program

%.o: %.c
	$(CC) $(CFLAGS) $(PYCFLAGS) -I$(APR_INCLUDES) -c $< -o $@

%.tab.c: %.y
	$(YACC) -t -vd $< -o $@
	$(CC) $(CFLAGS) $(PYCFLAGS) -c $@ -o $(@:.c=.o)

%.lex.c: %.l
	$(LEX) -d --header-file=$(@:.c=.h) -o $@ $<
	$(CC) $(CFLAGS) $(PYCFLAGS) -c $@ -o $(@:.c=.o)

program: $(YACC_GENERATED) $(LEX_GENERATED) $(OBJECTS)
	$(CC) $(CFLAGS) $(PYCLDFLAGS) $(OBJECTS) \
	$(YACC_GENERATED:.c=.o) $(LEX_GENERATED:.c=.o) \
	-I$(APR_INCLUDES) -o $(PROGRAM)

clean:
	rm -f $(OBJECTS)
	rm -f $(PROGRAM)
	rm -f *.tab.*
	rm -f *.lex.*
	rm -f test_list

check-syntax:
	$(CC) -o nul -S $(SOURCES)

clean_test_list:
	rm -f test_list
	rm -f list.o

test_list: clean_test_list list.o
	$(CC) $(CFLAGS) list.o test_list.c -lm -o test_list
