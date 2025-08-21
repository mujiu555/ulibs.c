/**
* @file: inc/tree/rbt.h
* @description: red-black tree
*/

#ifndef ULIB__TREE_RBT_H__
#define ULIB__TREE_RBT_H__

struct rbt_node_t;

struct rbt {
  struct rbt_node_t *root;
};

#endif // !ULIB__TREE_RBT_H__
