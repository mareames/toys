//
// queue.h
//
// header file for simple linked-list queue implementation
//
// Copyright (c) 2020, Martin Reames
//

#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"

typedef struct _q_elt_ {
  node_t          *packet;
  struct _q_elt_  *next;
} q_element_t;

typedef struct {
  q_element_t     *newest; // newest queue element (last to go)
  q_element_t     *oldest; // oldest queue element (next to go)
  q_element_t     *prev_oldest; // optimization: not used yet
} queue_t;

extern int initQ(queue_t *q);

extern int enQ(queue_t *q, node_t *n);

extern node_t* deQ(queue_t *q);

#endif /* QUEUE_H */
