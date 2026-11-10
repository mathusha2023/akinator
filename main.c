#include "log.h"
#include "bin_tree.h"
#include "tree_io.h"

int main(void)
{
    restart_log();

    BinTree tree = {};
    BinTreeError error = TREE_OK;

    tree_el_t x = 0;
    size_t n = 0;

    printf("Enter your numbers count: ");
    int res = scanf("%zu", &n);
    if (res <= 0)
    {
        LOG("ERROR WHILE READING n");
        return 1;
    }

    printf("Enter %zu numbers to sort: ", n);
    for (size_t i = 0; i < n; i++)
    {
        res = scanf(TREE_EL_SPECIFICATOR, &x);
        if (res <= 0)
        {
            LOG("ERROR WHILE READING number");
            return 1;
        }
        error = tree_insert_node(&tree, x);
        if (error != TREE_OK)
        {
            LOG("ERROR WHILE INSERTING VALUE TO TREE: %s", tree_get_str_error(error));
            return error;
        }
    }

    printf("\n");
    tree_print(&tree);

    tree_destroy(&tree);
    close_log();

    return 0;
}
