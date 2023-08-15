#include "utils.h"
#include "wherehaving.h"
#include <assert.h>
#include "orderby.h"

void test_evaluate_ast(void) {
    char *expression;
    YY_BUFFER_STATE buffer;

    expression = "1 + 2 == 3\n";
    buffer = yy_scan_string(expression);
    yyparse();
    assert(evaluate_ast(parsed_expression, NULL) == true);

    printf("evaluate_ast test 1 passed\n");

    yy_delete_buffer(buffer);

    expression = "5 * 5 + (-5 * 5) == 0\n";
    buffer = yy_scan_string(expression);
    yyparse();
    assert(evaluate_ast(parsed_expression, NULL) == true);

    printf("evaluate_ast test 2 passed\n");
    
    yy_delete_buffer(buffer);
    
    expression = "\"Bob\" == \"Alice\"\n";
    buffer = yy_scan_string(expression);
    yyparse();
    assert(evaluate_ast(parsed_expression, NULL) == false);

    printf("evaluate_ast test 3 passed\n");

    yy_delete_buffer(buffer);

    expression = "1 < 2 AND 2 < 3 AND 4 < 4\n";
    buffer = yy_scan_string(expression);
    yyparse();
    assert(evaluate_ast(parsed_expression, NULL) == false);

    printf("evaluate_ast test 4 passed\n");

    yy_delete_buffer(buffer);

    expression = "\"b\" < \"c\" AND 10 + 30 + 40 == 80\n";
    buffer = yy_scan_string(expression);
    yyparse();
    assert(evaluate_ast(parsed_expression, NULL) == true);

    printf("evaluate_ast test 5 passed\n");

    yy_delete_buffer(buffer);


    expression = "(\"b\" < \"c\" AND 10 + 30 == 80) OR (3 != 4)\n";
    buffer = yy_scan_string(expression);
    yyparse();
    assert(evaluate_ast(parsed_expression, NULL) == true);

    printf("evaluate_ast test 6 passed\n");

    yy_delete_buffer(buffer);

    printf("evaluate_ast tests have passed\n\n");

}

void test_value_in_arr(void) {
    ArrayList* array_list = initialize_arraylist();
    for (int i = 0; i < 10; i++) {
        int *add = malloc(sizeof(int));
        *add = i;
        add_to_arraylist(array_list, add);
    }
    assert(value_in_arr(array_list, 0) == true);

    printf("value_in_arr test 1 passed\n");

    assert(value_in_arr(array_list, 9) == true);

    printf("value_in_arr test 2 passed\n");

    assert(value_in_arr(array_list, 10) == false);

    printf("value_in_arr test 3 passed\n");

    printf("value_in_arr tests have passed\n\n");
}

void test_make_array_copy(void) {
    ArrayList* src = initialize_arraylist();
    char *elements[9] = {"a", "b", "c", "d", "e", "f", "g", "h", "hello"};
    for (int i = 0; i < 9; i++) {
        add_to_arraylist(src, strdup(elements[i]));
    }
    ArrayList* dest = make_array_copy(src);
    assert(dest->size == src->size);

    for (int i = 0; i < dest->size; i++) {
        assert(dest->array[i] != src->array[i]);

        assert(!strcmp(dest->array[i], src->array[i]));
    }
    printf("make_array_copy tests have passed\n\n");
}

void test_make_record_copy(void) {
    char *elements[9] = {"a", "b", "c", "d", "e", "f", "g", "h", "hello"};
    ArrayList* src_records = initialize_arraylist();
    for (int i = 0; i < 3; i++) {
        ArrayList* new_record = initialize_arraylist();
        for (int j = 0; j < 3; j++) {
            add_to_arraylist(new_record, strdup(elements[i * 3 + j]));
        }
        add_to_arraylist(src_records, new_record);
    }
    ArrayList* dst_records = make_record_copy(src_records);
    assert(src_records->size == dst_records->size);
    for (int i = 0; i < 3; i++) {
        ArrayList* src_record = src_records->array[i];
        ArrayList* dst_record = dst_records->array[i];
        assert(src_record != dst_record);
        assert(src_record->size == dst_record->size);
        for (int j = 0; j < 3; j++) {
            assert(dst_record->array[i] != src_record->array[i]);
            assert(!strcmp(dst_record->array[i], src_record->array[i]));
        }
    }
    printf("make_record_copy tests have passed\n\n");
}

