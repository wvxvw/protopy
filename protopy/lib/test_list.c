#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int int_cmp(list a, list b) {
    int ai = *(int*)car(a);
    int bi = *(int*)car(b);

    return (ai > bi) - (ai < bi);
}

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

    list rope = nil;
    i = 10;

    char* test_strings[10] = {
        "quick",
        "brown",
        "fox",
        "jumped",
        "over",
        "the",
        "very",
        "lazy",
        "dog",
        ".",
    };
    
    while (i--) {
        rope = cons(rope, tlist, cons(strdup(test_strings[9 - i]), tstr, nil));
    }
    printf("rope created: %s\n", str(rope));
    printf("rope_length(rope): %zu\n", rope_length(rope));

    char* buf = malloc(20 * sizeof(char));
    size_t read = rope_peek(rope, buf, 15);
    printf("rope_peek(rope): %zu, %s\n", read, buf);

    list rest_rope;
    read = rope_read(rope, buf, 15, &rest_rope);
    printf("rope_read(rope): %zu, %s, %s\n", read, buf, str(rest_rope));
    del(rope);
    printf("del(rope): %s\n", str(rest_rope));
    read = rope_read(rest_rope, buf, 15, &rope);
    printf("rope_read(rest_rope): %zu, %s, %s\n", read, buf, str(rope));
    del(rest_rope);
    read = rope_read(rope, buf, 15, &rest_rope);
    printf("rope_read(rope): %zu, %s, %s\n", read, buf, str(rest_rope));

    list repeated_fives = from_ints(5, 5, 5, 5, 5, 5);
    list repeated_nines = from_ints(5, 9, 9, 9, 9, 9);
    list sorted_ints = from_ints(5, 1, 2, 4, 3, 5);
    list sorted = sort_unique(append(sorted_ints, append(repeated_fives, repeated_nines)), int_cmp);
    printf("sort_unique(append(repeated_fives, repeated_nines)): %s\n", str(sorted));
    del(repeated_fives);
    del(repeated_nines);
    del(sorted_ints);
    printf("sorted after deleting sources: %s\n", str(sorted));
    return 0;
}
