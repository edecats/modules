# 1 "../pvAssignSubst.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../pvAssignSubst.st"
program pvAssignSubstTest("pre=pv,post=Subst")

%%#include "../testSupport.h"

string x;
assign x;

entry {
    seq_test_init(1);
}

ss test {
    state one {
        entry {
            pvAssignSubst(x, "{pre}Assign{post}.NAME");
        }
        when (pvConnected(x)) {
            pvGet(x,SYNC);
            testOk1(strcmp(x,"pvAssignSubst")==0);
        } exit
        when (delay(2)) {
            testFail("timeout");
        } exit
    }
}

exit {
    seq_test_done();
}
