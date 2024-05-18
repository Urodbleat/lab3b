#include "hash_table.h"

KeySpace *createKeySpace(KeyType key, RelType release, InfoType info) {
    KeySpace *newKeySpace = (KeySpace *)malloc(sizeof(KeySpace));
    if (newKeySpace == NULL) {
        return NULL;
    }
    newKeySpace->key = key;
    newKeySpace->release = release;
    newKeySpace->info = strdup(info);
    if (newKeySpace->info == NULL) {
        free(newKeySpace);
        return NULL;
    }
    newKeySpace->next = NULL;
    return newKeySpace;
}

Table *createTable(IndexType msize) {
    Table *newTable = (Table *)malloc(sizeof(Table));
    if (newTable == NULL) {
        return NULL;
    }
    newTable->ks = (KeySpace **)malloc(msize * sizeof(KeySpace *));
    if (newTable->ks == NULL) {
        free(newTable);
        return NULL;
    }
    for (int i = 0; i < msize; i++) {
        newTable->ks[i] = NULL;
    }
    newTable->msize = msize;
    newTable->csize = 0;
    return newTable;
}

int insertIntoTable(Table *table, KeyType key, RelType release, InfoType info) {
    unsigned int hash = key % table->msize;
    KeySpace *newKeySpace = createKeySpace(key, release, info);
    if (newKeySpace == NULL) {
        return MEMORY_ERROR;
    }
    if (table->ks[hash] == NULL) {
        table->ks[hash] = newKeySpace;
    } else {
        KeySpace *current = table->ks[hash];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newKeySpace;
    }
    table->csize++;
    return 0;
}

int deleteFromTable(Table *table, KeyType key) {
    unsigned int hash = key % table->msize;
    KeySpace *current = table->ks[hash];
    KeySpace *prev = NULL;
    while (current != NULL) {
        if (current->key == key) {
            if (prev == NULL) {
                table->ks[hash] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->info);
            free(current);
            table->csize--;
            return 0;
        }
        prev = current;
        current = current->next;
    }
    return INPUT_ERROR;
}

KeySpace *findInTable(Table *table, KeyType key) {
    unsigned int hash = key % table->msize;
    KeySpace *current = table->ks[hash];
    while (current != NULL) {
        if (current->key == key) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void printTable(Table *table) {
    for (int i = 0; i < table->msize; i++) {
        printf("Slot %d:\n", i);
        KeySpace *current = table->ks[i];
        while (current != NULL) {
            printf("Key: %u, Release: %u, Info: %s\n", current->key, current->release, current->info);
            current = current->next;
        }
    }
}

int importDataFromFile(Table *table, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return INPUT_ERROR;
    }
    KeyType key;
    RelType release;
    char info[100];
    while (fscanf(file, "%u %u %99[^\n]", &key, &release, info) == 3) {
        insertIntoTable(table, key, release, info);
    }
    fclose(file);
    return 0;
}

int exportDataToFile(Table *table, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return INPUT_ERROR;
    }
    for (int i = 0; i < table->msize; i++) {
        KeySpace *current = table->ks[i];
        while (current != NULL) {
            fprintf(file, "%u %u %s\n", current->key, current->release, current->info);
            current = current->next;
        }
    }
    fclose(file);
    return 0;
}

KeySpace *findAllVersionsByKey(Table *table, KeyType key) {
    KeySpace *result = NULL;
    for (int i = 0; i < table->msize; i++) {
        KeySpace *current = table->ks[i];
        while (current != NULL) {
            if (current->key == key) {
                KeySpace *newKeySpace = createKeySpace(current->key, current->release, current->info);
                if (newKeySpace == NULL) {
                    return NULL;
                }
                newKeySpace->next = result;
                result = newKeySpace;
            }
            current = current->next;
        }
    }
    return result;
}

int deleteAllVersionsByKey(Table *table, KeyType key) {
    int deleted = 0;
    for (int i = 0; i < table->msize; i++) {
        KeySpace *current = table->ks[i];
        KeySpace *prev = NULL;
        while (current != NULL) {
            if (current->key == key) {
                if (prev == NULL) {
                    table->ks[i] = current->next;
                } else {
                    prev->next = current->next;
                }
                free(current->info);
                free(current);
                deleted++;
            }
            prev = current;
            current = current->next;
        }
    }
    table->csize -= deleted;
    return deleted;
}
