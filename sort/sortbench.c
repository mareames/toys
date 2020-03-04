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
  /* SORT_MIN: first value in sort_t */
  SORT_MIN        = 0,

  SORT_QSORT_LIBC = SORT_MIN,
  SORT_INSERT     = 1,
  SORT_QSORT      = 2,
  SORT_QSORT_OPT  = 3,
  SORT_QSORT_MT   = 4,
  SORT_HEAP       = 5,
  SORT_MERGE      = 6,
  SORT_MERGE_OPT  = 7,

  /* SORT_MAX: last value in sort_t */
  SORT_MAX        = SORT_MERGE_OPT
} sort_t;

static
void sort(long *data, long *tmpdata, uint nelts, sort_t sort_method)
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
      return;
    }
    insertion_sort(data, 0, nelts - 1);
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

    default:
    assert(0);
  }

  gettimeofday(&tv_end, NULL);

  sort_time =
    (double)  (tv_end.tv_sec - tv_start.tv_sec) +
    ((double) (tv_end.tv_usec - tv_start.tv_usec)) / 1E6;
  assert(check_sort(data, nelts));

  printf("finished: sorted %u elements in %.2f seconds\n\n", nelts, sort_time);
}

static
void usage()
{
  printf(
         "usage:\n\n"
         "sorts [-k val | -m val]\n\n"
         "where val is [1..1000]\n"
         "(default is \"-m 100\", i.e., sort a random array of "
         "100 million long ints)\n\n"
         );
  exit(-1);
}

// messy function to parse the command-line arguments
static
void parse_args(int argc, char * argv[], uint *nelts)
{
  int i;
  long val;
  uint num_elts = 0;
  // iterate through the command-line arguments, using the
  // ones we can make sense of

  // use defaults if no command-line params
  if (argc == 1)
    return;

  i = 1;
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

  if (num_elts != 0)
    *nelts = num_elts;
}

int main(int argc, char * argv[])
{
  long  *data;
  long  *origdata;
  long  *tmpdata;
  uint   i;
  uint   seed;
  sort_t sort_idx;
  uint   nelts = DEFAULT_NELTS; // == 100 * M

  parse_args(argc, argv, &nelts);

  /* allocate memory */
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

  /* populate data */
  seed = ((uint) time(NULL)) % 16384;
  printf("main: seed is %u\n", seed);
  printf("main: sorting %d elements\n\n", nelts);

  srandom(seed);
  for(i = 0; i < nelts; ++i)
    origdata[i] = random();

  /* sort using different sorting methods */
  for (sort_idx = SORT_MIN; sort_idx <= SORT_MAX; sort_idx++)
  {
    /* initialize data so that every sort method starts with same input */
    memcpy(data, origdata, nelts * sizeof(long));

    /* sort it! */
    sort(data, tmpdata, nelts, sort_idx);
  }

  // free calloc'd memory (even though the process is about to terminate ...)
  free(data);
  free(origdata);
  return 0;
}
