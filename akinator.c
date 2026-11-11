#include "akinator.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "bin_tree.h"
#include "tree_io.h"
#include "config.h"
#include "log.h"
#include "graphviz.h"
#include "stack.h"

static void print_menu(void);
static int get_command(void);
static void flush_console(void);
static void add_new_answer(BinTree *tree, Node *curr_node);
static char *read_str(void);
static int make_description_stack(char *data, Node *node, Stack *data_stack, Stack *direct_stack);

void main_loop(BinTree *tree)
{
    assert(tree);

    printf("Добро пожаловать в акинатора by BAA! ");

    int cmd = 0;

    while (1)
    {
        print_menu();

        cmd = get_command();

        switch (cmd)
        {
        case 0:
            sohranyalka(tree);
            return;
        case 1:
            ugadaika(tree);
            break;
        case 2:
            glyadelka(tree);
            break;
        case 3:
            smotrelka(tree);
            break;
        case 4:
            printf("SRAVNILKA\n");
            break;
        default:
            printf("Чета хуйня какая то\n");
            return;
        }
    }
}

void ugadaika(BinTree *tree)
{
    assert(tree);

    Node *curr_node = tree->root;

    char c = 0;
    int right_answer = 0;

    while (1)
    {
        printf("%s? [1]ДА / [2]НЕТ\n", curr_node->data);
        printf("> ");

        c = (char)getchar();
        flush_console();

        if (c == '1')
        {
            if (curr_node->left)
                curr_node = curr_node->left;
            else
            {
                right_answer = 1;
                break;
            }
        }
        else if (c == '2')
        {
            if (curr_node->right)
                curr_node = curr_node->right;
            else
            {
                right_answer = 0;
                break;
            }
        }
        else
            printf("Выберите корректный вариант ответа!\n");
    }

    if (right_answer)
    {
        printf("Ха-ха-ха и снова я угадал!\n\n\n");
        return;
    }

    printf("Удивительно, нашлось что-то, чего я не знаю... ");
    add_new_answer(tree, curr_node);

    printf("\n\n\n");
}

void glyadelka(BinTree *tree)
{
    assert(tree);

    int error = write_graph(tree, PRETTY_GRAPH_FILE_NAME, 1);
    if (error)
    {
        printf("УПС я не смог собрать изображение текущего дерева, сори(");
        return;
    }

    const char *const COMMAND = "open %s";

    char cmd[MAX_FILENAME_LENGTH] = {};
    if (snprintf(cmd, MAX_FILENAME_LENGTH, COMMAND, PRETTY_GRAPH_FILE_NAME) < 0)
        return;

    system(cmd);

    printf("\n\n\n");
}

void smotrelka(BinTree *tree)
{
    assert(tree);

    printf("Про какой обьект вы хотите посмотреть информацию? Введите его название\n");
    printf("> ");

    char *name = read_str();
    if (!name)
    {
        printf("УПС не смог получить информацию, сорри(");
        return;
    }

    Stack data_stack = {}, direction_stack = {};
    init_stack(&data_stack, 100);
    init_stack(&direction_stack, 100);

    int res = make_description_stack(name, tree->root, &data_stack, &direction_stack);
    if (!res)
    {
        printf("Обьекта с именем %s не найдено в нашем дереве! Попробуйте изучить что-нибудь существующее, а лучше конечно матан\n", name);
    }
    else
    {
        printf("Вот описание %s: ", name);

        char *desc_data = NULL;
        char *desc_direction = NULL;
        size_t size1 = get_size(&data_stack), size2 = get_size(&direction_stack);
        for (size_t i = 0; i < size1 && i < size2; i++)
        {
            desc_data = data_stack.data[i];
            desc_direction = direction_stack.data[i];
            printf("%s %s -> ", desc_direction, desc_data);
        }
        printf("%s", name);
    }

    free_ptr(name);
    destroy_stack(&data_stack);
    destroy_stack(&direction_stack);

    printf("\n\n\n");
}

