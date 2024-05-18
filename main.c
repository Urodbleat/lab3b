#include <stdio.h>
#include "hash_table.h"
#include "menu.h"

int main() {
    Table *mainTable = createTable(10);
    if (mainTable == NULL) {
        return MEMORY_ERROR;
    }

    menu(mainTable);

    return 0;
}
