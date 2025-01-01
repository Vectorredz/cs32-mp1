#ifndef TEST_SETTINGS_H
#define TEST_SETTINGS_H

/*
    << Water Moon. >>
    Look into the Mirror...
*/
// --------------------------------------------------------- >>

// Modify both header and typedef to the list to test
#include "../tree-sequence/p_tree_list.c"
typedef PTreeList Reflection;

// Set which implementation to test. Choose among the following:
/*
    DOUBLY_LINKED_LIST
    DYNAMIC_ARRAY
    SKIP_LIST
    TREE_SEQUENCE
 */
#define IMPLEMENTATION "TREE_SEQUENCE"
// TYPE: boolean
// Set to true if you would like the current line being executed to be displayed
// This is useful for segfaults where the tester abruptly stops and the faulty line is unknown
#define LINE_DISPLAY false

// TYPE: boolean
// Set to true if you want to check for efficiency (TLE)
// NOTE: Timer only works for Windows as of now, please disable for other OS
#define CHECK_FOR_EFFICIENCY true

// TYPE: double
// TLE efficiency boundary (in milliseconds)
#define TLE_BOUND 1000.0

// Inputs File Path (contains operation, args, and correct result for cross-checks)
// ((test_input_0.txt)) is for testing the tester itself
#define INPUT_DIRECTORY "inputs"



#endif