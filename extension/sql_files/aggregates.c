#include "aggregates.h"
#include <stdlib.h>

#define OUTPUT_SIZE 100


/*
Reduction function that calculates the sum of all values in an arraylist
The arraylist is expected to contains strings of integers
*/
char* sum(ArrayList* al) {
    char* buf = malloc(OUTPUT_SIZE * sizeof(char));
    int s = 0;
    for (int i = 0; i < al->size; i++) {
        s += atoi(al->array[i]);
    }
    sprintf(buf, "%d", s);
    return buf;
}

/*
Reduction function that calcluates the number of items in an arraylits
*/
char* count(ArrayList* al) {
    char* buf = malloc(OUTPUT_SIZE * sizeof(char));
    int count = al->size;
    sprintf(buf, "%d", count);
    return buf;
}

/*
Reduction function that calculates the average of items in an arraylist (using integer division)
The arraylist is expected to contains strings of integers
*/
char* avg(ArrayList* al) {
    char* buf = malloc(OUTPUT_SIZE * sizeof(char));
    int s = 0;
    for (int i = 0; i < al->size; i++) {
        s += atoi(al->array[i]);
    }
    sprintf(buf, "%d", s / al->size);
    return buf;
}

/*
Reduction function that calculates the minimum value in an arraylist
The arraylist is expected to contains strings of integers
*/
char* min_agg(ArrayList* al) {
    char* buf = malloc(OUTPUT_SIZE * sizeof(char));
    int s = atoi(al->array[0]);
    for (int i = 1; i < al->size; i++) {
        s = MIN(s, atoi(al->array[i]));
    }

    sprintf(buf, "%d", s);
    return buf;
}

/*
Reduction function that calculates the minimum value in an arraylist
The arraylist is expected to contains strings of integers
*/
char* max_agg(ArrayList* al) {
    char* buf = malloc(OUTPUT_SIZE * sizeof(char));
    int s = atoi(al->array[0]);
    for (int i = 1; i < al->size; i++) {
        s = MAX(s, atoi(al->array[i]));
    }

    sprintf(buf, "%d", s);
    return buf;
}

/*
Function which returns a function pointer to the appropriate reduction function
depending on the command used in the query
*/
aggregate parse_agg(char* newp) {
    if (!strcmp(newp, "SUM")) {
        return &sum;
    } else if (!strcmp(newp, "COUNT")) {
        return &count;
    } else if (!strcmp(newp, "AVG")) {
        return &avg;
    } else if (!strcmp(newp, "MIN")) {
        return &min_agg;
    } else if (!strcmp(newp, "MAX")) {
        return &max_agg;
    } else {
        exit(EXIT_FAILURE);
    }
}