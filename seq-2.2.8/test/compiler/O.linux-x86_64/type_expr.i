# 1 "../type_expr.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../type_expr.st"






program type_exprTest

option +r;

entry {
    long n = 0;
    void *p = (void *)n;
    void **pp = (void**)n;
    void (*pf1)(void) = (void (*)(void))n;
    void (*pf2)(int x,int) = (void (*)(int,int))n;
    void (*pf3)(int*) = (void (*)(int*))n;
    void (*pf4)(int**) = (void (*)(int**))n;
    void (*pf5)(double**,int) = (void (*)(double**,int))n;
    void (*pf6)(double**,int(**)[2]) = (void (*)(double**,int(**)[2]))n;
    int const c = (int const)n;
    int const *pc = (int const *)n;
    unsigned long *const cp = (unsigned long *const)n;
    double **pd = (double**)(int*)n;
    double **ppd = (double**)(int(*)[3])n;
    void (*lf)(
        void *p,
        void **pp,
        void (*pf1)(void),
        void (*pf2)(int x,int),
        void (*pf3)(int*),
        void (*pf4)(int**),
        void (*pf5)(double**,int),
        void (*pf6)(double**,int(**)[2]),
        int const c,
        int const *pc,
        unsigned long *const cp,
        double **pd,
        double **ppd
    ) = (void (*)(
        void *p,
        void **pp,
        void (*pf1)(void),
        void (*pf2)(int x,int),
        void (*pf3)(int*),
        void (*pf4)(int**),
        void (*pf5)(double**,int),
        void (*pf6)(double**,int(**)[2]),
        int const c,
        int const *pc,
        unsigned long *const cp,
        double **pd,
        double **ppd
    ))n;
    lf(p,pp,pf1,pf2,pf3,pf4,pf5,pf6,c,pc,cp,pd,ppd);
}

# 1 "./../simple.st" 1






ss simple {
    state simple {
        when () {} exit
    }
}
# 59 "../type_expr.st" 2
