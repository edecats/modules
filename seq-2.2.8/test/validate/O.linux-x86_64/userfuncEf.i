# 1 "../userfuncEf.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../userfuncEf.st"






program userfuncEfTest

%%#include "../testSupport.h"

option +s;

evflag x;

%{
static void efSet_x(SS_ID ssId, EV_ID x)
{
    seq_efSet(ssId, x);
}
}%

entry {
    seq_test_init(2);
}

ss myss {
    state check_not_set {
        when (efTestAndClear(x)) {
            testFail("argh, no timeout!");
        } state set
        when (delay(0.5)) {
            testPass("cool, we came here");
        } state set
    }
    state set {
        when () {
            efSet_x(ssId, x);
        } state check_set
    }
    state check_set {
        when (efTestAndClear(x)) {
            testPass("cool, we came here");
        } exit
        when (delay(0.5)) {
            testFail("argh, timeout!");
        } exit
    }
}

exit {
    seq_test_done();
}
