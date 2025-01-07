#include "dynamic_array.h"

// helper functions
void _expandArray(dynamic_array *list)
{
    DATA *temp_array = (DATA*)malloc((list->size) * sizeof(DATA));

        // copying the array properly into a temporary one for easier copying
        for (LENGTH i=0; i<(list->elements); i++)
        {
            // if the starting element is == size, it starts back at 0
            // takes into account if list->start > 0
            // this part also increments the start by 1 all in one operation
            temp_array[i] = list->array[(list->start + i) % (list->size)];
        }

        free(list->array);
        list->array = (DATA*)malloc((2*(list->size)) * sizeof(DATA));
        
        // fixing the index situation to start at 0 again
        for (LENGTH i=0; i<(list->elements); i++)
        {
            list->array[i] = temp_array[i];
        }

        // refreshing the variables
        list->start = 0;

        if(list->elements==1) list->last = 0;
        else list->last = list->elements-1;

        list->size = 2*(list->size); 

        free(temp_array);
}

void _decreaseArray(dynamic_array *list)
{
    if (list->elements == 0)   
    {
        // makes the size 1
        free(list->array);
        list->array = (DATA*)malloc(sizeof(DATA));
        list->start = 0;
        list->last = 0;
        list->size = 1;
        list->elements = 0;
        list->reverse = false;
    }

    else
    {
        DATA *temp_array = (DATA*)malloc((list->elements) * sizeof(DATA));

        // copying the array properly into a temporary one for easier copying
        for (LENGTH i=0; i<(list->elements); i++)
        {
            // if the starting element is == size, it starts back at 0
            // takes into account if list->start > 0
            // this part also increments the start by 1 all in one operation
            temp_array[i] = list->array[(list->start + i) % (list->size)];
        }

        free(list->array);
        list->array = (DATA*)malloc(((list->size)/2) * sizeof(DATA));
        for (LENGTH i=0; i<list->elements; i++)
        {
            list->array[i] = temp_array[i];
        }

        // refreshing the variables
        list->start = 0;
        if(list->elements == 1 || list->elements == 0) list->last = 0;
        else list->last = list->elements-1;
        list->size = (list->size)/2;

        free(temp_array);
    }
}

dynamic_array *make(LENGTH n, DATA *seq)
{
    // undefined behavior
    if (n<0) return NULL;

    dynamic_array *list = (dynamic_array *)malloc(sizeof(dynamic_array));

    // if given seq is empty, create an array with size 1
    if (n==0)
    {
        // creates an empty array of size 20 (the critical point for expanding and sparsing)
        list->array = (DATA*)malloc((1) * sizeof(DATA)); // arbitrary number
        list->start = 0;
        list->last = 0;
        list->size = 1;
        list->elements = 0;
        list->reverse = false;
    }

    else
    {
        // dynamically allocating the array to be of size 2n 
        list->array = (DATA*)malloc((2*n) * sizeof(DATA));
        list->reverse = false;

        // setting the size of the array to 2n and keeping the current amount of elements in the list
        list->size = 2*n;
        list->elements = n; 

        // setting the first and last index
        list->last = n-1;
        list->start = 0;

        // copying the given sequence into the dynamic array
        for (LENGTH i=0; i<n; i++)
        {   
            // setting the values of each index
            list->array[i] = seq[i];
        }
    }

    return list;
}

void push_left(dynamic_array *list, DATA v)
{
    if (!list->reverse)
    {
        // special case if empty list
        if (list->elements == 0)
        {
            list->array[list->start] = v;

            list->elements++;

            _expandArray(list);
        }

        else
        {
            // makes the last index of the array the first
            if (list->start == 0) list->start = (list->size)-1;
            else list->start--; // takes into account if list->start > 0 

            // appends the new value
            list->array[list->start] = v;

            // increase to the size
            list->elements++;

            // if number of elements exceeds the size, it expands
            if(list->elements == list->size) _expandArray(list);
        }
    }

    else // list->reverse == true
    {
        // important so it will do the push_right operation
        list->reverse = !list->reverse;
        push_right(list, v);
        list->reverse = !list->reverse; // undo the first reverse
    }
}

