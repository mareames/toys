//
// llist.c
//
// simple linked-list implementation (not sorted)
//
// Copyright (c) 2019, 2020, Martin Reames
//

#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAIL    1

typedef struct _node_ {
  int            payload;
  struct _node_ *next;
} node_t;

// create a new node_t and configure it with its payload
int
new_node(node_t **node, int payload)
{
  node_t * n = NULL;

  // validate params
  if (!node)
    return FAIL;

  n = malloc(sizeof(node_t));
  if (n == NULL)
    return FAIL;

  n->payload = payload;
  n->next = NULL;
  *node = n;

  return SUCCESS;
}

// copy a node_t (just the payload, not the pointers)
int
copy_node(node_t *node, node_t **newnode)
{
  node_t *n;

  // validate params
  if (!node || !newnode)
    return FAIL;

  new_node(&n, node->payload);
  *newnode = n;

  return SUCCESS;
}

// prepend a node_t to the list (add it to the front)
int
prepend_node(node_t **list, node_t *node)
{
  // validate params
  if (!node || !list)
    return FAIL;

  // if list is empty, list becomes node_t
  if (*list == NULL) {
    *list = node;
    // the input node_t should have no children; enforce this
    node->next = NULL;

  }
  else {
    node->next = *list;
    *list = node;
  }

  return SUCCESS;
}

// append a node_t to the list (add it to the end)
int
append_node(node_t **list, node_t *node)
{
  node_t *iter;

  // validate params
  if (!node || !list)
    return FAIL;

  // if list is empty, list becomes node
  if (*list == NULL) {
    *list = node;
    // the 1input node_t should have no children; enforce this
    // rather than check
    node->next = NULL;

  }
  else {
    iter = *list;
    // find the end of the list
    while (iter->next != NULL)
      iter = iter->next;

    // iter is now the last element in the list
    iter->next = node;
    // the input node_t should have no children; enforce this
    node->next = NULL;
    node->next = NULL;
  }

  return SUCCESS;
}

// print out the payload of the list
int
print_list(node_t *list)
{
  node_t *iter;

  // validate params
  if (!list) {
    printf("empty list");
  }
  else {
    iter = list;
    while (iter != NULL) {
      printf("%d ", iter->payload);
      iter = iter->next;
    }
  }

  printf("\n");

  return SUCCESS;
}

// free the list
int
free_list(node_t *list)
{
  node_t *tmp;

  // note: if list if NULL, this loop does nothing
  while (list != NULL) {
    tmp = list;
    list = list->next;
    free(tmp);
  }

  return SUCCESS;
}

// copy the list, using append_node_t in O(n^2) time
int
copy_list1(node_t *list, node_t **copy)
{
  node_t *n = NULL;
  node_t *newlist = NULL;

  // validate params
  if (!list) {
    // return an empty list unless bad param
    if (!copy)
      return FAIL;
    else
      *copy = NULL;
    return SUCCESS;
  }
  if (!copy)
    return FAIL;

  // iterate over the list, copying each node_t and appending
  // it to the new list via append_node_t

  while (list != NULL) {
    copy_node(list, &n);
    append_node(&newlist, n);
    list = list->next;
  }

  *copy = newlist;

  return SUCCESS;
}

// copy the list in O(n) time
int
copy_list2(node_t *list, node_t **copy)
{
  node_t *n = NULL;
  node_t *iter = NULL;

  // validate params
  if (!list) {
    // return an empty list unless bad param
    if (!copy)
      return FAIL;
    else
      *copy = NULL;
    return SUCCESS;
  }
  if (!copy)
    return FAIL;

  // iterate over the list, copying each node_t and then
  // appending it to the new list by keeping track of the
  // end of the new list

  while (list != NULL) {
    copy_node(list, &n);
    list = list->next;

    // bootstrap the start of the list
    if (iter == NULL) {
      *copy = n; // keep track of the start of the list
      iter = n;
    }
    else {
      iter->next = n;
      iter = iter->next;
    }
  }

  return SUCCESS;
}