void sohranyalka(BinTree *tree)
{
    assert(tree);

    char c = 0;

    printf("Нажмите [1], чтобы сохранить новое дерево акинатора перед выходом, или любую другую клавишу для выхода\n");
    printf("> ");

    c = (char)getchar();
    if (c != '\n')
        flush_console();

    if (c != '1')
        return;

    TreeIOError error = write_tree_in_file_by_filename(tree, DATA_FILE_NAME, PREORDER);
    if (error != TREE_IO_OK)
    {
        FLOG("Error while saving tree to file %s", DATA_FILE_NAME);
        printf("УПС ошибка сохранения файла базы данных\n");
    }
    else
    {
        printf("Сохранено!\n");
    }
}

static void print_menu(void)
{
    printf("Выберите один из вариантов ниже:\n");
    printf("[1]. Угадать кого-то (что-то) или добавить новый обьект\n");
    printf("[2]. Посмотреть базу ответов\n");
    printf("[3]. Посмотреть информацию об обьекте\n");
    printf("[4]. Сравнить 2 обьекта\n");
    printf("[0]. Выйти из программы\n");
}

static int get_command(void)
{
    printf("> ");
    char c = (char)getchar();
    flush_console();

    while (c != '1' && c != '2' && c != '3' && c != '4' && c != '0')
    {
        printf("Некорректный ввод! Выберите один из приведенных вариантов!\n");
        printf("> ");
        c = (char)getchar();
        flush_console();
    }

    return (int)(c - '0');
}

static void flush_console(void)
{
    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

static void add_new_answer(BinTree *tree, Node *curr_node)
{
    assert(tree);
    assert(curr_node);

    printf("Кто это на самом деле?\n");
    printf("> ");

    char *who_is = read_str();
    if (!who_is)
        return;

    printf("Отлично! Чем %s отличается от %s? Он ...\n", who_is, curr_node->data);
    printf("> ");

    char *diff = read_str();
    if (!diff)
        return;

    // вставляем в поле ДА загаданную пользователем субстанцию
    BinTreeError error = tree_insert_node(tree, curr_node, LEFT_CHILD, who_is);
    if (error != TREE_OK)
    {
        FLOG("Error while inserting node with value %s!", who_is);
        free_ptr(who_is);
        free_ptr(diff);
        return;
    }

    // вставляем в поле НЕТ значение, которое раньше было в текущем листе
    error = tree_insert_node(tree, curr_node, RIGHT_CHILD, curr_node->data);
    if (error != TREE_OK)
    {
        FLOG("Error while inserting node with value %s!", curr_node->data);
        free_ptr(who_is);
        free_ptr(diff);
        return;
    }

    curr_node->data = diff;

    printf("Запомнил! Ну все, теперь точно не ошибусь!!!\n");
}

// NULL в случае ошибки
static char *read_str(void)
{
    char buff[MAX_NODE_DATA_LENGTH] = {};

    int c = getchar();
    size_t i = 0;

    for (i = 0; c != '\n' && c != EOF; i++)
    {
        buff[i] = (char)c;
        c = getchar();
    }
    buff[i++] = '\0';

    char *str = (char *)calloc(i, sizeof(char));
    if (!str)
    {
        return NULL;
    }

    strncpy(str, buff, i);

    return str;
}

// 0 - не наход на данной ветке, 1 - нашли значение
static int make_description_stack(char *data, Node *node, Stack *data_stack, Stack *direct_stack)
{
    assert(data);
    assert(data_stack);
    assert(direct_stack);

    if (!node)
        return 0;

    char *buf = NULL;

    if (!strcmp(node->data, data))
        return 1;

    push_stack(data_stack, node->data);

    push_stack(direct_stack, "");
    int res = make_description_stack(data, node->left, data_stack, direct_stack);
    if (res)
        return 1;
    pop_stack(direct_stack, &buf);

    push_stack(direct_stack, "не");
    res = make_description_stack(data, node->right, data_stack, direct_stack);
    if (res)
        return 1;
    pop_stack(direct_stack, &buf);

    pop_stack(data_stack, &buf);

    return 0;
}
