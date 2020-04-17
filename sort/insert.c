//
// insert.c
//
// insertion sort (non recursive)
//
// Copyright (c) 2019, 2020, Martin Reames
//

#include <string.h> /* memmove */
#include <stdio.h>
#include "sorts.h"

// basic insertion sort, no binary search
void insertion_sort(long *data, uint lo_ix, uint hi_ix)
{
  uint i, j;
  long tmp_elem;

  for (i = lo_ix + 1; i <= hi_ix; i++) {
    tmp_elem = data[i];
    j = i;

    while ((j > lo_ix) && (compare(&(data[j - 1]), &tmp_elem) > 0)) {
      data[j] = data[j - 1];
      j--;
    }

    data[j] = tmp_elem;
  }
}

// use binary search to find the place for val in the
// array data[lo_ix ... hi_ix]
int bsearch_find_idx(long *data, uint lo_ix, uint hi_ix, long val)
{
  uint idx;
  long loc;
  uint hi = hi_ix;
  uint lo = lo_ix;

  // handle base cases where val is < all elts or > all elts

  if (val <= data[lo]) {
    idx = lo;
  }
  else if (val >= data[hi]) {
    idx = hi+1;
  }
  else {

    // find correct location for the element in the sorted array
    while (hi > lo) {

      idx = lo + (hi - lo) / 2;
      loc = data[idx];

      if (val > loc) {
        lo = idx+1;
      }
      else if (val < loc) {
        hi = idx-1;
      }
      else {   // (val == data[idx])
        break; // this is where we belong!
      }

      // see if it's time to break out of the loop early
      if (val <= data[lo]) {
        idx = lo;
        break;
      }
      else if (val >= data[hi]) {
        idx = hi+1;
        break;
      }
    }
  }

  return idx;
}

// improved insertion sort, uses binary search and memmove
void insertion_sort_opt(long *data, uint lo_ix, uint hi_ix)
{
  uint i, j;
  long tmp_elem;
  uint idx;

  for (i = lo_ix+1; i <= hi_ix; i++) {
    tmp_elem = data[i];

    idx = bsearch_find_idx(data, 0, i-1, tmp_elem);

    // if this is not the largest element found so far
    // then we have to move part of the existing array
    // elements to make space for it; otherwise we
    // don't have to move it, it's already where it belongs
    if (idx < i) {
      memmove(&data[idx+1], &data[idx], sizeof(long)*(i - 1 - idx + 1));
      data[idx] = tmp_elem;
    }
  }
}

