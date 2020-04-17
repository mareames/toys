//
// tree.c
//
// some binary tree stuff
//
// Copyright (c) 2020, Martin Reames
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "common.h"
#include "queue.h"

const bool debug = true;

// create a new node_t and configure it with input value
node_t *
new_node(int value)
{
  node_t * n = malloc(sizeof(node_t));
  if (n == NULL)
    return NULL;

  n->value = value;
  n->refcnt = 1;
  n->left = NULL;
  n->right = NULL;

  return n;
}

// copy a node_t (just the contents, not the pointers)
//
// function is currently unused
node_t*
copy_node(node_t *node)
{
  node_t *n = NULL;

  // validate params
  if (node == NULL)
    return NULL;

  n = new_node(node->value);
  if (n != NULL)
    n->refcnt = node->refcnt;

  return n;
}

// binary search of the tree
//
// if the value is found, returns the node with the value
//
// if the value is not found, returns the node where to put that value
node_t *
tree_search(node_t *tree, int value)
{
  // sanity check of params
  if (!tree)
    return NULL;

  if (tree->value == value) {
    return tree;
  }
  else if (value < tree->value) {
    if (tree->left == NULL)
      return tree;
    else
      return tree_search(tree->left, value);
  }
  else {
    if (tree->right == NULL)
      return tree;
    else
      return tree_search(tree->right, value);
  }
}

// find a value in the tree if it exists
//
// function is currently unused
//
// found: *found = address of node containing value, if found
// found: *found = NULL if value does not exist in tree
int
find_value(node_t *tree, int value, node_t **found)
{
  node_t *n;

  // sanity check of params
  if (!tree || !found)
    return FAIL;

  n = tree_search(tree, value);
  if (n && n->value == value)
    *found = n;
  else
    *found = NULL;

  return SUCCESS;
}

// insert a value into the tree
int
insert_value(node_t **tree, int value)
{
  node_t *n;
  node_t *found;
  int ret = SUCCESS;

  if (debug)
    printf("inserting %d", value);

  if (!tree) {
    ret = FAIL;
    goto xit;
  }

  // base case: empty tree: just create a node and we're done
  if (*tree == NULL) {

    // create a node containing this value
    n = new_node(value);
    if (n == NULL) {
      ret = FAIL;
      goto xit;
    }

    *tree = n;

    ret = SUCCESS;
    goto xit;
  }

  // find the place in the tree to insert this value

  found = tree_search(*tree, value);
  if (found == NULL) {
    ret = FAIL;
    goto xit;
  }

  // if value is already in the tree, increment its refcnt
  if (value == found->value) {
    found->refcnt++;
  }
  else {
    // create a node containing this value
    n = new_node(value);
    if (n == NULL) {
      ret = FAIL;
      goto xit;
    }

    if (value < found->value) {
      assert(found->left == NULL);
      found->left = n;
      // printf("(parent: %d)", found->value);
    }
    else {
      assert(found->right == NULL);
      found->right = n;
      // printf("(parent: %d)", found->value);
    }
  }

 xit:
  if (debug)
    printf("... %s\n", ret == SUCCESS ? "success" : "failure");
  return ret;
}

// print a node's value
int
print_node(node_t *node)
{
  if (node == NULL)
    return FAIL;

  printf("%d", node->value);
  if (node->refcnt != 1)
    printf("(%d)", node->refcnt);
  printf(" ");

  return SUCCESS;
}

// print out the tree's values
int
print_tree_dfs_prefix(node_t *tree)
{
  // base condition of recursion
  if (!tree)
    return SUCCESS;

#undef recurse
#define recurse(X) print_tree_dfs_prefix(X)

  print_node(tree);
  recurse(tree->left);
  recurse(tree->right);

  return SUCCESS;
}

int
print_tree_dfs_postfix(node_t *tree)
{
  // base condition of recursion
  if (!tree)
    return SUCCESS;

#undef recurse
#define recurse(X) print_tree_dfs_postfix(X)

  recurse(tree->left);
  recurse(tree->right);
  print_node(tree);

  return SUCCESS;
}

int
print_tree_dfs_infix(node_t *tree)
{
  // base condition of recursion
  if (!tree)
    return SUCCESS;

#undef recurse
#define recurse(X) print_tree_dfs_infix(X)

  recurse(tree->left);
  print_node(tree);
  recurse(tree->right);

  return SUCCESS;
}

// recursive subroutine of print_tree_dfs_right_nodes_only_inorder
int print_tree_dfs_right_nodes_only_inorder_sub(node_t *tree, bool is_right)
{
  // base condition of recursion
  if (tree == NULL)
    return SUCCESS;

#undef recurse
#define recurse(X,Y) print_tree_dfs_right_nodes_only_inorder_sub(X,Y)

  if (tree->left)
    recurse(tree->left, false);

  if (is_right)
    print_node(tree);

  if (tree->right)
    recurse(tree->right, true);

  return SUCCESS;
}

// print only the right nodes of the tree using dfs
int
print_tree_dfs_right_nodes_only_inorder(node_t *tree)
{
  // the root node of the tree is not a "right node"
  return print_tree_dfs_right_nodes_only_inorder_sub(tree, false);
}

//
int
print_tree_bfs_right_nodes_only_sub(node_t *tree, queue_t *q)
{
  node_t * n;

  // sanity check of params
  if (tree == NULL || q == NULL)
    return FAIL;

  enQ(q, tree);
  n = deQ(q);
  while (n != NULL) {
    if (n->right != NULL) {
      print_node(n->right);
      enQ(q, n->right);
    }
    if (n->left != NULL) {
      enQ(q, n->left);
    }
    n = deQ(q);
  }

  return SUCCESS;
}

// print only the right nodes of the tree using bfs and a queue
int
print_tree_bfs_right_nodes_only(node_t *tree)
{
  queue_t q;

  if (tree == NULL)
    return FAIL;

  initQ(&q);

  return print_tree_bfs_right_nodes_only_sub(tree, &q);
}


// free the list
int
free_tree(node_t *tree)
{
  node_t *tmp;

  if (tree == NULL)
    return SUCCESS;

  free_tree(tree->left);
  free_tree(tree->right);
  free(tree);

  return SUCCESS;
}


// unit test to validate append function
int
test_insert()
{
  node_t *t = NULL;
  node_t *tree = NULL;

  int i, mod;

  // insert 1..10 to list in order
  for (i = 1; i <= 10; i++) {
    mod = (i*3 + 1) % 10;
    insert_value(&tree, mod);
  }
  // insert a couple of dup values
  insert_value(&tree, 7);
  insert_value(&tree, 3);

  // print the tree
  printf("\ntree dfs in order: ");
  print_tree_dfs_infix(tree);

  // print the tree preorder
  printf("\ntree dfs prefix order: ");
  print_tree_dfs_prefix(tree);

  // print the tree postfix
  printf("\ntree dfs postfix order: ");
  print_tree_dfs_postfix(tree);

  // print the tree right nodes in order
  printf("\ntree dfs in order right nodes only: ");
  print_tree_dfs_right_nodes_only_inorder(tree);

  // print the tree right nodes only via bfs
  printf("\ntree bfs right nodes only: ");
  print_tree_bfs_right_nodes_only(tree);

  // free the tree
  free_tree(tree);

  return SUCCESS;
}


int
main (int argc, char *argv[])
{
  printf("test insert: ");
  test_insert();
  printf("\n");

  return SUCCESS;
}
