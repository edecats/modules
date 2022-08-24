# 1 "../pvGetComplete.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../pvGetComplete.st"






program pvGetCompleteTest

%%#include "../testSupport.h"



entry {
    seq_test_init(1);
    testDiag("start");
}

ss pvGetComplete {
    int r[2];
    int w[2];
    assign r to {"pvGetComplete1","pvGetComplete2"};
    assign w to {"pvGetComplete1","pvGetComplete2"};
    int n = 0;

    state count {
        when (n < 20000) {
            n++;
            if (n%1000==0) {
                testDiag("passed %d runs", n);
            }
        } state test
        when () {
            testPass("%d pvGets completed in time and results agree with predictions", 2*20000);
        } exit
    }
    state test {
        entry {
            int i;
            w[0] = n;
            w[1] = -n;
            for (i=0; i<2; i++) {
                r[i] = 0;
                pvPut(w[i],SYNC);
            }
            for (i=0; i<2; i++) {
                pvGet(r[i],ASYNC);
            }
        }
        when (delay(2)) {
            testFail("pvGet completion timeout");
        } state fail
        when (pvGetComplete(r[0]) && pvGetComplete(r[1])) {
            int i;
            for (i=0; i<2; i++) {
                if (r[i] != w[i]) {
                    testFail("n=%d, i=%d, w=%d, r=%d", n, i, w[i], r[i]);
                    state fail;
                }
            }
        } state count
    }
    state fail {
        when () {
        } exit
    }
}

exit {
    testDiag("exit");
    seq_test_done();
}
