//
// countingsort.c
//
// simple implementation of counting sort
//
// Copyright (c) 2020, Martin Reames
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> /* memset */
#include <assert.h>
#include "sorts.h"

// counting sort subroutine
//
// input params
//
// data: array of unsorted integers
// data_len: # of elements in data
// tmpdata: array of integers for counting values in data
// maxval: maximum value that is stored in data (minval is always 0)
static void
count_values(long *data, uint data_len, uint *tmpdata, uint maxval)
{
  uint i;
  long val;
  uint tmpsz = maxval+1;

  // first memset tmpdata
  memset(tmpdata, 0, tmpsz * sizeof(long));

  // now fill tmpdata with counts of values in data
  for (i = 0; i < data_len; i++) {
    val = (uint)data[i];
    tmpdata[val]++;
  }
}

// counting sort
//
// input params
//
// . data: array of unsorted integers
// . lo_ix: smallest element in data array (typically 0)
// . hi_ix: highest element in data array (typically sizeof(data)-1)
// . maxval: maximum value stored in array (minval is always 0)
//
// output:
//
//   data is sorted

void
counting_sort(long *data, uint lo_ix, uint hi_ix, uint maxval)
{
  long nvals = maxval+1;
  uint nelts = hi_ix - lo_ix + 1;
  uint i, j;
  uint val;
  uint count;

  uint *tmpdata = calloc(nvals, sizeof(uint));

  // first we count how many copies there are of each value;
  // store into tmpdata
  count_values(data, nelts, tmpdata, maxval);

  // now we walk through the tmpdata array and use it to overwrite the
  // data array with the sorted numbers

  // iterate over tmpdata array
  for (i = 0, j = 0; i <= nvals; i++) {

    // fill in data array
    val = i;
    count = tmpdata[val];
    while (count > 0) {
      data[j] = val;
      count--;
      j++;
    }
  }

  // sanity check: make sure we wrote the correct number of
  // elements into data[]
  assert(j == nelts);

  // free our temp storage
  free(tmpdata);
}