void test_array_list_contains(void) {
    char *elements[9] = {"a", "b", "c", "d", "e", "f", "g", "h", "hello"};
    ArrayList* array_list = initialize_arraylist();
    for (int i = 0; i < 9; i++) {
        add_to_arraylist(array_list, strdup(elements[i]));
    }
    assert(array_list->size == sizeof(elements) / sizeof(char*));
    for (int i = 0; i < 9; i++) {
        assert(!strcmp(array_list->array[i], elements[i]));
    }
    printf("array_list_contains tests have passed\n\n");
}

void test_hash_table_contains(void) {
    char *keys[4] = {"angus", "aboud", "malhar", "jack"};
    char *values[4] = {"alpha", "beta", "gamma", "delta"};

    HashTable hash_table = initialise_hash_table();
    for (int i = 0; i < 4; i++) {
        insert(hash_table, strdup(keys[i]), strdup(values[i]));
    }
    for (int i = 0; i < 4; i++) {
        assert(!strcmp(values[i], get(hash_table, keys[i])));
    }
    
    printf("hash_table_contains tests have passed\n\n");
}

void test_record_comparator(void) {
    ArrayList* record1 = initialize_arraylist();
    add_to_arraylist(record1, strdup("a"));
    add_to_arraylist(record1, strdup("10"));
    add_to_arraylist(record1, strdup("z"));

    ArrayList* record2 = initialize_arraylist();
    add_to_arraylist(record2, strdup("b"));
    add_to_arraylist(record2, strdup("10"));
    add_to_arraylist(record2, "a");

    assert(record_comparator(record1, record2, (int[]) {1}, 1) < 0);
    assert(record_comparator(record1, record2, (int[]) {2}, 1) == 0);
    assert(record_comparator(record1, record2, (int[]) {2, 3}, 2) > 0);
    assert(record_comparator(record1, record2, (int[]) {1, 2, 3}, 3) < 0);

    printf("record_comparator tests have passed\n\n");
}

void test_bubble_sort(void) {
    ArrayList* record1 = initialize_arraylist();
    add_to_arraylist(record1, "d");
    ArrayList* record2 = initialize_arraylist();
    add_to_arraylist(record2, "b");
    ArrayList* record3 = initialize_arraylist();
    add_to_arraylist(record3, "c");
    ArrayList* record4 = initialize_arraylist();
    add_to_arraylist(record4, "a");
    ArrayList* record5 = initialize_arraylist();
    add_to_arraylist(record5, "b");

    ArrayList* sorted_list[5] = {record1, record2, record3, record4, record5};
    bubbleSort((void**) sorted_list, 5, (int[]) {1}, 1);
    for (int i = 0; i < 4; i++) {
        assert(record_comparator(sorted_list[i], sorted_list[i+1], (int[]) {1}, 1) <= 0);
    }

    printf("bubble_sort test 1 passed\n");

    bubbleSort((void**) sorted_list, 5, (int[]) {-1}, 1);
    for (int i = 0; i < 4; i++) {
        assert(record_comparator(sorted_list[i], sorted_list[i+1], (int[]) {1}, 1) >= 0);
    }

    printf("bubble_sort test 2 passed\n");

    printf("bubble_sort tests have passed\n\n");
}

int main(void) {
    printf("Starting tests...\n");
    test_evaluate_ast();
    test_value_in_arr();
    test_make_array_copy();
    test_make_record_copy();
    test_array_list_contains();
    test_hash_table_contains();
    test_record_comparator();
    test_bubble_sort();

    printf("\n################\n");
    printf("All tests passed\n");
    printf("################\n");
}