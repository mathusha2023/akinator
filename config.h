#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>

// #define DISABLE_LOGS

#define RED_COLOR "\x1b[31m"
#define GREEN_COLOR "\x1b[32m"
#define BLUE_COLOR "\x1b[34m"
#define YELLOW_COLOR "\x1b[33m"
#define CYAN_COLOR "\x1b[36m"
#define GREY_COLOR "\033[90m"
#define RESET_COLOR "\x1b[0m"

#define free_ptr(p) \
    {               \
        free(p);    \
        p = NULL;   \
    }

const char DOT_FILE_NAME[] = "log/generated_graph.dot";
const char LOGFILE_NAME[] = "log/log.html";
const char LOG_DIRECTORY[] = "log";
const char GRAPH_DIRECTORY[] = "log";
const char PROGRAM_NAME[] = "akinator";

const char DATA_FILE_NAME[] = "data.ak";
const char PRETTY_GRAPH_FILE_NAME[] = "seregasinshluhi.png";

const size_t MAX_FILENAME_LENGTH = 100;
const size_t MAX_SYS_COMMAND_LENGTH = 200;

const size_t MAX_NODE_DATA_LENGTH = 100;

const double EPSILON = 1e-6;

#endif // CONFIG_H