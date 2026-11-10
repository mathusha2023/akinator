#include "graphviz.h"
#include <stdio.h>
#include "log.h"
#include "bin_tree.h"

static void draw_node(Node *node, FILE *file, const char *left_label, const char *right_label);

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

int generate_graph_file(BinTree *tree, const char *left_label, const char *right_label)
{
    assert(tree);

    FILE *file = fopen(DOT_FILE_NAME, "w");
    if (file == NULL)
    {
        FLOG("Can not open file %s", DOT_FILE_NAME);
        return 1;
    }

    fprintf(file, "digraph {\n");
    fprintf(file, "  rankdir = TB;\n  nodesep=0.6;\n  ranksep=1.0;\n  node [shape = Mrecord; style = filled;];\n\n");

    if (tree->root)
        draw_node(tree->root, file, left_label, right_label);

    fprintf(file, "}\n");
    fclose(file);

    return 0;
}

int generate_pretty_graph_file(BinTree *tree)
{
    assert(tree);

    return generate_graph_file(tree, "ДА", "НЕТ");
}

int generate_debug_graph_file(BinTree *tree)
{
    assert(tree);

    return generate_graph_file(tree, "L", "R");
}

static void draw_node(Node *node, FILE *file, const char *left_label, const char *right_label)
{
    assert(node);
    assert(file);

    fprintf(file, "  V%p [fontsize=%d; fontcolor=\"%s\"; label = \"\\\"" TREE_EL_SPECIFICATOR "\\\"\"; fillcolor=\"%s\"; color=\"%s\"]\n",
            node, NODE_FONT_SIZE, NODE_FONT_COLOR, node->data, NODE_FILL_COLOR, NODE_COLOR);

    if (node->left)
    {
        draw_node(node->left, file, left_label, right_label);
        fprintf(file, "  V%p -> V%p [fontsize=%d; fontcolor=\"%s\"; label=\"%s\"; color=\"%s\";]\n",
                node, node->left, ARROW_HINT_FONT_SIZE, LEFT_EDGE_COLOR, left_label, LEFT_EDGE_COLOR);
    }

    if (node->right)
    {
        draw_node(node->right, file, left_label, right_label);
        fprintf(file, "  V%p -> V%p [fontsize=%d; fontcolor=\"%s\"; label=\"%s\"; color=\"%s\";]\n",
                node, node->right, ARROW_HINT_FONT_SIZE, RIGHT_EDGE_COLOR, right_label, RIGHT_EDGE_COLOR);
    }
}
