# 1 "../version.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../version.st"
program versionTest

# 1 "../../../include/seq_release.h" 1
# 4 "../version.st" 2
%%#include <stdlib.h>

entry {

    printf("conditional compilation in\n");

    printf("seq version is 2.2 or greater\n");





%{

    printf("seq version is 2.2 or greater\n");



}%


%%
%%#if 2002008 >= 2002000
%% printf("seq version is 2.2 or greater\n");
%%#else
%% printf("seq version is 2.1 or less\n");
%%#endif
}

ss x { state x { when (delay(1)) {exit(0);} exit } }
