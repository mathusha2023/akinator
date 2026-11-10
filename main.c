#include "log.h"
#include "bin_tree.h"
#include "tree_io.h"

int main(void)
{
    restart_log();

    BinTree tree = {};
    BinTreeError error = TREE_OK;

    error = tree_insert_node(&tree, tree.root, ROOT_CHILD, "Никто");
    if (error != TREE_OK)
    {
        LOG("ERRORR: %s", tree_get_str_error(error));
        return error;
    }

    error = tree_insert_node(&tree, tree.root, LEFT_CHILD, "Мама");
    if (error != TREE_OK)
    {
        LOG("ERRORR: %s", tree_get_str_error(error));
        return error;
    }

    error = tree_insert_node(&tree, tree.root, RIGHT_CHILD, "Папа");
    if (error != TREE_OK)
    {
        LOG("ERRORR: %s", tree_get_str_error(error));
        return error;
    }

    tree_print(&tree);

    tree_destroy(&tree);
    close_log();

    return 0;
}
