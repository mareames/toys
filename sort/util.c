//
// util.c
//
// misc utility functions
//
// Copyright (c) 2019, 2020, Martin Reames
//

#include <stdlib.h>
#include <stdio.h>  /* printf */
#include <math.h>
#include <string.h> /* memcmp */
#include "sorts.h"

// validate that the input data is actually sorted
bool check_sort(long *data, uint len)
{
  uint i;
  bool sorted = true;

  /* NB: i starts at 1 not 0 because we do i-1 in loop */
  for (i = 1; i < len; i++) {
    if (data[i] < data[i - 1]) {
      sorted = false;
      break;
    }
  }

  if (!sorted)
    print_array(data, len, i);

  return sorted;
}

// validate that the two input data arrays are identical
bool check_sort_cmp(long *data1, long *data2, uint len)
{
  uint i;

  return (memcmp(data1, data2, len * sizeof(long)) == 0);
}

// print out the array (for debugging)
void print_array(long *data, uint len, int badelt)
{
  const uint ELTS_PER_LINE = 1;
  uint i;

  if (badelt > 0)
    printf("bad = %d\n\n", badelt);

  for (i = 0; i < len; i++) {
    printf("%ld ", data[i]);
    if (i % ELTS_PER_LINE == (ELTS_PER_LINE-1))
      printf("\n");
  }

  printf("\n\n");
}


// compare left and right (both are long*), returning -1, 0, 1
int compare(const void *left, const void * right)
{
  return ( *(long*) left - *(long*) right );

/*
  if ((*((long*) left)) < (*((long*) right)))
    return -1;
  else if ((*((long*) left)) == (*((long*) right)))
    return 0;
  else
    return 1;
*/
}


// swap left and right (both are long*)
void swap_elem(void *left, void *right)
{
  long tmp;

  tmp = *((long*) left);
  *((long*) left) = *((long*) right);
  *((long*) right) = tmp;
}



