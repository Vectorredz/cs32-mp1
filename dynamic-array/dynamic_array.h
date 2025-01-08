#include "../H_global.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct dynamic_array 
{
    DATA *array; // array
    LENGTH last; // last valid element's index
    LENGTH start; // first valid element's index 
    LENGTH size; // number of valid elements inside the array
    LENGTH capacity; // capacity of the array 
    bool reverse; // used to make O(1) reversal of list
} dynamic_array;


// made for simpler implementation
void expand_array(dynamic_array *d);
void decrease_array(dynamic_array *d);

dynamic_array *make(LENGTH n, DATA *seq);
void push_left(dynamic_array *d, DATA v);
void push_right(dynamic_array *d, DATA v);
bool pop_left(dynamic_array *d);
bool pop_right(dynamic_array *d);
DATA peek_left(dynamic_array *d); 
DATA peek_right(dynamic_array *d); 
LENGTH size(dynamic_array *d); 
bool empty(dynamic_array *d); 
DATA get(dynamic_array *d, LENGTH i); 
void set(dynamic_array *d, LENGTH i, DATA v); 
void reverse(dynamic_array *d); 
