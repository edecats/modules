# 1 "../funcdef.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../funcdef.st"
program funcdefTest

option +r;

int x;
assign x;

void *f1(void)
{
    pvPut(x);
    return 0;
}

void *f1a()
{
    return 0;
}


void pvSetX(int val);

ss simple {
    state simple {
        when () {
            void *p = f1();
            p = f2(p,(double **)f1a);
            pvSetX(1.0);
        } exit
    }
}

void *f2(void *x, double **d)
{
    void *(*f)(void) = (void *(*)(void))x;
    return f();
}

void pvSetX(int val)
{
    pvPut(x,val);
    printf("x=%d, val=%d\n", x, val);
}
