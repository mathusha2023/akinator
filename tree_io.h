#ifndef TREE_IO_H
#define TREE_IO_H

#include "bin_tree.h"
#include <stdio.h>

typedef enum TreeOrders
{
    PREORDER,
    INORDER,
    POSTORDER,
} TreeOrders;

typedef enum TreeIOError
{
    TREE_IO_OK,
    TREE_IO_FILE_ERROR,
    TREE_IO_EMPTY_TREE,
} TreeIOError;

static const char *STR_TREE_IO_ERRORS[] = {
    "TREE_IO_OK",
    "TREE_IO_FILE_ERROR",
    "TREE_IO_EMPTY_TREE",
};

TreeIOError write_tree_in_file_by_filename(BinTree *tree, const char *filename, TreeOrders order);
TreeIOError write_tree_in_file(BinTree *tree, FILE *file, TreeOrders order);
TreeIOError write_node_in_file(Node *node, FILE *file, TreeOrders order);

const char *tree_io_get_str_error(TreeIOError error);

#endif // TREE_IO_H