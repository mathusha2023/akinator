#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include "bin_tree.h"

static const char *NODE_FILL_COLOR = "#E3F2FD";
static const char *NODE_COLOR = "#1E88E5";
static const char *NODE_FONT_COLOR = "#011a31";

const int NODE_FONT_SIZE = 16;

static const char *EDGE_COLOR = "#1E88E5";

int write_graph(BinTree *tree, const char *filename, int mode);
int generate_pretty_graph_file(BinTree *tree);
int generate_debug_graph_file(BinTree *tree);

#endif // GRAPHVIZ_H
