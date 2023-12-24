#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  struct node *parent;
  struct node *left;
  struct node *right;
  int value;
} node_t;

typedef struct {
  node_t *root;
} tree_t;

node_t *new_node(int value) {
  node_t *n = malloc(sizeof(node_t));
  memset(n, 0, sizeof(*n));
  n->value = value;

  return n;
}

void tree_insert(tree_t *t, node_t *n) {
  node_t *y = NULL;
  node_t *x = t->root;

  while (x) {
    y = x;
    if (n->value < x->value) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  n->parent = y;
  if (!y) {
    t->root = n;
  } else if (n->value < y->value) {
    y->left = n;
  } else {
    y->right = n;
  }
}

node_t *tree_search_recur(node_t *n, int k) {
  if (!n) {
    return NULL;
  }

  if (n->value == k) {
    return n;
  }

  if (n->value < k) {
    return tree_search_recur(n->left, k);
  }

  return tree_search_recur(n->right, k);
}

inline node_t *tree_search(tree_t *t, int k) {
  return tree_search_recur(t->root, k);
}

void inorder_traverse_recur(node_t *n) {
  if (n->left) {
    inorder_traverse_recur(n->left);
  }

  printf("%d ", n->value);

  if (n->right) {
    inorder_traverse_recur(n->right);
  }
}

static inline void inorder_traverse(tree_t *t) {
  return inorder_traverse_recur(t->root);
}

node_t *tree_minimum(node_t *x) {
  while (x->left) {
    x = x->left;
  }

  return x;
}

node_t *tree_successor(node_t *x) {
  if (x->right) {
    return tree_minimum(x->right);
  }

  node_t *y = x->parent;
  while (y && x == y->right) {
    x = y;
    y = y->parent;
  }

  return y;
}

void transplant(tree_t *t, node_t *u, node_t *v) {
  if (!u->parent) {
    t->root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }

  if (v) {
    v->parent = u->parent;
  }
}

void tree_delete(tree_t *t, node_t *z) {
  if (!z->left) {
    return transplant(t, z, z->right);
  }
  if (!z->right) {
    return transplant(t, z, z->left);
  }

  node_t *y = tree_minimum(z->right);
  if (y->parent != z) {
    transplant(t, y, y->right);
    y->right = z->right;
    y->right->parent = y;
  }
  transplant(t, z, y);
  y->left = z->left;
  y->left->parent = y;
}

tree_t *from_array(int *values, size_t len) {
  tree_t *t = malloc(sizeof(tree_t));
  t->root = NULL;
  for (size_t i = 0; i < len; i++) {
    tree_insert(t, new_node(values[i]));
  }

  return t;
}

#define LEN(xs) sizeof(xs) / sizeof(xs[0])

int main() {
  int values[] = {2,  4,   1,  567, 23, 67, 3,  5, 3,  56, 234,
                  67, 345, 67, 2,   7,  9,  34, 4, 78, 0};

  tree_t *tree = from_array(values, LEN(values));

  inorder_traverse(tree);
  printf("\n");

  tree_delete(tree, tree->root->right->left);
  inorder_traverse(tree);
  printf("\n");
}
