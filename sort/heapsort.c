//
// heapsort.c
//
// basic heapsort (non recursive)
// 
// Copyright (c) 2019, 2020, Martin Reames
//

#include "sorts.h"

/* basic heapsort without recursion */
void heapsort(long *arr, uint lo_ix, uint hi_ix)
{
  long t; /* the temporary value */
  uint nelts = hi_ix - lo_ix + 1;
  int  n = nelts;
  uint parent = n / 2;
  uint index;
  uint child; 
  
  /* loop until array is sorted */
  while (1) 
  {
    if (parent > 0) { 
      /* first stage: sorting the heap */
      parent--;
      t = arr[parent];    /* save old value to t */
    } 
    else {
      /* second stage: extracting elements in-place */
      n--;                /* make the heap smaller */
      if (n == 0) {
        return;           /* when the heap is empty, we're done */
      }
      t = arr[n];         /* save lost heap entry to temporary */
      arr[n] = arr[0];    /* save root entry beyond heap */
    }

    /* insert operation: pushing t down the heap to replace the parent */
    index = parent; /* start at the parent index */
    child = index * 2 + 1; /* get its left child index */
    while (child < n) {
      /* choose the largest child */
      if ((child + 1 < n)  &&  
          /* (arr[child + 1] > arr[child]) */ 
          (compare(&(arr[child + 1]), &(arr[child])) > 0)) {
        child++; /* right child exists and is bigger */
      }
      
      /* is the largest child larger than the entry? */
      if (/* (arr[child] > t) */ 
          compare(&(arr[child]), &t) > 0) {
        arr[index] = arr[child]; /* overwrite entry with child */
        index = child; /* move index to the child */
        child = index * 2 + 1; /* get the left child and go around again */
      } 
      else {
        break; /* we found where t belongs */
      }
    }
    
    /* store the temporary value at its new location */
    arr[index] = t; 
  }
}
