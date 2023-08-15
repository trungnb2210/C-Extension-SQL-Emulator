#ifndef TABLE_H
#define TABLE_H
#include "arraylist.h"

typedef struct {
    ArrayList* fields;
    ArrayList* records;
} Table;
extern void free_table(Table*, ArrayList*);
extern Table* init_table(void);
extern Table* make_table_copy(Table*);

#endif