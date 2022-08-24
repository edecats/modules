# 1 "../namingConflict.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "../namingConflict.st"






program namingConflictTest

ss x {
    state y_z {
        when () {
        } state y_z
    }
}

ss x_y {
    state z {
        when () {
        } state z
    }
}
