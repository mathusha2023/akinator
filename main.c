#include "log.h"
#include "bin_tree.h"

int main(void)
{
    restart_log();

    BinTree tree = {};
    BinTreeError error = TREE_OK;

    error = tree_insert_node(&tree, 10);
    if (error != TREE_OK)
    {
        LOG("ERROR: %s", tree_get_str_error(error));
        return error;
    }
    tree_print(&tree);

    error = tree_insert_node(&tree, 5);
    if (error != TREE_OK)
    {
        LOG("ERROR: %s", tree_get_str_error(error));
        return error;
    }
    tree_print(&tree);

    error = tree_insert_node(&tree, 7);
    if (error != TREE_OK)
    {
        LOG("ERROR: %s", tree_get_str_error(error));
        return error;
    }
    tree_print(&tree);

    error = tree_insert_node(&tree, 20);
    if (error != TREE_OK)
    {
        LOG("ERROR: %s", tree_get_str_error(error));
        return error;
    }
    tree_print(&tree);

    error = tree_insert_node(&tree, 1488);
    if (error != TREE_OK)
    {
        LOG("ERROR: %s", tree_get_str_error(error));
        return error;
    }
    tree_print(&tree);

    error = tree_insert_node(&tree, -1);
    if (error != TREE_OK)
    {
        LOG("ERROR: %s", tree_get_str_error(error));
        return error;
    }
    tree_print(&tree);

    tree_destroy(&tree);

    tree_print(&tree);

    close_log();

    return 0;
}
