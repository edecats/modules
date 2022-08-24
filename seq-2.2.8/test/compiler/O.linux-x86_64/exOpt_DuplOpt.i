# 1 "../exOpt_DuplOpt.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../exOpt_DuplOpt.st"




program exOpt_DuplOptTest
float v;
assign v to "grw:xxxExample";
monitor v;

ss ss1 {
    state low {
        option + t;
        entry {
            printf("Will do this on entry");
            printf("Another thing to do on entry");
        }
        when(v > 5.0) {
            printf("now changing to high\n");
        } state high
        when(delay(.1)) {
        } state low
        exit {
            printf("Something to do on exit");
        }
    } state high {
        option - t;
        option + t;
        when(v <= 5.0) {
            printf("changing to low\n");
        } state low
        when(delay(.1)) {
        } state high
    }
}
