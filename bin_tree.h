#ifndef BIN_TREE_H
#define BIN_TREE_H

#include <stddef.h>

typedef char *tree_el_t;
#define TREE_EL_SPECIFICATOR "%s"

typedef enum BinTreeError
{
    TREE_OK,
    TREE_MEMORY_ERROR,
    TREE_INCORRECT_CHILD_TYPE,
    TREE_INCORRECT_INSERT_ARGUMENTS,
} BinTreeError;

static const char *STR_BIN_TREE_ERRORS[] = {
    "TREE_OK",
    "TREE_MEMORY_ERROR",
    "TREE_INCORRECT_CHILD_TYPE",
    "TREE_INCORRECT_INSERT_ARGUMENTS",
};

typedef enum ChildType
{
    ROOT_CHILD,
    LEFT_CHILD,
    RIGHT_CHILD,
} ChildType;

static const char *STR_BIN_TREE_CHILD_TYPES[] = {
    "ROOT_CHILD",
    "LEFT_CHILD",
    "RIGHT_CHILD",
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

BinTreeError tree_insert_node(BinTree *tree, Node *node, ChildType child_type, tree_el_t value);
void tree_destroy(BinTree *tree);
void node_destroy(Node **node);

void tree_print(BinTree *tree);
void tree_dump(BinTree *tree);
const char *tree_get_str_error(BinTreeError error);
const char *tree_get_str_child_type(ChildType child_type);

#endif // BIN_TREE_H