#include "log.h"
#include "bin_tree.h"
#include "tree_io.h"
#include "akinator.h"

int main(void)
{
    restart_log();

    BinTree tree = {};

    TreeIOError error = parse_tree_from_file_by_filename(&tree, DATA_FILE_NAME);
    if (error != TREE_IO_OK)
    {
        LOG("ERROR WHILE PARSING TREE: %s", tree_io_get_str_error(error));
        return error;
    }

    main_loop(&tree);

    tree_destroy(&tree);

    close_log();

    return 0;
}
