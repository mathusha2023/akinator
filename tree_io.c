#include "tree_io.h"
#include <stdio.h>
#include <string.h>
#include "bin_tree.h"
#include "log.h"
#include "my_string.h"
#include "files.h"

static TreeIOError parse_node_from_string(String string, Node **node_ptr, size_t *nodes_append);
static TreeIOError parse_node_data_from_string(String string, char **node_data_ptr, size_t *end_index);
static TreeIOError log_and_return_incorrect_input_format_error(String string, const char *special_info);

TreeIOError write_tree_in_file_by_filename(BinTree *tree, const char *filename, TreeOrders order)
{
    assert(tree);
    assert(filename);

    FILE *file = fopen(filename, "w");
    if (!file)
    {
        FLOG("ERROR: cant open file %s for writing tree:(", filename);
        return TREE_IO_FILE_ERROR;
    }

    TreeIOError error = write_tree_in_file(tree, file, order);
    fclose(file);

    return error;
}

TreeIOError write_tree_in_file(BinTree *tree, FILE *file, TreeOrders order)
{
    assert(tree);
    assert(file);

    TreeIOError error = write_node_in_file(tree->root, file, order);
    if (error != TREE_IO_OK)
    {
        FLOG("ERROR: cant write root in file: %s", tree_io_get_str_error(error));
        return error;
    }

    return TREE_IO_OK;
}

TreeIOError write_node_in_file(Node *node, FILE *file, TreeOrders order)
{
    assert(file);

    TreeIOError error = TREE_IO_OK;

    // если был передан NULL в качестве узла то просто пишем его и выходим
    if (!node)
    {
        if (fprintf(file, "nil") < 0)
        {
            FLOG("ERROR: cant write 'nil' in file <%p>", file);
            return TREE_IO_FILE_ERROR;
        }
        return error;
    }

    if (fprintf(file, "(") < 0)
    {
        FLOG("ERROR: cant write '(' in file <%p>", file);
        return TREE_IO_FILE_ERROR;
    }

    if (order == PREORDER)
    {
        if (fprintf(file, "\"" TREE_EL_SPECIFICATOR "\" ", node->data) < 0)
        {
            FLOG("ERROR: cant write '" TREE_EL_SPECIFICATOR "' in file <%p>", node->data, file);
            return TREE_IO_FILE_ERROR;
        }
    }

    // пишем левый
    error = write_node_in_file(node->left, file, order);
    if (error != TREE_IO_OK)
    {
        FLOG("ERROR: cant write node in file: %s", tree_io_get_str_error(error));
        return error;
    }

    // пробел - разделитель
    if (fprintf(file, " ") < 0)
    {
        FLOG("ERROR: cant write ' ' in file <%p>", file);
        return TREE_IO_FILE_ERROR;
    }

    if (order == INORDER)
    {
        if (fprintf(file, "\"" TREE_EL_SPECIFICATOR "\" ", node->data) < 0)
        {
            FLOG("ERROR: cant write '" TREE_EL_SPECIFICATOR "' in file <%p>", node->data, file);
            return TREE_IO_FILE_ERROR;
        }
    }

    // пишем правый
    error = write_node_in_file(node->right, file, order);
    if (error != TREE_IO_OK)
    {
        FLOG("ERROR: cant write node in file: %s", tree_io_get_str_error(error));
        return error;
    }

    if (order == POSTORDER)
    {
        if (fprintf(file, " \"" TREE_EL_SPECIFICATOR "\"", node->data) < 0)
        {
            FLOG("ERROR: cant write '" TREE_EL_SPECIFICATOR "' in file <%p>", node->data, file);
            return TREE_IO_FILE_ERROR;
        }
    }

    if (fprintf(file, ")") < 0)
    {
        FLOG("ERROR: cant write ')' in file <%p>", file);
        return TREE_IO_FILE_ERROR;
    }

    return TREE_IO_OK;
}

TreeIOError parse_tree_from_file_by_filename(BinTree *tree, const char *filename)
{
    assert(tree);
    assert(filename);

    String string = {};

    int file_error = read_file_to_string(filename, &string);
    if (file_error != 0)
    {
        FLOG("Error while reading tree to file: %d", file_error);
        return TREE_IO_FILE_ERROR;
    }

    TreeIOError error = parse_tree_from_string(tree, string);
    if (error != TREE_IO_OK)
    {
        FLOG("Error while parsing tree from string: %s", tree_io_get_str_error(error));
        return error;
    }

    free_ptr(string.p);
    string = (String){};

    return TREE_IO_OK;
}

TreeIOError parse_tree_from_string(BinTree *tree, String string)
{
    assert(tree);
    assert(string.p);
    assert(string.length);

    size_t nodes_append = 0;

    TreeIOError error = parse_node_from_string(string, &tree->root, &nodes_append);
    if (error != TREE_IO_OK)
    {
        char *s = copy_string_to_char_arr(string);
        if (s)
        {
            FLOG("ERROR while parsing tree from string <%s>: %s", s, tree_io_get_str_error(error));
            free_ptr(s);
        }
        else
        {
            FLOG("ERROR while parsing tree: %s", tree_io_get_str_error(error));
        }
        return error;
    }

    tree->n_nodes = nodes_append;

    tree_dump(tree);

    return TREE_IO_OK;
}

