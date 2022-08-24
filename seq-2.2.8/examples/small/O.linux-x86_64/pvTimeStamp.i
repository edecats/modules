# 1 "../pvTimeStamp.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../pvTimeStamp.st"
program pvTimeStampTest

int test = 0;
assign test to "pvTimeStampTest";
monitor test;

ss one {
    state one {
        when (delay(1)) {
            printf("sec=%d\n",pvTimeStamp(test).secPastEpoch);
            test++;
            pvPut(test,SYNC);
        } state one
    }
}
