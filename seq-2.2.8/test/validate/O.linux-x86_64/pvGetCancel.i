# 1 "../pvGetCancel.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../pvGetCancel.st"






program pvGetCancelTest



%%#include "../testSupport.h"



entry {
    seq_test_init(8*50);
    testDiag("start");
}

ss pvGetCancel {
    int x[2];
    int y[2] = {1,2};
    assign x to { "pvGetCancel1", "pvGetCancel2"};
    assign y to { "pvGetCancel1", "pvGetCancel2"};
    typename pvStat status;
    int n = 0;

    state done {
        when (n < 50) {
            n++;
        } state test
        when (delay(0.2)) {
        } exit
    }
    state test {
        entry {
            testOk1(pvGet(x[0],ASYNC) == pvStatOK);
            testOk1(pvGet(x[1],ASYNC) == pvStatOK);
            pvArrayGetCancel(x,2);
            x[0] = x[1] = 0;
            y[0] = y[1] = 1;
            testOk1(pvPut(y[0],SYNC) == pvStatOK);
            testOk1(pvPut(y[1],SYNC) == pvStatOK);
            testOk1(pvGet(x[0],ASYNC) == pvStatOK);
            testOk1(pvGet(x[1],ASYNC) == pvStatOK);
        }
        when (delay(5.0)) {
            testFail("pvGet completion timeout");
            testSkip(1,"further tests aborted");
        } exit
        when (pvArrayGetComplete(x,2)) {
            testOk1(x[0] == y[0]);
            testOk1(x[1] == y[1]);
        } state done
    }
}

exit {
    testDiag("exit");
    seq_test_done();
}
