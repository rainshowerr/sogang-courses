#ifndef MP2_H
#define MP2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

void	selection(int *arr, int num);
int		partition(int *arr, int p, int r);
void	quick(int *arr, int p, int r);
void	merge(int *arr, int p, int r);
void	mysort(int *arr, int p, int r, int depth);
void	swap(int *a, int *b);

#endif