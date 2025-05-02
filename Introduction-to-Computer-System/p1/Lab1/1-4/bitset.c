#include "bitset.h"

static int	ft_pow(int base, int exp) {
	int res = 1;

	for (int i = 0; i < exp; i++)
		res *= base;
	return (res);
}

void addNumber(unsigned char* set, int x) {
	int a = x / 8, b = x % 8;

    if ((set[a] << b) < 0)
		  return;
	set[a] += ft_pow(2, 7 - b);
}