#include <stdlib.h>

typedef enum {
  COLOR_RED,
  COLOR_BLACK,
} node_color_t;

typedef struct node {
  node_color_t color;
  struct node *left;
  struct node *right;
  struct node *parent;
  int key; // TODO: change to void * and add a compare function and a value
           // field
} node_t;

node_t nil_t = {
    .color = COLOR_BLACK,
    .left = NULL,
    .right = NULL,
    .key = 0,
};

node_t *nil = &nil_t;

node_t *new_node(int key, node_color_t color) {
  node_t *n = malloc(sizeof(node_t));

  n->right = nil;
  n->left = nil;
  n->key = key;
  n->color = color;
  n->parent = nil;

  return n;
}

void left_rotate(node_t *x) {
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != nil) {
    y->left->parent = x;
  }

  y->parent = x->parent;

  if (x->parent != nil) {
    if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
  }

  y->left = x;
  x->parent = y;
}

void right_rotate(node_t *x) {
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != nil) {
    y->right->parent = x;
  }

  y->parent = x->parent;

  if (x->parent != nil) {
    if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
  }

  y->right = x;
  x->parent = y;
}

void tree_fixup(node_t *root, node_t *z) {
  while (z->parent->color == COLOR_RED) {
    if (z->parent == z->parent->parent->left) {
      node_t *y = z->parent->parent->right; // Uncle of z
      if (y->color == COLOR_RED) {
        z->parent->color = COLOR_BLACK;
        y->color = COLOR_BLACK;
        z->parent->parent->color = COLOR_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          z = z->parent;
          left_rotate(z);
        }
        z->parent->color = COLOR_BLACK;
        z->parent->parent->color = COLOR_RED;
        right_rotate(z->parent->parent);
      }
    } else {
      node_t *y = z->parent->parent->left; // Uncle of z
      if (y->color == COLOR_RED) {
        z->parent->color = COLOR_BLACK;
        y->color = COLOR_BLACK;
        z->parent->parent->color = COLOR_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(z);
        }
        z->parent->color = COLOR_BLACK;
        z->parent->parent->color = COLOR_RED;
        left_rotate(z->parent->parent);
      }
    }
  }

  root->color = COLOR_BLACK;
}

void tree_insert(node_t **root, node_t *z) {
  node_t *y = nil;
  node_t *x = *root;

  while (x != nil) {
    y = x;
    if (z->key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  z->parent = y;

  if (y == nil) {
    *root = z;
    return;
  }

  if (z->key < x->key) {
    x->left = z;
  } else {
    x->right = z;
  }

  z->left = nil;
  z->right = nil;
  z->color = COLOR_RED;

  tree_fixup(*root, z);
}

void transplant(node_t **root, node_t *u, node_t *v) {
  if (!u->parent) {
    *root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }

  v->parent = u->parent;
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

void delete_fixup(node_t **root, node_t *x) {
  while (x != *root && x->color == COLOR_BLACK) {
    if (x == x->parent->left) {
      node_t *w = x->parent->right;
      if (w->color == COLOR_RED) {
        w->color = COLOR_BLACK;
        x->parent->color = COLOR_RED;
        left_rotate(x->parent);

        w = x->parent->right;
      }

      if (w->left->color == COLOR_BLACK && w->right->color == COLOR_BLACK) {
        w->color = COLOR_RED;
        x = x->parent;
      } else if (w->right->color == COLOR_BLACK) {
        w->left->color = COLOR_BLACK;
        w->color = COLOR_RED;
        right_rotate(w);
        w = x->parent->right;
        w->color = x->parent->color;
        x->parent->color = COLOR_BLACK;
      }

      w->right->color = COLOR_BLACK;
      left_rotate(x->parent);
      x = *root;
    } else {
      node_t *w = x->parent->left;
      if (w->color == COLOR_RED) {
        w->color = COLOR_BLACK;
        x->parent->color = COLOR_RED;
        right_rotate(x->parent);

        w = x->parent->left;
      }

      if (w->left->color == COLOR_BLACK && w->right->color == COLOR_BLACK) {
        w->color = COLOR_RED;
        x = x->parent;
      } else if (w->left->color == COLOR_BLACK) {
        w->right->color = COLOR_BLACK;
        w->color = COLOR_RED;
        left_rotate(w);
        w = x->parent->left;
        w->color = x->parent->color;
        x->parent->color = COLOR_BLACK;
      }

      w->left->color = COLOR_BLACK;
      right_rotate(x->parent);
      x = *root;
    }
  }
  x->color = COLOR_BLACK;
}

void delete(node_t **root, node_t *z) {
  node_t *y = z;
  node_t *x;
  node_color_t y_original_color = y->color;

  if (z->left == nil) {
    x = z->right;
    transplant(root, z, z->right);
  } else if (z->right == nil) {
    x = z->left;
    transplant(root, z, z->left);
  } else {
    y = tree_minimum(z->right);
    y_original_color = y->color;
    x = y->right;
    if (y->parent == z) {
      x->parent = y;
    } else {
      transplant(root, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }

    transplant(root, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == COLOR_BLACK) {
    delete_fixup(root, x);
  }
}
