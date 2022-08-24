# 1 "../parameter.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../parameter.st"



program parameterTest("var=default")

ss one {
    state one {
        when() {
            printf("var=%s\n",macValueGet("var"));
        } exit
    }
}
