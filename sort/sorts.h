//
// sorts.h
//
// declaration of all the sorting routines
//
// Copyright (c) 2019, 2020, Martin Reames
//

#ifndef SORTS_H
#define SORTS_H

#include <stdbool.h>

enum { K                      = 1024,
       M                      = K * K,
       DEFAULT_NELTS          = 100 * M,
       MAX_INSERT_SORT_NELTS  = 256 * K,
       MIN_MERGE_SORT_NELTS   = 32,
       MIN_QUICKSORT_NELTS    = 32,
       QSORT_THREAD_THRESHOLD = 65536
};

typedef unsigned int    uint;
typedef unsigned short  ushort;

typedef struct
{
  long  *data;
  uint   lo_ix;
  uint   hi_ix;
  ushort call_depth;
  ushort max_call_depth;
} qsort_info_t;

extern
bool check_sort(long *data, uint len);

extern
void print_array(long *data, uint len, int badelt);

extern
int compare(const void *left, const void * right);

extern
void swap_elem(void *left, void *right);

extern
void insertion_sort(long *data, uint lo_ix, uint hi_ix);

extern
void insertion_sort_opt(long *data, uint lo_ix, uint hi_ix);

extern
void quicksort(long *data, uint lo_ix, uint hi_ix);

extern
void quicksort_opt(long *data, uint lo_ix, uint hi_ix,
                   bool multithread);

extern
void heapsort(long *data, uint lo_ix, uint hi_ix);

extern
void merge_sort(long *data, uint lo_ix, uint hi_ix);

extern
void merge_sort_opt(long *data, long *tmpdata, uint lo_ix, uint hi_ix);

#endif /* SORTS_H */
