# 1 "../sync_not_monitored.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../sync_not_monitored.st"






program p

float x = 0.0;
assign x;
evflag f;
sync x to f;

ss simple {
    state simple {

        when (x==x) {
            efSet(f);
        } exit
    }
}
