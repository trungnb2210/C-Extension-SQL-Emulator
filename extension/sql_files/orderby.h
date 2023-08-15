#ifndef ORDERBY_H
#define ORDERBY_H
#include "arraylist.h"
#include "utils.h"
#include "table.h"

Table* parse_order(Table* result, ArrayList* operands);
void bubbleSort(void **, int , int *, int );
int record_comparator(ArrayList*, ArrayList*, int *, int);
#endif
