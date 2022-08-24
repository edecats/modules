# 1 "../traffic.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../traffic.st"
program traffic





int lights[3];
assign lights to {
    "traffic:red_light",
    "traffic:yellow_light",
    "traffic:green_light"
};

double red_time = 3.0;
double red_yellow_time = 1.0;
double green_time = 4.0;
double yellow_time = 1.0;

ss light {
    state red {
        entry {
            lights[0]=1; pvPut(lights[0]);
            lights[1]=0; pvPut(lights[1]);
            lights[2]=0; pvPut(lights[2]);
        }
        when (delay(red_time)) {
        } state red_yellow
    }
    state red_yellow {
        entry {
            lights[0]=1; pvPut(lights[0]);
            lights[1]=1; pvPut(lights[1]);
            lights[2]=0; pvPut(lights[2]);
        }
        when (delay(red_yellow_time)) {
        } state green
    }
    state green {
        entry {
            lights[0]=0; pvPut(lights[0]);
            lights[1]=0; pvPut(lights[1]);
            lights[2]=1; pvPut(lights[2]);
        }
        when (delay(green_time)) {
        } state yellow
    }
    state yellow {
        entry {
            lights[0]=0; pvPut(lights[0]);
            lights[1]=1; pvPut(lights[1]);
            lights[2]=0; pvPut(lights[2]);
        }
        when (delay(yellow_time)) {
        } state red
    }
}
