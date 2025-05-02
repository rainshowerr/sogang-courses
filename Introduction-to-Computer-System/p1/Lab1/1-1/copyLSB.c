#include "copyLSB.h"

int copyLSB(int num) {
    return num << 31 >> 31;
}