// hash_table.h
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Error {
    MEMORY_ERROR,
    INPUT_ERROR,
};

// Типы данных
typedef unsigned int KeyType;
typedef char * InfoType;
typedef unsigned int RelType;
typedef unsigned int IndexType;

// Структура элемента пространства ключей
typedef struct KeySpace {
    KeyType key;
    RelType release;
    InfoType info;
    struct KeySpace *next;
} KeySpace;

// Структура хеш-таблицы
typedef struct Table {
    KeySpace **ks;
    IndexType msize;
    IndexType csize;
} Table;

// Функции для работы с хеш-таблицей
KeySpace *createKeySpace(KeyType key, RelType release, InfoType info);
Table *createTable(IndexType msize);
int insertIntoTable(Table *table, KeyType key, RelType release, InfoType info);
int deleteFromTable(Table *table, KeyType key);
KeySpace *findInTable(Table *table, KeyType key);
void printTable(Table *table);
int importDataFromFile(Table *table, const char *filename);
int exportDataToFile(Table *table, const char *filename);
KeySpace *findAllVersionsByKey(Table *table, KeyType key);
int deleteAllVersionsByKey(Table *table, KeyType key);

#endif // HASH_TABLE_H
