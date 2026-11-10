#include "bin_tree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "log.h"
#include "config.h"
#include "tree_io.h"
#include "graphviz.h"

static void insert_node_in_sorted_order(BinTree *tree, Node *new_node);

BinTreeError tree_insert_node(BinTree *tree, tree_el_t value)
{
    assert(tree);

    FLOG("<h1>INSERT NODE WITH VALUE " TREE_EL_SPECIFICATOR "</h1>", value);
    tree_dump(tree);

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

    FLOG("INSERT NODE WITH VALUE " TREE_EL_SPECIFICATOR " successful", value);
    tree_dump(tree);

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

    // в дебаг моде
    if (write_graph(tree, filename, 0))
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
