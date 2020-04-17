//
// mergesort.c
//
// various optimizations of merge sort
//
// Copyright (c) 2019, 2020, Martin Reames
//

#include <stdlib.h>
#include <math.h>
#include <string.h> /* memcpy */
#include "sorts.h"

// merge sort subroutine
//
// input is an array partitioned into two sorted lists
//
//   [lo_ix .. mid] [mid + 1 .. hi_ix]
//
// plus optional tmp array so we don't have to allocate it every time
//
// output is the merge of these two lists
static void
merge(long *data, long *tmpdata, uint lo_ix, uint hi_ix)
{
  long  *tmp;
  uint   t; // tmp list iterator
  uint   mid = lo_ix + ((hi_ix - lo_ix) / 2);
  uint   nelts = hi_ix - lo_ix + 1;
  uint   i; // list #1 [lo_ix ... mid] iterator
  uint   j; // list #2 [mid + 1 ... hi_ix] iterator
  uint   imax = mid;
  uint   jmax = hi_ix;

  if (tmpdata == NULL) {
    tmp = calloc(nelts, sizeof(long));
  }
  else {
    tmp = tmpdata;
  }

  i = lo_ix;
  j = mid + 1;
  t = 0;

  while (i <= imax && j <= jmax) {
    if (compare(&data[i], &data[j]) < 0) {
      tmp[t] = data[i];
      i++;
    }
    else {
      tmp[t] = data[j];
      j++;
    }
    t++;
  }

  // copy rest of list #1 (if any)
  while (i <= imax) {
    tmp[t] = data[i];
    i++;
    t++;
  }

  // copy rest of list #2 (if any)
  while (j <= jmax) {
    tmp[t] = data[j];
    j++;
    t++;
  }

  // now copy back tmp on top of the elements we sorted
  memcpy((void*)&data[lo_ix], (void*)&tmp[0], nelts * sizeof(long));
  //
  // for (i = lo_ix, t = 0; t < nelts; i++, t++)
  //   data[i] = tmp[t];

  // free the tmp array if we allocated it above
  if (tmpdata == NULL)
    free(tmp);
}

// basic merge sort (recursive)
void
merge_sort(long *data, uint lo_ix, uint hi_ix)
{
  uint mid = lo_ix + ((hi_ix - lo_ix) / 2);

  // base case #1 (one list element): done
  if (lo_ix == hi_ix)
    return;

  // base case #2: (two list elements): swap elements if needed, then done
  if (lo_ix + 1 == hi_ix) {
    if (compare(&data[lo_ix], &data[hi_ix]) > 0)
      swap_elem(&data[lo_ix], &data[hi_ix]);
    return;
  }

  merge_sort(data, lo_ix, mid);
  merge_sort(data, mid + 1, hi_ix);

  merge(data, NULL, lo_ix, hi_ix);
}

// slightly optimized merge sort (recursive)
void
merge_sort_opt(long *data, long *tmpdata, uint lo_ix, uint hi_ix)
{
  uint mid = lo_ix + ((hi_ix - lo_ix) / 2);

  // OPTIMIZATION: use insertion sort for a small number of elements
  if ((hi_ix - lo_ix + 1) < MIN_MERGE_SORT_NELTS) {
    insertion_sort_opt(data, lo_ix, hi_ix);
    return;
  }

  merge_sort(data, lo_ix, mid);
  merge_sort(data, mid + 1, hi_ix);

  // OPTIMIZATION: use tmpdata array so we can avoid calloc() + free()
  // for every merge operation
  merge(data, tmpdata, lo_ix, hi_ix);
}
