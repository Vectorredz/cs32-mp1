#include "dynamic_array.h"
#include <assert.h>

// helper functions
void _expand_array(dynamic_array *list)
{
    DATA *temp_array = (DATA*)malloc((list->capacity) * sizeof(DATA));

        // copying the array properly into a temporary one for easier copying
        for (LENGTH i = 0; i < (list->size); i++)
        {
            // if the starting element is == size, it starts back at 0
            // takes into account if list->start > 0
            // this part also increments the start by 1 all in one operation
            temp_array[i] = list->array[(list->start + i) % (list->capacity)];
        }

        // delete the current array
        free(list->array);

        // recreate the resized array
        list->array = (DATA*)malloc((2 * (list->capacity)) * sizeof(DATA));
        
        // fixing the index situation to start at 0 again
        for (LENGTH i = 0; i < (list->size); i++)
        {
            list->array[i] = temp_array[i];
        }

        // refreshing the variables
        list->start = 0;

        if(list->size == 1) list->last = 0;
        else list->last = list->size - 1;

        list->capacity = 2 * (list->capacity); 

        free(temp_array);
}

void _decrease_array(dynamic_array *list)
{
    if (list->size == 0)   
    {
        // makes the size 1
        free(list->array);
        list->array = (DATA*)malloc(sizeof(DATA));
        list->start = 0;
        list->last = 0;
        list->capacity = 1;
        list->size = 0;
        list->reverse = false;
    }

    else
    {
        DATA *temp_array = (DATA*)malloc((list->size) * sizeof(DATA));

        // copying the array properly into a temporary one for easier copying
        for (LENGTH i = 0; i < (list->size); i++)
        {
            // if the starting element is == size, it starts back at 0
            // takes into account if list->start > 0
            // this part also increments the start by 1 all in one operation
            temp_array[i] = list->array[(list->start + i) % (list->capacity)];
        }


        // delete the current array
        free(list->array);

        // recreate the resized the array
        list->array = (DATA*)malloc(((list->capacity) / 2) * sizeof(DATA));

        for (LENGTH i = 0; i < list->size; i++)
        {
            list->array[i] = temp_array[i];
        }

        // refreshing the variables
        list->start = 0;

        if(list->size == 1 || list->size == 0) list->last = 0;
        else list->last = list->size - 1;

        list->capacity = (list->capacity)/2;

        free(temp_array);
    }
}

dynamic_array *make(LENGTH n, DATA *seq)
{
    // undefined behavior
    if (n < 0) return NULL;
 
    dynamic_array *list = (dynamic_array *)malloc(sizeof(dynamic_array));

    // if given seq is empty, create an array with size 1
    if (n == 0)
    {
        // creates an empty array of size 20 (the critical point for expanding and sparsing)
        list->array = (DATA*)malloc((1) * sizeof(DATA)); // arbitrary number
        list->start = 0;
        list->last = 0;
        list->capacity = 1;
        list->size = 0;
        list->reverse = false;
    }

    else
    {
        // dynamically allocating the array to be of size 2n 
        list->array = (DATA*)malloc((2 * n) * sizeof(DATA));
        list->reverse = false;

        // setting the size of the array to 2n and keeping the current amount of elements in the list
        list->capacity = 2 * n;
        list->size = n; 

        // setting the first and last index
        list->last = n - 1;
        list->start = 0;

        // copying the given sequence into the dynamic array
        for (LENGTH i = 0; i < n; i++)
        {   
            // setting the values of each index
            list->array[i] = seq[i];
        }
    }

    // makes sure that the list is in between these parameters
    assert(list->capacity / 4 <= list->size && list->size <= list->capacity);
    return list;
}

void push_left(dynamic_array *list, DATA v)
{
    if (!list->reverse)
    {
        // special case if empty list
        if (list->size == 0)
        {
            list->size++;
            list->array[list->start] = v;
            
            // always expands since elements = 1 and size = 1 when it comes from an empty list
            _expand_array(list);
        }

        else
        {
            // if number of elements after the push is equal to the size, it expands before pushing
            if(list->size + 1 == list->capacity) _expand_array(list);

            // makes the last index of the array the first
            if (list->start == 0) list->start = (list->capacity) - 1;
            else list->start--; // takes into account if list->start > 0 

            // increase to the size
            list->size++;

            // appends the new value
            list->array[list->start] = v;
        }
    }

    else // list->reverse == true
    {
        // important so it will do the push_right operation
        list->reverse = !list->reverse;
        push_right(list, v);
        list->reverse = !list->reverse; // undo the first reverse
    }

    // makes sure that the list is in between these parameters
    assert(list->capacity / 4 <= list->size && list->size <= list->capacity);
}

