#ifndef AKINATOR_H
#define AKINATOR_H

#include "bin_tree.h"

const char GREETING_PH[] = "Добро пожаловать в акинатора by BAA! ";
const char CHOOSE_ONE_VARIANT[] = "Выберите один из вариантов ниже:\n";
const char CHOOSE_CORRECT_VARIANT_PH[] = "Выберите корректный вариант ответа!\n";
const char I_WIN_PH[] = "Ха-ха-ха и снова я угадал!\n\n\n";
const char START_ADDING_PH[] = "Удивительно, нашлось что-то, чего я не знаю... ";
const char START_WATCHING_PH[] = "Про какой обьект вы хотите посмотреть информацию? Введите его название\n";
const char SAVE_PH[] = "Нажмите [1], чтобы сохранить новое дерево акинатора перед выходом, или любую другую клавишу для выхода\n";

void main_loop(BinTree *tree);

void ugadaika(BinTree *tree);
void glyadelka(BinTree *tree);
void smotrelka(BinTree *tree);
void sohranyalka(BinTree *tree);
void say(const char *s);

#endif // AKINATOR_H
