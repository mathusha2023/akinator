#include "bin_tree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "log.h"
#include "config.h"
#include "tree_io.h"

static void node_destroy(Node **node);
static void insert_node_in_sorted_order(BinTree *tree, Node *new_node);

BinTreeError tree_insert_node(BinTree *tree, tree_el_t value)
{
    assert(tree);

    Node *new_node = (Node *)calloc(1, sizeof(Node));
    if (!new_node)
    {
        LOG("Error while allocating memory for tree [%p] node with value " TREE_EL_SPECIFICATOR,
            tree, value);
        return TREE_MEMORY_ERROR;
    }
    new_node->data = value;

    insert_node_in_sorted_order(tree, new_node);

    tree->n_nodes++;

    return TREE_OK;
}

void tree_destroy(BinTree *tree)
{
    assert(tree);

    if (tree->root)
        node_destroy(&tree->root);

    tree->n_nodes = 0;
}

void tree_print(BinTree *tree)
{
    assert(tree);

    TreeIOError error = write_tree_in_file(tree, stdout, INORDER);
    if (error == TREE_IO_EMPTY_TREE)
    {
        printf("Tree dump: tree is empty");
    }
    else if (error != TREE_IO_OK)
    {
        LOG("ERROR: cant print tree: %s", tree_io_get_str_error(error));
    }
    printf("\n");
}

const char *tree_get_str_error(BinTreeError error)
{
    return STR_BIN_TREE_ERRORS[error];
}

static void insert_node_in_sorted_order(BinTree *tree, Node *new_node)
{
    assert(tree);
    assert(new_node);

    Node **curr_node = &tree->root;

    while (*curr_node)
    {
        if ((*curr_node)->data > new_node->data)
        {
            curr_node = &(*curr_node)->left;
        }
        else
        {
            curr_node = &(*curr_node)->right;
        }
    }

    *curr_node = new_node;
}

static void node_destroy(Node **node)
{
    assert(node);
    assert(*node);

    if ((*node)->left)
        node_destroy(&(*node)->left);

    if ((*node)->right)
        node_destroy(&(*node)->right);

    free_ptr(*node);
}
