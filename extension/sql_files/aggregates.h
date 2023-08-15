#ifndef AGGREGATES_H
#define AGGREGATES_H
#include "utils.h"

extern char* sum(ArrayList*);
extern char* count(ArrayList*);
extern char* avg(ArrayList*);
extern char* min_agg(ArrayList*);
extern char* max_agg(ArrayList*);
typedef char* (*aggregate)(ArrayList*);
extern aggregate parse_agg(char *);

#endif