void push_right(dynamic_array *list, DATA v)
{
    if (!list->reverse)
    {
        if (list->size == 0)
        {
            // increasing the variable that keeps track of how many elements there are in the list
            list->size++;

            // adding the new element into an empty list
            list->array[list->last] = v;

            // always expands since elements = 1 and size = 1 when it comes from an empty list
            _expand_array(list);
        }

        else
        {

            // if number of elements is equal to the size, it expands
            if(list->size + 1 == list->capacity) _expand_array(list);

            // case when the array isn't full but the index exceeds the size, it means there's no index at 0
            // this part also increments by 1  in one operation  
            list->last = (list->last + 1) % list->capacity;

            // increment size so that we can expand the array first before 
            list->size++;
        
            //setting the variable and increasing the number of valid elements
            list->array[list->last] = v;
        }
    }

    else // list->reverse == true
    {   
        // important so it will do the push_left operation
        list->reverse = !list->reverse;
        push_left(list, v); 
        list->reverse = !list->reverse; // undo the first reverse
    }

    // makes sure that the list is in between these parameters
    assert(list->capacity / 4 <= list->size && list->size <= list->capacity);
}

bool pop_left(dynamic_array *list)
{
    // return false if empty
    if (list->size == 0) return false;

    if(!list->reverse)
    {
        // we aren't actually popping the element, we just change the index range
        list->size--;
        list->start++;

        // if the start index is not 0
        if(list->start >= list->capacity) list->start = 0;

        // if the number of elements becomes too sparse (1/4 of size)
        if((list->size) <= (list->capacity) / 4) _decrease_array(list); 

        return true;
    }

    else // list->reverse == true
    {
        // important so it will do the push_right operation
        list->reverse = !list->reverse;
        pop_right(list);
        list->reverse = !list->reverse; // undo the first reverse
        
        return true;
    }

    // makes sure that the list is in between these parameters
    assert(list->capacity / 4 <= list->size && list->size <= list->capacity);
}

bool pop_right(dynamic_array *list)
{
    // return false if empty
    if (list->size == 0) return false;

    if (!list->reverse)
    {
        // we aren't actually popping the element, we just change the index range
        list->size--;


        if ((list->last) == 0) list->last = (list->capacity)-1;
        else list->last--;
        
        // if the number of elements becomes too sparse (1/4 of size)
        if((list->size) <= (list->capacity) / 4) _decrease_array(list); 
    
        return true;
    }

    else // list->reverse == true
    {
        // important so it will do the push_right operation
        list->reverse = !list->reverse;
        pop_left(list);
        list->reverse = !list->reverse; // undo the first reverse

        return true;
    }

    // makes sure that the list is in between these parameters
    assert(list->capacity / 4 <= list->size && list->size <= list->capacity);
}

DATA peek_left(dynamic_array *list)
{
    // if no elements in the array
    if (list->size == 0)
    {
        fprintf(stderr, "IndexError: list index out of range\n");
        exit(1);
    }

    //returns leftmost element, return rightmost if reversed
    if (list->reverse) return list->array[list->last];
    return list->array[list->start];
}

DATA peek_right(dynamic_array *list)
{
    // if no elements in the array
    if (list->size == 0)
    {
        fprintf(stderr, "IndexError: list index out of range\n");
        exit(1);
    }

    // returns rightmost element, return leftmost if reversed 
    if (list->reverse) return list->array[list->start];
    return list->array[list->last];
}

LENGTH size(dynamic_array *list) 
{
    // returns size
    return list->size;
}

bool empty(dynamic_array *list)
{
    // if size=0 then it is empty, otherwise its not empty
    if ((list->size) == 0) return true;
    return false;
}

DATA get(dynamic_array *list, LENGTH i)
{   
    // add indexing error
    if (!(0 <= i && i < list->size))
    {
        fprintf(stderr, "IndexError: list index out of range\n");
        exit(1);
    }

    // the actual getting algorithm
    if (!list->reverse)
    {
        // returns the i-th element in terms of list->start (fixing the indexing)
        LENGTH new_i = (list->start + i) % list->capacity;

        return list->array[new_i];
    } 

    else // list->reverse == true
    {
        // reverses the index 
        LENGTH reverse_i = (list->start + (list->size - 1 - i)) % list->capacity;
        
        return list->array[reverse_i];
    }
}

void set(dynamic_array *list, LENGTH i, DATA v)
{
    // add indexing error
    if (!(0 <= i && i < list->size))
    {
        fprintf(stderr, "IndexError: list index out of range\n");\
        exit(1);\
    }

    if (!list->reverse)
    {
        // returns the i-th element in terms of list->start (fixing the indexing)
        // includes circular indexing
        LENGTH new_i = (list->start + i) % list->capacity;
    

        // sets the i-th element as v
        list->array[new_i] = v;
    }

    else // list->reverse == true
    {

        // reverses the index 
        LENGTH reverse_i = (list->start + (list->size - 1 - i)) % list->capacity;
        list->array[reverse_i] = v;
    }   
}

void reverse(dynamic_array *list)
{
    // sets the bool into the opposite of what it currently is
    list->reverse = !list->reverse;
}




// TEST FUNCTIONS

void TEST_elements(dynamic_array* list, LENGTH* nRef, DATA** seqRef){
    DATA* seq = (DATA*) malloc((list->size) * sizeof(DATA));
    
    // insert elements from left to right into seq here

    for (LENGTH i = 0; i < list->size; i++)
    {
        LENGTH new_i = (list->start + i) % list->capacity;
        seq[i] = list->array[new_i];
    }

    *nRef = list->size;
    *seqRef = seq;
}

bool TEST_internal(dynamic_array* list){
    return true;
}
bool TEST_reversed(dynamic_array* list){
    // make it return if list is reversed here
    return list->reverse;
}