const char *tree_io_get_str_error(TreeIOError error)
{
    return STR_TREE_IO_ERRORS[error];
}

static TreeIOError parse_node_from_string(String string, Node **node_ptr, size_t *nodes_append)
{
    assert(string.p);
    assert(string.length);
    assert(node_ptr);
    assert(!*node_ptr);

    if (string.length < 5 || string.p[0] != '(' || string.p[string.length - 1] != ')' || string.p[1] != '"')
    {
        return log_and_return_incorrect_input_format_error(string, "length should be >=5, (\"...\") string format should be");
    }

    // избавляемся от круглых скобок вокруг строки
    string.p++;
    string.length -= 2;

    char *node_data = NULL;
    size_t i = 0;

    TreeIOError error = parse_node_data_from_string(string, &node_data, &i);
    if (error != TREE_IO_OK)
    {
        FLOG("Error while parsing node data from string");
        return error;
    }

    assert(string.length - i > 5);

    Node *new_node = (Node *)calloc(1, sizeof(Node));
    if (!new_node)
    {
        FLOG("Error: can not allocate memory for new node");
        free_ptr(node_data);
        return TREE_IO_MEMORY_ERROR;
    }
    new_node->data = node_data;

    i++;
    if (string.p[i] != ' ')
    {
        free_ptr(node_data);
        free_ptr(new_node);
        return log_and_return_incorrect_input_format_error(string, NULL);
    }
    i++;

    // если там nil
    if (string.p[i] == 'n' && string.p[i + 1] == 'i' && string.p[i + 2] == 'l')
    {
        i += 3;
    }
    else
    {
        size_t bracket_seq_length = get_min_bracket_sequence_length((String){.p = string.p + i, .length = string.length - i});

        // последовательность не нашлась или она неправильная
        if (!bracket_seq_length)
        {
            free_ptr(node_data);
            free_ptr(new_node);
            return log_and_return_incorrect_input_format_error(string, NULL);
        }

        error = parse_node_from_string((String){.p = string.p + i, .length = bracket_seq_length}, &new_node->left, nodes_append);
        if (error != TREE_IO_OK)
        {
            FLOG("Error while recursivly call parsing for left child: %s", tree_io_get_str_error(error));
            free_ptr(node_data);
            free_ptr(new_node);
            return error;
        }

        i += bracket_seq_length;
    }

    if (string.p[i] != ' ')
    {
        free_ptr(node_data);
        free_ptr(new_node);
        return log_and_return_incorrect_input_format_error(string, NULL);
    }
    i++;

    // если там не nil
    if (!(string.p[i] == 'n' && string.p[i + 1] == 'i' && string.p[i + 2] == 'l'))
    {
        error = parse_node_from_string((String){.p = string.p + i, .length = string.length - i}, &new_node->right, nodes_append);
        if (error != TREE_IO_OK)
        {
            FLOG("Error while recursivly call parsing for right child: %s", tree_io_get_str_error(error));
            free_ptr(node_data);
            free_ptr(new_node);
            return error;
        }
    }

    *node_ptr = new_node;
    (*nodes_append)++;

    return TREE_IO_OK;
}

// по указателю на массив чаров записываем наши спаршенные данные
// в end_index кладем индекс последней кавычке в строке
static TreeIOError parse_node_data_from_string(String string, char **node_data_ptr, size_t *end_index)
{
    assert(node_data_ptr);
    assert(end_index);

    char buff[MAX_NODE_DATA_LENGTH + 1] = {};

    size_t i = 1;
    for (i = 1; i < string.length && i < MAX_NODE_DATA_LENGTH && string.p[i] != '"'; i++)
    {
        buff[i - 1] = string.p[i];
    }
    buff[i] = '\0';

    // вторая кавычка не нашлась
    if (i == string.length)
    {
        return log_and_return_incorrect_input_format_error(string, "there are no the second quote");
    }
    // слишком длинная строка данных
    else if (i == MAX_NODE_DATA_LENGTH)
    {
        return log_and_return_incorrect_input_format_error(string, "node data length is very long");
    }

    char *node_data = (char *)calloc(i + 1, sizeof(char));
    if (!node_data)
    {
        FLOG("Error: can not allocate memory for node data string");
        return TREE_IO_MEMORY_ERROR;
    }

    strncpy(node_data, buff, i + 1);

    *node_data_ptr = node_data;
    *end_index = i;

    return TREE_IO_OK;
}

// в special_info можно передать строку которая будет выведена после дефолтного сообщения об ошибке
// либо NULL если ничего лишнего не нужно
static TreeIOError log_and_return_incorrect_input_format_error(String string, const char *special_info)
{
    const char *const mes = "Error: incorrect string input";
    char *s = copy_string_to_char_arr(string);

    if (special_info)
    {
        if (s)
        {
            FLOG("%s (%s): <%s>", mes, s, special_info);
            free_ptr(s);
        }
        else
        {
            FLOG("%s (%s)", mes, special_info);
        }
    }
    else
    {
        if (s)
        {
            FLOG("%s: <%s>", mes, s);
            free_ptr(s);
        }
        else
        {
            FLOG("%s", mes);
        }
    }
    return TREE_IO_INCORRECT_STRING_FORMAT;
}