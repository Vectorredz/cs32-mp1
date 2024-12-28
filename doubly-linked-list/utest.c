/*****************************************************/
/*                                                   */
/*         | ======== | UNIT TEST | ======== |       */
/*                                                   */
/*****************************************************/

#include "d_linked_list.h"
#include "d_linked_list.c"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define VERIFY(b) do {\
    bool _b = (b);\
    fprintf(stderr, "VERIFYING: %s\n", (#b));\
    if (!_b) {\
        fprintf(stderr, "\nVERIFICATION FAILED!\n");\
        exit(1);\
        }\
} while (0)

DATA *make_seq(LENGTH size, DATA *array){
    for (LENGTH i = 0; i < size; i++){
        array[i] = i;
    }
    return array;
}

LENGTH main(){
    // Test case #1 : Operation Correctness
    LENGTH t1 = 1;
    printf("\n | ************* | TEST CASE #%d | ************* |\n\n", t1);

    list *l1 = init();
    VERIFY(empty(l1));
    // push_? operations
    push_right(l1, 1);
    push_right(l1, 2);
    push_right(l1, 3);
    push_left(l1, 4);
    push_left(l1, 5);
    push_left(l1, 6);
    push_right(l1, 7);
    push_left(l1, 8);
    display(l1);
    VERIFY(size(l1) == 8);

    // pop_? operations
    pop_left(l1);  
    pop_left(l1);
    pop_right(l1);
    pop_right(l1);
    display(l1);
    VERIFY(size(l1) == 4);

    // peek_? operation
    VERIFY(peek_left(l1) == 5);
    VERIFY(peek_right(l1) == 2);
    pop_left(l1);
    pop_right(l1);  
    VERIFY(peek_left(l1) == 4);
    VERIFY(peek_right(l1) == 1);  

    // size  empty operations
    VERIFY(size(l1) == 2);
    pop_left(l1);
    pop_right(l1);  
    VERIFY(size(l1) == 0);
    VERIFY(empty(l1));

    // make operation
    LENGTH n1 = 6;
    DATA *array = malloc(n1 * sizeof(DATA));
    DATA *seq1 = make_seq(n1, array);
    list *l2 = make(n1, seq1);
    display(l2);

    // SETTERS  GETTERS operation
    VERIFY(get(l2, 1) == 1);
    VERIFY(get(l2, 5) == 5);
    
    set(l2, 0, 9);
    VERIFY(peek_left(l2) == 9);
    set(l2, n1-1, 0);
    VERIFY(peek_right(l2) == 0);

    // reverse operation
    // once called, it would toggle reverse operation

    // toggle on reverse hence pop_left <-> pop_right
    reverse(l2);
    pop_left(l2);
    display(l2);
    pop_right(l2);
    display(l2);

    // toggle off reverse hence pop_left <-> pop_right
    reverse(l2);
    pop_left(l2);
    display(l2);
    pop_right(l2);
    display(l2);

    // GETTERS
    VERIFY(get(l2, 1) == 3);
    reverse(l2);
    VERIFY(get(l2, 1) == 2);

    // SETTERS
    // reverse is toggled ON so traversal will be from tail -> head
    set(l2, 0, 1);
    VERIFY(peek_left(l2) == 1);
    // reverse is toggled OFF so traversal will be from head -> tail
    reverse(l2);
    
    set(l2, 0, 3);
    VERIFY(peek_left(l2) == 3);


    printf("\nVERIFIED: TEST CASE #%d PASSED.\n", t1);

    // Test case #2 : Out-of-bounds Check
    /* Aims to check the bounds for each operation. This includes "index out of range", "invalid value passed", etc. */

    LENGTH t2 = 2;
    printf("\n | ************* | TEST CASE #%d | ************* |\n\n", t2);

    // make operation
    LENGTH n2 = 1;
    DATA *array1 = malloc(n2 * sizeof(DATA));
    DATA *seq2 = make_seq(n2, array);
    list *l3 = make(n2, seq1);
    display(l3);

    // pop_? operations

    // 1. popping a single node
    VERIFY(pop_left(l3));

    // 2. popping an empty list 
    // this must throw an exception: "IndexError: Pop From Empty List"
    VERIFY(pop_left(l3) == false);

    // peek_? operations

    // 1. peeking an empty list
    // this must throw an exception: "IndexError: Peek From Empty List"
    VERIFY(peek_left(l3));
    VERIFY(peek_right(l3));

    // SETTERS and GETTERS

    // 1. getting an element from an empty list
    // this must throw an exception: "IndexError: list index out of range"
    VERIFY(get(l3, 2));

    // 2. setting an element from an empty list
    // this must throw an exception: "IndexError: list index out of range"
    set(l3, 2, 1);
}