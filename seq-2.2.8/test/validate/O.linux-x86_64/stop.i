# 1 "../stop.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../stop.st"






program stopTest

%%#include <stdlib.h>
%%#include "../testSupport.h"

entry {
    seq_test_init(1);
}

ss main {
    state start {
        when (delay(0.5)) {
            seqStop(epicsThreadGetIdSelf());
        } state final
    }
    state final {
        when (FALSE) {
        } state final
    }
}

ss progress {
    state running {
        when (delay(1)) {
            testFail("still running...\n");
            seq_test_done();
            exit(0);
        } state running
    }
}

exit {
    testPass("program terminated");
    seq_test_done();
}
