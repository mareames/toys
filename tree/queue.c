//
// queue.c
//
// simple array-based queue implementation
//
// Copyright (c) 2020, Martin Reames
//

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "common.h"
#include "queue.h"

// create and initialize a queue element
q_element_t * new_queue_element(node_t *node)
{
  q_element_t * e;
  e = malloc(sizeof(q_element_t));
  if (e != NULL) {
    e->packet = node;
    e->next = NULL;
  }
  return e;
}

// initialize a queue
int initQ(queue_t *q)
{
  q->newest = q->oldest = q->prev_oldest = NULL;
  return SUCCESS;
}

// adds this node to the queue
// resizes as necessary
int enQ(queue_t *q, node_t *n)
{
  q_element_t *e = new_queue_element(n);

  if (e == NULL)
    return FAIL;

  // base case: queue has 0 elts currently
  if (q->oldest == NULL) {
    assert(q->newest == NULL);
    q->newest = q->oldest = e;
  }

  // otherwise set up the new queue element as the
  // newest value
  else {
    e->next = q->newest;
    q->newest = e;
  }

  return SUCCESS;
}

// removes a node from the queue
node_t * deQ(queue_t *q)
{
  node_t * n = NULL;
  q_element_t *iter;

  // sanity check: empty queue
  if (q->oldest == NULL) {
    assert(q->newest == NULL);
    return NULL;
  }

  // base case: one element in queue
  if (q->oldest == q->newest) {
    n = q->oldest->packet;
    free(q->oldest);
    q->oldest = q->newest = NULL;
  }
  else {

    // otherwise, keep track of the oldest node
    n = q->oldest->packet;

    // and then find the next oldest value
    iter = q->newest;
    while (iter->next != q->oldest) {
      iter = iter->next;
    }

    // free memory from the oldest element
    free(q->oldest);

    // set up the oldest element to be iter
    q->oldest = iter;
    iter->next = NULL; // not actually necessary
  }

  return n;
}
