//
// sortbench.c
//
// benchmarking various sorting methods (libc qsort, insertion, heap, quick, merge)
//
// Copyright (c) 2019, 2020, Martin Reames
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "sorts.h"

typedef enum {
  // SORT_MIN: first value in sort_t
  SORT_MIN        = 0,

  SORT_QSORT_LIBC = SORT_MIN,
  SORT_INSERT,
  SORT_INSERT_OPT,
  SORT_QSORT,
  SORT_QSORT_OPT,
  SORT_QSORT_MT,
  SORT_HEAP,
  SORT_MERGE,
  SORT_MERGE_OPT,

  SORT_COUNTING,

  // SORT_MAX: last value in sort_t
  SORT_MAX        = SORT_COUNTING
} sort_t;

// returns true if data is sorted at the end of this function
// returns false if we didn't sort
//
// assuming all the sorts are working correctly, we will only return
// false for slow sort methods
static bool
sort(long *data, long *tmpdata, uint nelts, sort_t sort_method,
     uint maxval /* only used by counting sort */)
{
  struct timeval tv_start, tv_end;
  double sort_time;

  gettimeofday(&tv_start, NULL);

  switch(sort_method)
  {
    case SORT_QSORT_LIBC:
    printf("sorting: sort method is libc qsort()\n");
    qsort(data, nelts, sizeof(long), &compare);
    break;

    case SORT_INSERT:
    printf("sorting: sort method is insertion sort\n");
    if (nelts > MAX_INSERT_SORT_NELTS) {
      printf("(skipping as this sort is O(n^2), i.e., painfully slow "
             "for so many elts)\n\n");
      return false;
    }
    insertion_sort(data, 0, nelts - 1);
    break;

    case SORT_INSERT_OPT:
    printf("sorting: sort method is insertion sort opt\n");
    if (nelts > MAX_INSERT_SORT_NELTS*4) {
      printf("(skipping as this sort is O(n^2), i.e., painfully slow "
             "for so many elts)\n\n");
      return false;
    }
    insertion_sort_opt(data, 0, nelts - 1);
    break;

    case SORT_QSORT:
    printf("sorting: sort method is quicksort\n");
    quicksort(data, 0, nelts - 1);
    break;

    case SORT_QSORT_OPT:
    printf("sorting: sort method is quicksort opt\n");
    quicksort_opt(data, 0, nelts - 1, false);
    break;

    case SORT_QSORT_MT:
    printf("sorting: sort method is quicksort opt mt\n");
    quicksort_opt(data, 0, nelts - 1, true);
    break;

    case SORT_HEAP:
    printf("sorting: sort method is heapsort\n");
    heapsort(data, 0, nelts - 1);
    break;

    case SORT_MERGE:
    printf("sorting: sort method is merge sort\n");
    merge_sort(data, 0, nelts - 1);
    break;

    case SORT_MERGE_OPT:
    printf("sorting: sort method is merge sort opt\n");
    merge_sort_opt(data, tmpdata, 0, nelts - 1);
    break;

    case SORT_COUNTING:
    printf("sorting: sort method is counting sort\n");
    counting_sort(data, 0, nelts - 1, maxval);
    break;

    default:
    assert(0);
  }

  gettimeofday(&tv_end, NULL);

  sort_time =
    (double)  (tv_end.tv_sec - tv_start.tv_sec) +
    ((double) (tv_end.tv_usec - tv_start.tv_usec)) / 1E6;
  assert(check_sort(data, nelts));

  printf("finished: sorted %u elements in %.2f seconds\n\n", nelts, sort_time);

  return true;
}

static
void usage()
{
  printf(
         "usage:\n\n"
         "sorts [-k val | -m val] [-c maxval]\n\n"
         "where val is [1..1000] and maxval is [1..100,000,000]\n"
         "(default is \"-m 100\", i.e., sort a random array of "
         "100 million long ints and omit counting sort)\n\n"
         );
  exit(-1);
}

