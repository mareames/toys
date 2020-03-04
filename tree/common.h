//
// common.h
//
// common definitions for binary tree stuff
//
// Copyright (c) 2020, Martin Reames
//

#ifndef COMMON_H
#define COMMON_H

#define SUCCESS 0
#define FAIL    1

typedef struct _node_ {
  int            value;
  int            refcnt;
  struct _node_ *left;
  struct _node_ *right;
} node_t;

#endif /* COMMON_H */
