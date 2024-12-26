#include "dynamic_array.h"

// helper functions
void expand_array(dynamic_array *d)
{
    int64_t *temp_array = (int64_t*)malloc((d->size) * sizeof(int64_t));

        // copying the array properly into a temporary one for easier copying
        for (int i=0; i<(d->elements); i++)
        {
            temp_array[i] = d->array[(d->start)++];

            // if the starting element is == size, it starts back at 0
            // takes into account if d->start > 0
            if ((d->start) == (d->size)) d->start = 0;
        }

        d->array = (int64_t*)realloc(d->array, 2*(d->size));
        
        // fixing the index situation to start at 0 again
        for (int i=0; i<(d->elements); i++)
        {
            d->array[i] = temp_array[i];
        }

        // refreshing the variables
        d->start = 0;
        d->last = (d->elements)-1;
        d->size *= 2; // size = size*2

        free(temp_array);
}

void decrease_array(dynamic_array *d)
{
    if (d->elements == 0)
    {
        // makes the size 1
        d->array = (int64_t*)realloc(d->array, 1);
        d->start = 0;
        d->last = 0;
        d->size = 1;
        d->elements = 0;
        d->reverse = false;
    }

    else
    {
        int64_t *temp_array = (int64_t*)malloc((d->size) * sizeof(int64_t));

        // copying the array properly into a temporary one for easier copying
        for (int i=0; i<(d->elements); i++)
        {
            temp_array[i] = d->array[(d->start)++];

            // if the starting element is == size, it starts back at 0
            // takes into account if d->start > 0
            if ((d->start) == (d->size)) d->start = 0;
        }


        d->array = (int64_t*)realloc(d->array, floor((d->size)/2));
        for (int i=0; i<d->elements; i++)
        {
            d->array[i] = temp_array[i];
        }

        // refreshing the variables
        d->start = 0;
        d->last = (d->elements)-1;
        d->size = floor((d->size)/2);

        free(temp_array);
    }
}

dynamic_array *make(int n, int64_t *seq)
{
    // undefined behavior
    if (n<0) return NULL;

    dynamic_array *d = (dynamic_array *)malloc(sizeof(d));

    // if given seq is empty, create an array with size 1
    if (n==0)
    {
        // creates an empty array of size 20 (the critical point for expanding and sparsing)
        d->array = (int64_t*)malloc((1) * sizeof(int64_t)); // arbitrary number
        d->start = 0;
        d->last = 0;
        d->size = 1;
        d->elements = 0;
        d->reverse = false;
    }

    else
    {
        // dynamically allocating the array to be of size 2n 
        d->array = (int64_t*)malloc((2*n) * sizeof(int64_t));
        d->reverse = false;

        // setting the size of the array to 2n and keeping the current amount of elements in the list
        d->size = 2*n;
        d->elements = n; 

        // setting the first and last index
        d->last = n-1;
        d->start = 0;

        // copying the given sequence into the dynamic array
        for (int i=0; i<n; i++)
        {   
            // setting the values of each index
            d->array[i] = seq[i];
        }
    }

    return d;
}

void push_left(dynamic_array *d, int64_t v)
{
    if (d->reverse == false)
    {
        // special case if empty list
        if (d->elements == 0)
        {
            d->array[d->start] = v;

            d->elements++;
        }

        else
        {
            // makes the last index of the array the first
            if (d->start == 0) d->start = (d->size)-1;
            else d->start--; // takes into account if d->start > 0 

            // pushing of the value and saving the "leftmost"
            d->array[d->start] = v;

            // increase to the size
            d->elements++;
        }
    }

    else // d->reverse == true
    {
        // important so it will do the push_right operation
        d->reverse = !d->reverse;
        push_right(d, v);
        d->reverse = !d->reverse; // undo the first reverse
    }

    // if number of elements exceeds the size, it expands
    if(d->elements == d->size) expand_array(d);
}

void push_right(dynamic_array *d, int64_t v)
{
    if (d->reverse == false)
    {
        if (d->elements == 0)
        {
            // adding the new element into an empty list
            d->array[d->start] = v;
            
            // increasing the variable that keeps track of how many elements there are in the list
            d->elements++;
        }

        else
        {
            // increment first
            d->last++;
            
            // case when the array isn't full but the index exceeds the size, it means there's no index at 0
            if(d->last >= d->size && d->elements < d->size) d->last = 0;
        
            //setting the variables as new ones
            d->array[d->last] = v;
            d->elements++;
        }
    }

    else // d->reverse == true
    {   
        // important so it will do the push_left operation
        d->reverse = !d->reverse;
        push_left(d, v); 
        d->reverse = !d->reverse; // undo the first reverse
    }

    // if number of elements exceeds the size, it expands
    if(d->elements == d->size) expand_array(d);
}

