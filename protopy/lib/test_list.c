#include <stdio.h>
#include <stdlib.h>
#include "list.h"


int main(int argc, char** argv) {
    int foo = 1234567;
    size_t i = 10;
    list elts = nil;
    list elts2 = nil;
    int* elt;

    printf("empty list: %p\n", elts);
    while (i--) {
        elt = malloc(sizeof(int));
        *elt = i;
        elts = cons(elt, tint, elts);
    }
    printf("some string: %s\n", str_str("Whatever 123"));
    printf("some escapes: %s\n", str_str("Whatever\n\x05\r\t 123"));
    printf("some int: %s\n", int_str(&foo));
    printf("some list: %s\n", str(elts));
    elts2 = from_strings(5, "a", "b", "c", "d", "e"); // duplicate(elts);
    printf("nreversed(elts): %s\n", str(nreverse(elts)));
    printf("reversed(elts2): %s\n", str(reverse(elts2)));
    printf("elts2 after reversed(elts2): %s\n", str(elts2));
    printf("elts2 after reversed(elts2): %s\n", str(elts2));
    elts = duplicate(elts2);
    printf("elts after reversed(elts): %s\n", str(elts));
    printf("append(elts2, elts): %s\n", str(append(elts2, elts)));
    printf("elts2 after append(elts2, elts): %s\n", str(elts2));
    printf("elts after append(elts2, elts): %s\n", str(elts));
    printf("elts after duplicate(elts): %s\n", str(duplicate(elts)));
    printf("append(elts, elts2): %s\n", str(append(elts, elts2)));
    del(elts);
    del(elts2);

    list ints = from_ints(5, 1, 2, 3, 4, 5);
    printf("ints: %s\n", str(ints));
    list strings = from_strings(3, "foo", "bar", "baz");
    printf("strings: %s\n", str(strings));
    list lists = from_lists(2, ints, strings);
    printf("lists: %s\n", str(lists));

    printf("append(ints, strings): %s\n", str(append(ints, strings)));
    return 0;
}