void push_right(dynamic_array *list, DATA v)
{
    if (!list->reverse)
    {
        if (list->elements == 0)
        {
            // adding the new element into an empty list
            list->array[list->last] = v;
            
            // increasing the variable that keeps track of how many elements there are in the list
            list->elements++;

            _expandArray(list);
        }

        else
        {
            
            // case when the array isn't full but the index exceeds the size, it means there's no index at 0
            // this part also increments by 1  in one operation 
            list->last = (list->last+1) % list->size;

            if(list->last<0) list->last = list->size-1;
        
            //setting the variable and increasing the number of valid elements
            list->array[list->last] = v;
            list->elements++;

            // if number of elements exceeds the size, it expands
            if(list->elements == list->size) _expandArray(list);
        }
    }

    else // list->reverse == true
    {   
        // important so it will do the push_left operation
        list->reverse = !list->reverse;
        push_left(list, v); 
        list->reverse = !list->reverse; // undo the first reverse
    }
}

bool pop_left(dynamic_array *list)
{
    // return false if empty
    if (list->elements == 0 ) return false;

    if(list->reverse == false)
    {
        // we aren't actually popping the element, we just change the index range
        list->elements--;
        list->start++;

        // if the start index is not 0
        if(list->start >= list->size) list->start = 0;

        if((list->elements) <= (list->size)/4) _decreaseArray(list); 

        return true;
    }

    else // list->reverse == true
    {
        // important so it will do the push_right operation
        list->reverse = !list->reverse;
        pop_right(list);
        list->reverse = !list->reverse; // undo the first reverse
    }
}

bool pop_right(dynamic_array *list)
{
    // return false if empty
    if (list->elements == 0 ) return false;

    if (list->reverse == false)
    {
        // we aren't actually popping the element, we just change the index range
        list->elements--;


        if ((list->last) == 0) list->last = (list->size)-1;
        else list->last--;
        

        if((list->elements) <= (list->size)/4) _decreaseArray(list); 
    
        return true;
    }

    else // list->reverse == true
    {
        // important so it will do the push_right operation
        list->reverse = !list->reverse;
        pop_left(list);
        list->reverse = !list->reverse; // undo the first reverse
    }
}

DATA peek_left(dynamic_array *list)
{
    //returns leftmost element, return rightmost if reversed
    if (list->reverse) return list->array[list->last];
    return list->array[list->start];
}

DATA peek_right(dynamic_array *list)
{
    // returns rightmost element, return leftmost if reversed 
    if (list->reverse) return list->array[list->start];
    return list->array[list->last];
}

LENGTH size(dynamic_array *list) 
{
    // returns size
    return list->elements;
}

bool empty(dynamic_array *list)
{
    // if size=0 then it is empty, otherwise its not empty
    if ((list->elements) == 0) return true;
    return false;
}

DATA get(dynamic_array *list, LENGTH i)
{   
    // add indexing error
    if (!(0 <= i && i < list->elements))
    {
        fprintf(stderr, "IndexError: list index out of range\n");
        exit(1);
    }

    // the actual getting algorithm
    if (list->reverse == false)
    {
        // returns the i-th element in terms of list->start (fixing the indexing)
        LENGTH new_i = (list->start + i) % list->size;

        return list->array[new_i];
    } 

    else // list->reverse == true
    {
        // reverses the index 
        LENGTH reverse_index = (list->start + (list->elements -1 -i)) % list->size;
        
        return list->array[reverse_index];
    }
}

void set(dynamic_array *list, LENGTH i, DATA v)
{
    // add indexing error
    if (!(0 <= i && i < list->elements))
    {
        fprintf(stderr, "IndexError: list index out of range\n");\
        exit(1);\
    }

    if (list->reverse == false)
    {
        // returns the i-th element in terms of list->start (fixing the indexing)
        // includes circular indexing
        LENGTH new_i = (list->start + i) % list-> size;
    

        // sets the i-th element as v
        list->array[new_i] = v;
    }

    else // list->reverse == true
    {

        // reverses the index 
        LENGTH reverse_index = (list->start + (list->elements -1 -i)) % list->size;
        list->array[reverse_index] = v;
    }   
}

void reverse(dynamic_array *list)
{
    // sets the bool into the opposite of what it currently is
    list->reverse = !list->reverse;
}



// TEST FUNCTIONS

void TEST_elements(dynamic_array* list, LENGTH* nRef, DATA** seqRef){
    DATA* seq = (DATA*) malloc((list->elements)*sizeof(DATA));
    
    // insert elements from left to right into seq here

    for (LENGTH i=0; i<list->elements; i++)
    {
        LENGTH new_i = (list->start + i) % list->size;
        seq[i] = list->array[new_i];
    }

    *nRef = list->elements;
    *seqRef = seq;
}

bool TEST_internal(dynamic_array* list){
    // put own tests here, return false if it failed to satisfy
    return true;
}
bool TEST_reversed(dynamic_array* list){
    // make it return if list is reversed here
    return list->reverse;
}