bool pop_left(dynamic_array *d)
{
    if(d->reverse == false)
    {
        // return false if empty
        if (d->elements == 0 ) return false;

        // we aren't actually popping the element, we just change the index range
        d->elements--;
        d->start++;

        // if the start index is not 0
        if(d->start >= d->size) d->start = 0;

        if(floor((d->elements)) <= floor((d->size)/4)) decrease_array(d); 

        return true;
    }

    else // d->reverse == true
    {
        // important so it will do the push_right operation
        d->reverse = !d->reverse;
        pop_right(d);
        d->reverse = !d->reverse; // undo the first reverse
    }
}

bool pop_right(dynamic_array *d)
{
    if (d->reverse == false)
    {
        // return false if empty
        if (d->elements == 0 ) return false;

        // we aren't actually popping the element, we just change the index range
        d->elements--;
        d->last--;

        // if last index is >= 0
        if ((d->last) < 0) d->last = (d->size)-1;
        

        if(floor((d->elements)) <= floor((d->size)/4)) decrease_array(d); 
    
        return true;
    }

    else // d->reverse == true
    {
        // important so it will do the push_right operation
        d->reverse = !d->reverse;
        pop_left(d);
        d->reverse = !d->reverse; // undo the first reverse
    }
}

int64_t peek_left(dynamic_array *d)
{
    //returns leftmost element, return rightmost if reversed
    if (d->reverse) return d->array[d->last];
    return d->array[d->start];
}

int64_t peek_right(dynamic_array *d)
{
    // returns rightmost element, return leftmost if reversed 
    if (d->reverse) return d->array[d->start];
    return d->array[d->last];
}

int size(dynamic_array *d) 
{
    // returns size
    return d->elements;
}

bool empty(dynamic_array *d)
{
    // if size=0 then it is empty, otherwise its not empty
    if ((d->elements) == 0) return true;
    return false;
}

int64_t get(dynamic_array *d, int i)
{   
    // add indexing error
    if (0 > i >= d->elements)
    {
        fprintf(stderr, "IndexError: list index out of range\n");\
        exit(1);\
    }

    // the actual getting algorithm
    if (d->reverse == false)
    {
        // returns the i-th element in terms of d->start (fixing the indexing)
        int new_i = d->start + i;
        
        // if d->start > d->last due to circular indexing
        if (new_i >= d->size) new_i -= d->size;

        return d->array[new_i];
    } 

    else // d->reverse == true
    {
        // reverses the index 
        int reverse_index = abs((d->last) - i);
        
        return d->array[reverse_index];
    }
}

void set(dynamic_array *d, int i, int64_t v)
{
    // add indexing error
    if (0 > i >= d->elements)
    {
        fprintf(stderr, "IndexError: list index out of range\n");\
        exit(1);\
    }

    if (d->reverse == false)
    {
        // returns the i-th element in terms of d->start (fixing the indexing)
        int new_i = d->start + i;
        
        // if d->start > d->last due to circular indexing
        if (new_i >= d->size) new_i -= d->size;

        // sets the i-th element as v
        d->array[new_i] = v;
    }

    else // d->reverse == true
    {

        // reverses the index 
        int reverse_index = abs((d->last) - i);
        d->array[reverse_index] = v;
    }   
}

void reverse(dynamic_array *d)
{
    // sets the bool into the opposite of what it currently is
    d->reverse = !d->reverse;
}


/*

    reversing the index mini proof

    0 1 2 3 4 5

    size = 6

    index 5 is 0 on reverse, (size-1) - index => 5 - 5 = 0
    index 4 is 1 on reverse, (size-1) - index => 5 - 4 = 1
    index 3 is 2 on reverse, (size-1) - index => 5 - 3 = 2
    index 2 is 3 on reverse, (size-1) - index => 5 - 2 = 3
    index 1 is 4 on reverse, (size-1) - index => 5 - 1 = 4
    index 0 is 5 on reverse, (size-1) - index => 5 - 0 = 5

    ////

    0 
    
    size = 1

    index 0 is 0 on reverse, (size-1) - index => 0 - 0 = 0


    special indexing due to circular reasons:

    0 1 2 3 start(4) 5
    4 + (i=3) (wants the 4th element or index 3 in terms of 0 indexing)
    7 - size = 7 - 6 = 1;

    0:4, 1:5, 2:0, 3:1
    1 is the 4th element or theoretically at index 3


    start(5) 6 0 1 2 3 last(4)
    4 - (i=5) = -1
    2 - (size=7) = 1
*/
