// Write a C program called array2.c that has an array of 400 integers in the function of main that is dynamically allocated.
#include <stdlib.h>
int main() {
  int *p;
  p = (int*)malloc(400 * sizeof(int));
  p[0] = 712;
  p[399] = 1;
  p[66] = 35;
  free(p);
}
