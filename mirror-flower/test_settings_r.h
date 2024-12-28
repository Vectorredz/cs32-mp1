#ifndef TEST_SETTINGS_H
#define TEST_SETTINGS_H

/*
    << Water Moon. >>
    Look into the Mirror...
*/
// --------------------------------------------------------- >>

// Modify both header and typedef to the list to test
#include "../doubly-linked-list/d_linked_list.c" //(yes this is a Sin :man_running:)
typedef list Reflection;

// TYPE: boolean
// Set to true if you would like the current line being executed to be displayed
// This is useful for segfaults where the tester abruptly stops and the faulty line is unknown
#define LINE_DISPLAY false

// TYPE: double
// TLE efficiency boundary (in milliseconds)
#define TLE_BOUND 1000.0

// Input File Path (contains operation, args, and correct result for cross-checks)
// ((test_input_0.csv)) is for testing the tester itself
#define INPUT_FILE "test_input.csv"

// Output File Path (contains operation, current size (n), and deltatime benchmark for graph plotting)
#define OUTPUT_FILE "test_output.csv"



#endif