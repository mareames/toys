//
// quick.c
//
// various optimizations of quicksort
//
// Copyright (c) 2019, 2020, Martin Reames
//

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include "sorts.h"

void *qsort_core_thread(void *arg);

// basic quicksort (recursive)
void quicksort(long *data, uint lo_ix, uint hi_ix)
{
  int i, j;
  long pivot_elem;

  // base case #1 (one list element): done
  if (lo_ix == hi_ix)
    return;

  // base case #2: (two list elements): swap elements if needed, then done
  if (lo_ix + 1 == hi_ix) {
    if (compare(&data[lo_ix], &data[hi_ix]) > 0)
      swap_elem(&data[lo_ix], &data[hi_ix]);
    return;
  }

  pivot_elem = data[lo_ix + ((hi_ix - lo_ix) >> 1)];

  i = lo_ix;
  j = hi_ix;
  while (i <= j) {
    while (compare(&data[i], &pivot_elem) < 0)
      i++;
    while (compare(&data[j], &pivot_elem) > 0)
      j--;

    if (i <= j) {
      swap_elem(&data[i], &data[j]);
      i++;
      j--;
    }
  }

  if (lo_ix < j)
    quicksort(data, lo_ix, j);

  if (i < hi_ix)
    quicksort(data, i, hi_ix);

  return;
}

// core subroutine of quicksort_opt
void qsort_core(long *data, int lo_ix, int hi_ix,
                unsigned short call_depth, unsigned short max_call_depth,
                bool multithread)
{
  int i, j, lsize, rsize;
  long pivot_elem;
  pthread_t cthread;
  qsort_info_t cthread_info;
  int rc;
  bool spawn_threads;

  // OPTIMIZATION: use insertion sort for a small number of elements
  if ((hi_ix - lo_ix) < MIN_QUICKSORT_NELTS) {
    insertion_sort(data, lo_ix, hi_ix);
    return;
  }

  // if the recursion depth is too big, use heap sort
  if (call_depth > max_call_depth) {
    heapsort(&data[lo_ix], lo_ix, hi_ix);
    return;
  }

  pivot_elem = data[lo_ix + ((hi_ix - lo_ix) >> 1)];

  i = lo_ix;
  j = hi_ix;
  while (i <= j) {
    while (compare(&data[i], &pivot_elem) < 0)
      i++;
    while (compare(&data[j], &pivot_elem) > 0)
      j--;

    if (i <= j) {
      swap_elem(&data[i], &data[j]);
      i++;
      j--;
    }
  }

  lsize = (lo_ix < j) ? (j - lo_ix) : 0;
  rsize = (i < hi_ix) ? (hi_ix - j) : 0;
  spawn_threads =
    multithread &&
    (lsize > QSORT_THREAD_THRESHOLD) &&
    (rsize > QSORT_THREAD_THRESHOLD);

  // case 1: we've spawned too many threads already, so recursively
  // sort the sub-arrays in the current thread
  if (!spawn_threads) {
    if (lo_ix < j)
      qsort_core(data, lo_ix, j, call_depth + 1, max_call_depth, spawn_threads);

    if (i < hi_ix)
      qsort_core(data, i, hi_ix, call_depth + 1, max_call_depth, spawn_threads);
  }
  // case 2: create new thread to recursively sort the left sub-array
  else {
    // create a child thread to sort the left sub-array
    cthread_info.data = data;
    cthread_info.lo_ix = lo_ix;
    cthread_info.hi_ix = j;
    cthread_info.call_depth = call_depth + 1;
    cthread_info.max_call_depth = max_call_depth;

    rc = pthread_create(&cthread, NULL, &qsort_core_thread, &cthread_info);
    assert(rc == 0);

    // sort the right sub-array
    qsort_core(data, i, hi_ix, call_depth + 1, max_call_depth, spawn_threads);

    // wait for the child to finish
    pthread_join(cthread, NULL);
  }

  return;
}

// multi-threaded subroutine of quicksort_opt
void *qsort_core_thread(void *arg)
{
  qsort_info_t *qsort_info = (qsort_info_t *) arg;

  qsort_core(qsort_info->data, qsort_info->lo_ix, qsort_info->hi_ix,
             qsort_info->call_depth, qsort_info->max_call_depth, true);

  return NULL;
}

// optimized, possibly multi-threaded version of quicksort
void quicksort_opt(long *data, uint lo_ix, uint hi_ix,
                   bool multithread)
{
  qsort_core(data, (int) lo_ix, (int) hi_ix, 0,
             (unsigned short) (2 * 3.32 * log10((double) (hi_ix - lo_ix + 1))),
             multithread);
}
