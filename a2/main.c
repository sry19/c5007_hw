#include <stdio.h>
#include "sort.h"
#include <stdlib.h>
#include <stdbool.h>

int main() {
  srand(1);
  printf("======================\n");
  printf("sorting\n");
  printf("======================\n");
  bool ifprinted = false;
  for (int k = 1; k < 8; k++) {
    int array[1<<k];
    int length = 1<<k;
    int comparisons = 0;
    for (int i = 0; i < length; i++) {
      array[i] = rand() % 100;
    }
    if (ifprinted) {
    for (int i = 0; i < length; i++) {
    printf("%d,", array[i]);
    }
    printf("\n");}
    comparisons = Sort(array, length);
    if (ifprinted) {
    for (int i = 0; i < length; i++) {
      printf("%d,", array[i]);
    }
    printf("\n");}
    if (k == 3) {
      printf("small, %d numbers,%d comparisons\n", length, comparisons);
    }
    if (k == 5) {
      printf("medium, %d numbers,%d comparisons\n", length, comparisons);
    }
     if (k == 7) {
      printf("large, %d numbers,%d comparisons\n", length, comparisons);
    }
  }
}
