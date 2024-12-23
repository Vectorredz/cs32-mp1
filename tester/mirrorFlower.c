/*
<< Water Moon. >>

INSTRUCTIONS:
There is a SETTINGS section right below this comment.
By default it tests the list Mirror against itself, however you can change it:
1. Replace #include to include the file of the list you want to test
    - Example: #include "skip-list.c"
2. Replace the typedef for Flower with the List's type
    - Example: typedef SkipList Flower;

*/

/* SETTINGS */
#include "mirror.c"
typedef Mirror Flower;


/*
//////////////////////////////////////////////////////////////////////////////////////////////
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/


#include "mirror.c"
#include "stdlib.h"
#include "stdio.h"


typedef struct _BenchmarkRecord {
    double make;

    double size;
    double empty;
    double reverse;

    double get;
    double set;
    double peek_left;
    double peek_right;
    
    double push_left;
    double push_right;
    double pop_left;
    double pop_right;
      
} BenchmarkRecord;



int main(){
    
}