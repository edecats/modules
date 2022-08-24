# 1 "../exitOptx.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../exitOptx.st"




program exitOptxTest

%%#include "../testSupport.h"

entry {
    seq_test_init(4);
}

ss opt_x {
    int exit1cnt = 0;
    int exit2cnt = 0;
    int delayed1 = 0;
    int delayed2 = 0;
    state plus_x {
        when (delay(0.1)) {
            delayed1++;
        } state plus_x
        when (delayed1) {
        } state minus_x
        exit {
            testOk1(exit1cnt==0);
            exit1cnt++;
        }
    }
    state minus_x {
        option -x;
        when (delay(0.1)) {
            delayed2++;
        } state minus_x
        when (delayed2 > 1) {
            testOk1(exit2cnt == delayed2);
        } exit
        exit {
            exit2cnt++;
            testOk1(exit2cnt == delayed2);
        }
    }
}

exit {
    seq_test_done();
}
