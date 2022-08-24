# 1 "../sizeof.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../sizeof.st"






program sizeofTest

%%#include "../testSupport.h"

entry {
    seq_test_init(9);
}

ss test {
    state sizes {
        when () {
            char c;
            char *p = &c;
            testOk1(sizeof(char)==1);
            testOk1(sizeof(*p)==1);
            testOk1(sizeof(string) >= 40);
            testOk1(sizeof(int8_t) == 1);
            testOk1(sizeof(uint8_t) == 1);
            testOk1(sizeof(int16_t) == 2);
            testOk1(sizeof(uint16_t) == 2);
            testOk1(sizeof(int32_t) == 4);
            testOk1(sizeof(uint32_t) == 4);
            c = *p;
        } exit
    }
}

exit {
    seq_test_done();
}