// reverse a list using prepend_node_t function in O(n) time
int
reverse_list1(node_t *list, node_t **reverse)
{
  node_t *n = NULL;

  // validate params
  if (!list) {
    // return an empty list unless bad param
    if (!reverse)
      return FAIL;
    else
      *reverse = NULL;
    return SUCCESS;
  }
  if (!reverse)
    return FAIL;

  // iterate over the list, copying each node_t and then
  // prepending that node_t via prepend_list

  while (list != NULL) {
    copy_node(list, &n);
    list = list->next;
    prepend_node(reverse, n);
  }

  return SUCCESS;
}

// reverse a list without using prepend_node_t function in O(n) time
int
reverse_list2(node_t *list, node_t **reverse)
{
  node_t *n = NULL;
  node_t *iter = NULL;

  // validate params
  if (!list) {
    // return an empty list unless bad param
    if (!reverse)
      return FAIL;
    else
      *reverse = NULL;
    return SUCCESS;
  }
  if (!reverse)
    return FAIL;

  // iterate over the list, copying each node_t and then
  // prepending that node_t via prepend_list

  while (list != NULL) {
    copy_node(list, &n);
    list = list->next;

    if (iter == NULL) {
      iter = n;
    }
    else {
      n->next = iter;
      iter = n;
    }
  }

  *reverse = iter;

  return SUCCESS;
}

// unit test to validate append function
int
test_append()
{
  node_t *t = NULL;
  node_t *list = NULL;

  int i;

  // insert 1..10 to list in order
  for (i = 1; i <= 10; i++) {
    new_node(&t, i);
    append_node(&list, t);
  }

  // print the list
  print_list(list);

  // free the list
  free_list(list);

  return SUCCESS;
}

// unit test to validate prepend function
int
test_prepend()
{
  node_t *t = NULL;
  node_t *list = NULL;

  int i;

  // insert 1..10 to list in reverse order
  for (i = 1; i <= 10; i++) {
    new_node(&t, i);
    prepend_node(&list, t);
  }

  // print the list
  print_list(list);

  // free the list
  free_list(list);

  return SUCCESS;
}

typedef enum {
  listop_copylist_1 =    1,
  listop_copylist_2 =    2,
  listop_reverselist_1 = 3,
  listop_reverselist_2 = 4
} listop_t;

// unit test to validate copy_list1 function
int
test_list_op(listop_t listop)
{
  node_t *t = NULL;
  node_t *list = NULL;
  node_t *newlist = NULL;

  int i;

  // insert 1..10 to list in reverse order
  for (i = 1; i <= 10; i++) {
    new_node(&t, i);
    prepend_node(&list, t);
  }

  // print the original list
  printf("original list: ");
  print_list(list);

  // perform the list operation
  switch (listop) {

  case listop_copylist_1:
    copy_list1(list, &newlist);
    break;

  case listop_copylist_2:
    copy_list1(list, &newlist);
    break;

  case listop_reverselist_1:
    reverse_list1(list, &newlist);
    break;

  case listop_reverselist_2:
    reverse_list2(list, &newlist);
    break;

  default:
    return FAIL;
  }

  // print the copy list
  printf("new list: ");
  print_list(newlist);

  // free the first list
  free_list(list);

  // free the new list
  free_list(newlist);

  return SUCCESS;
}

int
main (int argc, char *argv[])
{
  printf("test append: ");
  test_append();
  printf("\n");

  printf("test prepend: ");
  test_prepend();
  printf("\n");

  printf("test copy_list1\n");
  test_list_op(listop_copylist_1);
  printf("\n");

  printf("test copy_list2\n");
  test_list_op(listop_copylist_2);
  printf("\n");

  printf("test reverse_list1\n");
  test_list_op(listop_reverselist_1);
  printf("\n");

  printf("test reverse_list2\n");
  test_list_op(listop_reverselist_2);
  printf("\n");

  return SUCCESS;
}
