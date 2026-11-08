#include "log.h"
#include "bin_tree.h"
#include "tree_io.h"

int main(void)
{
    restart_log();

    BinTree tree = {};
    BinTreeError error = TREE_OK;

    tree_destroy(&tree);
    close_log();

    return 0;
}
