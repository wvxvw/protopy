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
    printf("some string: %s\n", str_str(cstr_bytes("Whatever 123")));
    printf("some escapes: %s\n", str_str(cstr_bytes("Whatever\n\x05\r\t 123")));
    printf("some int: %s\n", int_str(&foo));
    printf("some list: %s\n", str(elts));
    elts2 = from_strings(5, "a", "b", "c", "d", "e"); // duplicate(elts);
    printf("elts2: %s\n", str(elts2));
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
        rope = cons(rope, tlist, cons(cstr_bytes(test_strings[9 - i]), tstr, nil));
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

    list weird_characters = cons_str(
        "\x10{\"\t\t\xd7\x11\x00\x00\x00\x00\x00\x00*\x07\x08\x87\x01\xa0\x01\xb1\x05",
        22,
        nil);

    weird_characters = cons_str("abc\000abc", 7, weird_characters);
    printf("weird_characters: %s\n", str(weird_characters));

    list weird_rope = cons_str("abc\000\000\000cba", 9, nil);
    list weird_remainder;
    char* abc = malloc(4 * sizeof(char));
    rope_read(weird_rope, abc, 3, &weird_remainder);
    abc[3] = '\0';
    printf("weird_remainder: %s\n", str(weird_remainder));
    printf("abc: %s\n", abc);
    return 0;
}
