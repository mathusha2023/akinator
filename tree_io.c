#include "tree_io.h"
#include <stdio.h>
#include "bin_tree.h"
#include "log.h"

TreeIOError write_tree_in_file_by_filename(BinTree *tree, const char *filename, TreeOrders order)
{
    assert(tree);
    assert(filename);

    FILE *file = fopen(filename, "w");
    if (!file)
    {
        FLOG("ERROR: cant open file %s for writing tree:(", filename);
        return TREE_IO_FILE_ERROR;
    }

    TreeIOError error = write_tree_in_file(tree, file, order);
    fclose(file);

    return error;
}

TreeIOError write_tree_in_file(BinTree *tree, FILE *file, TreeOrders order)
{
    assert(tree);
    assert(file);

    if (!tree->root)
        return TREE_IO_EMPTY_TREE;

    TreeIOError error = write_node_in_file(tree->root, file, order);
    if (error != TREE_IO_OK)
    {
        FLOG("ERROR: cant write root in file: %s", tree_io_get_str_error(error));
        return error;
    }

    return TREE_IO_OK;
}

#define printf_data()                                                                            \
    {                                                                                            \
        if (fprintf(file, "\"" TREE_EL_SPECIFICATOR "\"", node->data) < 0)                       \
        {                                                                                        \
            FLOG("ERROR: cant write '" TREE_EL_SPECIFICATOR "' in file <%p>", node->data, file); \
            return TREE_IO_FILE_ERROR;                                                           \
        }                                                                                        \
    }

TreeIOError write_node_in_file(Node *node, FILE *file, TreeOrders order)
{
    assert(node);
    assert(file);

    if (fprintf(file, "(") < 0)
    {
        FLOG("ERROR: cant write '(' in file <%p>", file);
        return TREE_IO_FILE_ERROR;
    }

    if (order == PREORDER)
    {
        printf_data();
    }

    if (node->left)
    {
        TreeIOError error = write_node_in_file(node->left, file, order);
        if (error != TREE_IO_OK)
        {
            FLOG("ERROR: cant write node in file: %s", tree_io_get_str_error(error));
            return error;
        }
    }

    if (order == INORDER)
        printf_data();

    if (node->right)
    {
        TreeIOError error = write_node_in_file(node->right, file, order);
        if (error != TREE_IO_OK)
        {
            FLOG("ERROR: cant write node in file: %s", tree_io_get_str_error(error));
            return error;
        }
    }

    if (order == POSTORDER)
        printf_data();

    if (fprintf(file, ")") < 0)
    {
        FLOG("ERROR: cant write ')' in file <%p>", file);
        return TREE_IO_FILE_ERROR;
    }

    return TREE_IO_OK;
}

const char *tree_io_get_str_error(TreeIOError error)
{
    return STR_TREE_IO_ERRORS[error];
}