// messy function to parse the command-line arguments
static
void parse_args(int argc, char * argv[], uint *nelts,
                bool *incl_count, uint *max_count_val)
{
  int i;
  long val;
  uint num_elts = 0;

  // should we do the counting sort or not?
  bool do_counting_sort = false;
  // if we're doing counting sort, what's the max value?
  uint maxval = 0;

  // iterate through the command-line arguments, using the
  // ones we can make sense of

  // use defaults if no command-line params
  if (argc == 1)
    return;

  i = 1;
  while (i < argc) {
    if (strcmp(argv[i], "-k") == 0) {
      i++;
      if (i == argc)
        usage();
      val = atoi(argv[i]);
      if (0 > val || val > 1000)
        usage();
      num_elts = val * K;
    }
    else if (strcmp(argv[i], "-m") == 0) {
      i++;
      if (i == argc)
        usage();
      val = atoi(argv[i]);
      if (0 > val || val > 1000)
        usage();
      num_elts = val * M;
    }
    else if (strcmp(argv[i], "-c") == 0) {
      i++;
      if (i == argc)
        usage();
      val = atoi(argv[i]);
      if (0 > val || val > MAX_COUNTINGSORT_VALUE)
        usage();
      maxval = val;
      do_counting_sort = true;
    }
    i++;
  }

  if (num_elts != 0)
    *nelts = num_elts;

  if (do_counting_sort) {
    *incl_count = true;
    *max_count_val = maxval;
  }
  else {
    *incl_count = false;
  }
}

int main(int argc, char * argv[])
{
  long  *data     = NULL;
  long  *origdata = NULL;
  long  *tmpdata  = NULL;
  long  *cmpdata  = NULL;
  uint   i;
  uint   seed;
  sort_t sort_idx;
  uint   nelts = DEFAULT_NELTS; // == 100 * M

  // counting sort stuff
  bool   do_counting_sort = false;
  uint   maxval;

  parse_args(argc, argv, &nelts,
             &do_counting_sort, &maxval);

  // allocate memory
  data = (long *) calloc(nelts, sizeof(long));
  if (data == NULL) {
    printf("error: cannot allocate memory for data\n");
    return -1;
  }

  origdata = (long *) calloc(nelts, sizeof(long));
  if (origdata == NULL) {
    printf("error: cannot allocate memory for origdata\n");
    return -1;
  }

  tmpdata = (long *) calloc(nelts, sizeof(long));
  if (tmpdata == NULL) {
    printf("error: cannot allocate memory for tmpdata\n");
    return -1;
  }

  cmpdata = (long *) calloc(nelts, sizeof(long));
  if (cmpdata == NULL) {
    printf("error: cannot allocate memory for cmpdata\n");
    return -1;
  }

  // populate data
  seed = ((uint) time(NULL)) % 16384;
  printf("main: seed is %u\n", seed);
  printf("main: sorting %d elements\n\n", nelts);

  srandom(seed);

  // if we're doing counting sort, we need to limit the range of data values
  // (to {0..maxval-1})
  if (do_counting_sort) {
    for (i = 0; i < nelts; ++i)
      origdata[i] = abs(random() % maxval);
  }
  else {
    for (i = 0; i < nelts; ++i)
      origdata[i] = random();
  }

  // sort using different sorting methods
  for (sort_idx = SORT_MIN; sort_idx <= SORT_MAX; sort_idx++)
  {
    if (sort_idx == SORT_COUNTING && !do_counting_sort)
      continue;

    // initialize data so that every sort method starts with same input
    memcpy(data, origdata, nelts * sizeof(long));

    // sort it!
    if (sort(data, tmpdata, nelts, sort_idx, maxval) == false)
      continue;

    // now compare the sort with the previous sort method
    if (sort_idx == SORT_MIN) {
      memcpy(cmpdata, data, nelts * sizeof(long));
    }
    else {
      if (!check_sort_cmp(cmpdata, data, nelts)) {
        printf("\n**** sorted data is different than previous sort!\n");
      }
    }

  }

  // free calloc'd memory (even though the process is about to terminate ...)
  free(data);
  free(origdata);
  free(tmpdata);
  return 0;
}
