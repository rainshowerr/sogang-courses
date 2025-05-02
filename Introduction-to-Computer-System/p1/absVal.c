#include "absVal.h"

int absVal(int num) {
    int sign;

    sign = num >> 31 << 31 >> 31;
    return (~sign & num) | (sign & (~num + 1));
}