# 1 "../void.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../void.st"






program voidTest

%%#include "../testSupport.h"

int x = 42;
void *p = &x;

entry {
    seq_test_init(1);
}

ss test {
    state sizes {
        when () {
            testOk1(*(int *)p == x);
        } exit
    }
}

exit {
    seq_test_done();
}
