#ifndef BIN_TREE_H
#define BIN_TREE_H

#include <stddef.h>

typedef int tree_el_t;
#define TREE_EL_SPECIFICATOR "%d"

typedef enum BinTreeError
{
    TREE_OK,
    TREE_MEMORY_ERROR,
} BinTreeError;

static const char *STR_BIN_TREE_ERRORS[] = {
    "TREE_OK",
    "TREE_MEMORY_ERROR",
};

typedef struct Node
{
    tree_el_t data;
    Node *left;
    Node *right;
} Node;

typedef struct BinTree
{
    Node *root;
    size_t n_nodes;
} BinTree;

BinTreeError tree_insert_node(BinTree *tree, tree_el_t value);
void tree_destroy(BinTree *tree);
void node_destroy(Node **node);

void tree_print(BinTree *tree);
void tree_dump(BinTree *tree);
const char *tree_get_str_error(BinTreeError error);

#endif // BIN_TREE_H