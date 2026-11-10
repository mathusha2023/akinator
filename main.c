#include "log.h"
#include "bin_tree.h"
#include "tree_io.h"

int main(void)
{
    restart_log();

    BinTree tree = {};

    TreeIOError error = parse_tree_from_file_by_filename(&tree, "data.txt");
    if (error != TREE_IO_OK)
    {
        LOG("ERROR WHILE PARSING: %s", tree_io_get_str_error(error));
        return error;
    }

    close_log();

    return 0;
}
