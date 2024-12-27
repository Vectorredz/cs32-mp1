#include "../H_global.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct dynamic_array 
{
    int64_t *array; // array
    int last; // last valid element's index
    int start; // first valid element's index 
    int elements; // number of valid elements (n)
    int size; // size of the array (2n) or upper bound of the array
    bool reverse; // used to make O(1) reversal of list
} dynamic_array;


// made for simpler implementation
void expand_array(dynamic_array *d);
void decrease_array(dynamic_array *d);

dynamic_array *make(int n, int64_t *seq);
void push_left(dynamic_array *d, int64_t v);
void push_right(dynamic_array *d, int64_t v);
bool pop_left(dynamic_array *d);
bool pop_right(dynamic_array *d);
int64_t peek_left(dynamic_array *d); 
int64_t peek_right(dynamic_array *d); 
int size(dynamic_array *d); 
bool empty(dynamic_array *d); 
int64_t get(dynamic_array *d, int i); 
void set(dynamic_array *d, int i, int64_t v); 
void reverse(dynamic_array *d); 
