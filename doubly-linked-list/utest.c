/*****************************************************/
/*                                                   */
/*          ======== | UNIT TEST | ========          */
/*                                                   */
/*****************************************************/

#include "dynamic_list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define VERIFY(b) do {\
    bool _b = (b);\
    fprintf(stderr, "verifying: %s\n", (#b));\
    if (!_b) {\
        fprintf(stderr, "verification failed!\n");\
        exit(1);\
        }\
} while (0)

int main(){
    // Test case #1 : Operation Correctness
    

    // Test case #2 : Out-of-bounds Check
    // Test case #3 : Edge-cases

    //TODO ...

}