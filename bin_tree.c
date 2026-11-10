#include "bin_tree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "log.h"
#include "config.h"
#include "tree_io.h"
#include "graphviz.h"

static BinTreeError insert_node_to_pointer(Node **node_ptr, tree_el_t value);

// если дерево пусто, то tree->root = NULL; node = NULL; child_type = ROOT_CHILD
// в таком случае вставка элемента в корень пройдет успешно
// если же хоть одно из этих условий не выполняется при наличии остальных, то это ошибка
// короче говоря, для корректной вставки или должны быть выполнены все приведенные условия
// или ни одно из них
BinTreeError tree_insert_node(BinTree *tree, Node *node, ChildType child_type, tree_el_t value)
{
    assert(tree);

    int cond1 = !tree->root, cond2 = !node, cond3 = child_type == ROOT_CHILD;

    BinTreeError error = TREE_OK;

    if (cond1 && cond2 && cond3)
    {
        FLOG("<h1>Inserting node with value \"" TREE_EL_SPECIFICATOR "\" to the tree [%p] root</h1>",
             value, tree);

        error = insert_node_to_pointer(&tree->root, value);
    }

    else if (!(cond1 || cond2 || cond3))
    {
        FLOG("<h1>Inserting node with value \"" TREE_EL_SPECIFICATOR
             "\" to the tree [%p] as node with value \"" TREE_EL_SPECIFICATOR "\" %s</h1>",
             value, tree, node->data, tree_get_str_child_type(child_type));

        switch (child_type)
        {
        case LEFT_CHILD:
            error = insert_node_to_pointer(&node->left, value);
            break;
        case RIGHT_CHILD:
            error = insert_node_to_pointer(&node->right, value);
            break;
        case ROOT_CHILD:
        default:
            FLOG("ERROR: incorrect child type!!!");
            return TREE_INCORRECT_CHILD_TYPE;
        }
    }

    else
    {
        FLOG("Error: incorrect arguments in function tree_insert_node() call. "
             "Please, write comments in code and NE EBITE MY BRAINNNN");
        error = TREE_INCORRECT_INSERT_ARGUMENTS;
    }

    if (error != TREE_OK)
    {
        FLOG("Error while inserting node: %s", tree_get_str_error(error));
        return error;
    }

    tree->n_nodes++;

    if (!(cond1 || cond2 || cond3))
        FLOG("Successful insert node with value \"" TREE_EL_SPECIFICATOR "\" to the tree [%p] root",
             value, tree);

    else if (!(cond1 || cond2 || cond3))
        FLOG("<h1>Successful insert node with value \"" TREE_EL_SPECIFICATOR
             "\" to the tree [%p] as node with value \"" TREE_EL_SPECIFICATOR "\" %s</h1>",
             value, tree, node->data, tree_get_str_child_type(child_type));

    tree_dump(tree);

    return error;
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
    if (error != TREE_IO_OK)
    {
        LOG("ERROR: cant print tree: %s", tree_io_get_str_error(error));
    }
    printf("\n");
}

void node_destroy(Node **node)
{
    assert(node);
    assert(*node);

    if ((*node)->left)
        node_destroy(&(*node)->left);

    if ((*node)->right)
        node_destroy(&(*node)->right);

    free_ptr(*node);
}

void tree_dump(BinTree *tree)
{
    assert(tree);

    FLOG("<h2>TREE [%p] DUMP:</h2>", tree);
    FLOG("tree->n_nodes = %zu", tree->n_nodes);
    FLOG("tree->root = %p", tree->root);

    // с учетом 2 по \0 в константах можно не делать +2, но так надежнее)
    char graph_dir_path[sizeof(LOG_DIRECTORY) + sizeof(GRAPH_DIRECTORY) + 2] = {};
    if (snprintf(graph_dir_path, sizeof(graph_dir_path), "%s/%s", LOG_DIRECTORY, GRAPH_DIRECTORY) < 0)
    {
        FLOG("Error: can not write graph_dir_path to buffer");
        return;
    }

    create_folder_if_not_exists(graph_dir_path);

    char filename[MAX_FILENAME_LENGTH] = {};

    struct timespec ts = {};
    timespec_get(&ts, TIME_UTC);
    struct tm *local_time = localtime(&ts.tv_sec);

    if (snprintf(filename, MAX_FILENAME_LENGTH, "%s/%s/%04d.%02d.%02d_%02d:%02d:%02d:%03ld.png", LOG_DIRECTORY, GRAPH_DIRECTORY,
                 local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
                 local_time->tm_hour, local_time->tm_min, local_time->tm_sec, ts.tv_nsec) < 0)
    {
        FLOG("Error: can not write graph filename to buf");
        return;
    }

    // в претти моде (чтобы были видны ДА / НЕТ)
    if (write_graph(tree, filename, 1))
    {
        FLOG("Error: cant write write graph to file %s", filename);
        return;
    }

    // открываем сам файл чтобы убрать собщения с местом и временем вызова на картинке
    FILE *logfile = fopen(LOGFILE_NAME, "a");
    if (!logfile)
        return;

    fprintf(logfile, "\n\n");
    fprintf(logfile, "<img src = \"%s\" alt = \"Здесь должен быть граф связного списка((\">", filename + strlen(LOG_DIRECTORY) + 1);
    fprintf(logfile, "\n\n\n\n");

    fclose(logfile);
}

const char *tree_get_str_error(BinTreeError error)
{
    return STR_BIN_TREE_ERRORS[error];
}

const char *tree_get_str_child_type(ChildType child_type)
{
    return STR_BIN_TREE_CHILD_TYPES[child_type];
}

static BinTreeError insert_node_to_pointer(Node **node_ptr, tree_el_t value)
{
    assert(node_ptr);
    assert(!*node_ptr);

    Node *new_node = (Node *)calloc(1, sizeof(Node));
    if (!new_node)
    {
        FLOG("Error while allocating memory for node with value " TREE_EL_SPECIFICATOR, value);
        return TREE_MEMORY_ERROR;
    }
    new_node->data = value;

    *node_ptr = new_node;
    return TREE_OK;
}
