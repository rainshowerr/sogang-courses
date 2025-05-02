#include "conditional.h"

int conditional(int a, int b, int c) {
    int flag;

    flag = !a << 31 >> 31;
    return (flag & c) | (~flag & b);
}