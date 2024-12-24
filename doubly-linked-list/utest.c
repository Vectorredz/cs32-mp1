/*****************************************************/
/*                                                   */
/*         | ======== | UNIT TEST | ======== |       */
/*                                                   */
/*****************************************************/

#include "dynamic_list.h"
#include "main.c"
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

int64_t *make_seq(int size, int64_t *array){
    for (int i = 0; i < size; i++){
        array[i] = i;
    }
    return array;
}

int main(){
    // Test case #1 : Operation Correctness
    int t1 = 1;
    printf("\n | ************* | TEST CASE #%d | ************* |\n\n", t1);

    list l1 = init();
    VERIFY(empty(&l1));
    // push_? operations
    push_right(&l1, 1);
    push_right(&l1, 2);
    push_right(&l1, 3);
    push_left(&l1, 4);
    push_left(&l1, 5);
    push_left(&l1, 6);
    push_right(&l1, 7);
    push_left(&l1, 8);
    display(&l1);
    VERIFY(size(&l1) == 8);

    // pop_? operations
    pop_left(&l1);  
    pop_left(&l1);
    pop_right(&l1);
    pop_right(&l1);
    display(&l1);
    VERIFY(size(&l1) == 4);

    // peek_? operation
    VERIFY(peek_left(&l1) == 5);
    VERIFY(peek_right(&l1) == 2);
    pop_left(&l1);
    pop_right(&l1);  
    VERIFY(peek_left(&l1) == 4);
    VERIFY(peek_right(&l1) == 1);  

    // size & empty operations
    VERIFY(size(&l1) == 2);
    pop_left(&l1);
    pop_right(&l1);  
    VERIFY(size(&l1) == 0);
    VERIFY(empty(&l1));

    // make operation
    int n = 6;
    int64_t *array = malloc(n * sizeof(int64_t));
    int64_t *seq1 = make_seq(n, array);
    list *l2 = make(n, seq1);
    display(l2);

    // SETTERS & GETTERS operation
    VERIFY(get(l2, 1) == 1);
    VERIFY(get(l2, 5) == 5);
    
    set(l2, 0, 9);
    VERIFY(peek_left(l2) == 9);
    set(l2, n-1, 0);
    VERIFY(peek_right(l2) == 1);

    printf("\nVERIFIED: TEST CASE #%d PASSED.", t1);
    

    // Test case #3 : Out-of-bounds Check
    // Test case #4 : Edge-cases

    //TODO ...
    
}