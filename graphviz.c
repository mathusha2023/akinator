#include "graphviz.h"
#include <stdio.h>
#include "log.h"
#include "bin_tree.h"

static void draw_pretty_node(Node *node, FILE *file);
static void draw_debug_node(Node *node, FILE *file);

// mode: 0 - debug, 1 - pretty
int write_graph(BinTree *tree, const char *filename, int mode)
{
    assert(tree);
    assert(filename);

    if (mode == 1)
    {
        if (generate_pretty_graph_file(tree))
        {
            FLOG("Error: can not generate graph");
            return 1;
        }
    }
    else
    {
        if (generate_debug_graph_file(tree))
        {
            FLOG("Error: can not generate graph");
            return 1;
        }
    }

    char command[MAX_SYS_COMMAND_LENGTH] = {};
    if (snprintf(command, sizeof(command), "dot %s -Tpng -o %s", DOT_FILE_NAME, filename) < 0)
    {
        FLOG("Error: can not write sys command to buf");
        return 1;
    }

    system(command);
    return 0;
}

int generate_pretty_graph_file(BinTree *tree)
{
    assert(tree);

    FILE *file = fopen(DOT_FILE_NAME, "w");
    if (file == NULL)
    {
        FLOG("Can not open file %s", DOT_FILE_NAME);
        return 1;
    }

    fprintf(file, "digraph {\n");
    fprintf(file, "  rankdir = TB;\n  nodesep=0.6;\n  ranksep=1.0;\n  node [shape = Mrecord; style = filled;];\n  edge [color = \"%s\";];\n\n", EDGE_COLOR);

    if (tree->root)
        draw_pretty_node(tree->root, file);

    fprintf(file, "}\n");
    fclose(file);

    return 0;
}

int generate_debug_graph_file(BinTree *tree)
{
    assert(tree);

    FILE *file = fopen(DOT_FILE_NAME, "w");
    if (file == NULL)
    {
        FLOG("Can not open file %s", DOT_FILE_NAME);
        return 1;
    }

    fprintf(file, "digraph {\n");
    fprintf(file, "  rankdir = TB;\n  nodesep=0.6;\n  ranksep=1.0;\n  node [shape = Mrecord; style = filled;];\n  edge [color = \"%s\";];\n\n", EDGE_COLOR);

    if (tree->root)
        draw_debug_node(tree->root, file);

    fprintf(file, "}\n");
    fclose(file);

    return 0;
}

static void draw_pretty_node(Node *node, FILE *file)
{
    assert(node);
    assert(file);

    fprintf(file, "  V%p [fontsize=%d; fontcolor=\"%s\"; label = \"\\\"" TREE_EL_SPECIFICATOR "\\\"\"; fillcolor=\"%s\"; color=\"%s\"]\n",
            node, NODE_FONT_SIZE, NODE_FONT_COLOR, node->data, NODE_FILL_COLOR, NODE_COLOR);

    if (node->left)
    {
        draw_pretty_node(node->left, file);
        fprintf(file, "  V%p -> V%p [label=\"ДА\"]\n", node, node->left);
    }

    if (node->right)
    {
        draw_pretty_node(node->right, file);
        fprintf(file, "  V%p -> V%p [label=\"НЕТ\"]\n", node, node->right);
    }
}

static void draw_debug_node(Node *node, FILE *file)
{
    assert(node);
    assert(file);

    fprintf(file, "  V%p [fontsize=%d; fontcolor=\"%s\"; label = \"\\\"" TREE_EL_SPECIFICATOR "\\\"\"; fillcolor=\"%s\"; color=\"%s\"]\n",
            node, NODE_FONT_SIZE, NODE_FONT_COLOR, node->data, NODE_FILL_COLOR, NODE_COLOR);

    if (node->left)
    {
        draw_debug_node(node->left, file);
        fprintf(file, "  V%p -> V%p [label=\"lt\"]\n", node, node->left);
    }

    if (node->right)
    {
        draw_debug_node(node->right, file);
        fprintf(file, "  V%p -> V%p [label=\"rt\"]\n", node, node->right);
    }
}