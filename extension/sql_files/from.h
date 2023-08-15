#ifndef FROM_H
#define FROM_H
#include "table.h"
#include "hashtable.h"
#include "arraylist.h"

Table* parse_from(HashTable, ArrayList*);
Table* product(Table*[], int);

#endif