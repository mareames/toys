//
// misc_sorts.c
//
// insertion sort (non recursive)
// 
// Copyright (c) 2019, 2020, Martin Reames
//

#include "sorts.h"

/* basic insertion sort */
void insertion_sort(long *data, uint lo_ix, uint hi_ix)
{
  uint i, j;
  long tmp_elem;

  for(i = lo_ix + 1; i <= hi_ix; i++)
  {
    tmp_elem = data[i];
    j = i;

    while ((j > lo_ix) && (compare(&(data[j - 1]), &tmp_elem) > 0))
    {
      data[j] = data[j - 1];
      j--; 
    }

    data[j] = tmp_elem;
  }
}

