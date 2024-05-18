#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

void send_command_menu() {
    printf("\033[0;33m____________________________________\n");
    printf("__________Вот ваше меню__________\n");
    printf("____Выберите одну из опций____\n");
    printf("[1] Добавить новый элемент\n");
    printf("[2] Удалить элемент\n");
    printf("[3] Поиск по ключу\n");
    printf("[4] Вывести таблицу\n");
    printf("[5] Экспортировать таблицу в файл\n");
    printf("[6] Импортировать таблицу из файла\n");
    printf("[7] Поиск всех версий элемента по ключу\n");
    printf("[8] Удаление всех версий элемента по ключу\n");
    printf("[9] Выход\n");
    printf("____________________________________\033[0m\n");
}

void menu(Table *mainTable) {
    printf("\033[H\033[J");
    send_command_menu();
    while (1) {
        printf("\033[1;90mВведите команду (число [1-9]):\033[0;0m\n");
        int command;
        if (scanf("%d", &command) != 1) {
            printf("Некорректный ввод, попробуйте еще раз\n");
            scanf("%*[^\n]*c");
            continue;
        }
        switch (command) {
            case 1:
                printf("Введите данные нового элемента: ");
                char info[100];
                scanf("%s", info);

                printf("Введите ключ элемента: ");
                KeyType key;
                if (scanf("%u", &key) != 1) {
                    printf("Некорректный ввод, попробуйте еще раз\n");
                    scanf("%*[^\n]*c");
                    continue;
                }

                printf("Введите номер версии элемента: ");
                RelType release;
                if (scanf("%u", &release) != 1) {
                    printf("Некорректный ввод, попробуйте еще раз\n");
                    scanf("%*[^\n]*c");
                    continue;
                }

                insertIntoTable(mainTable, key, release, info);
                break;

            case 2:
                printf("Введите ключ элемента для удаления: ");
                if (scanf("%u", &key) != 1) {
                    printf("Некорректный ввод, попробуйте еще раз\n");
                    scanf("%*[^\n]*c");
                    continue;
                }
                deleteFromTable(mainTable, key);
                break;

            case 3:
                printf("Введите ключ для поиска: ");
                if (scanf("%u", &key) != 1) {
                    printf("Некорректный ввод, попробуйте еще раз\n");
                    scanf("%*[^\n]*c");
                    continue;
                }
                KeySpace *result = findInTable(mainTable, key);
                if (result != NULL) {
                    printf("Найденный элемент:\n");
                    printf("Ключ: %u, Release: %u, Info: %s\n", result->key, result->release, result->info);
                } else {
                    printf("Элемент с ключом %u не найден\n", key);
                }
                break;

            case 4:
                printf("Содержимое таблицы:\n");
                printTable(mainTable);
                break;

            case 5:
                printf("Введите имя файла для экспорта: ");
                char export_filename[100];
                scanf("%s", export_filename);
                exportDataToFile(mainTable, export_filename);
                break;

            case 6:
                printf("Введите имя файла для импорта: ");
                char import_filename[100];
                scanf("%s", import_filename);
                importDataFromFile(mainTable, import_filename);
                break;

            case 7:
                printf("Введите ключ для поиска всех версий элемента: ");
                if (scanf("%u", &key) != 1) {
                    printf("Некорректный ввод, попробуйте еще раз\n");
                    scanf("%*[^\n]*c");
                    continue;
                }
                KeySpace *all_versions = findAllVersionsByKey(mainTable, key);
                if (all_versions != NULL) {
                    printf("Найденные версии элемента с ключом %u:\n", key);
                    KeySpace *current = all_versions;
                    while (current != NULL) {
                        printf("Ключ: %u, Release: %u, Info: %s\n", current->key, current->release, current->info);
                        current = current->next;
                    }
                    // Освобождаем память, выделенную под найденные версии
                    current = all_versions;
                    while (current != NULL) {
                        KeySpace *temp = current->next;
                        free(current->info);
                        free(current);
                        current = temp;
                    }
                } else {
                    printf("Элемент с ключом %u не найден\n", key);
                }
                break;

            case 8:
                printf("Введите ключ для удаления всех версий элемента: ");
                if (scanf("%u", &key) != 1) {
                    printf("Некорректный ввод, попробуйте еще раз\n");
                    scanf("%*[^\n]*c");
                    continue;
                }
                deleteAllVersionsByKey(mainTable, key);
                break;

            case 9:
                return;

            default:
                printf("Неверная команда, пожалуйста, попробуйте еще раз\n");
                break;
        }
    }
}
