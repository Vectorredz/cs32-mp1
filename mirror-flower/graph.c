/*
    << Grapher >>
*/
// --------------------------------------------------------- >>
/* ----------------------------------------- <<
            ||-- OUTPUT --||
<< ----------------------------------------- */
/*
- After the tests, the tester will output the plot points in OUTPUT_DIRECTORY.
*/
#include "test_settings_g.h"
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>
// Do not edit past this point!
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>


// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- MISCELLANEOUS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>

#include "time.h"

// TIMER
// TODO: implement other os later
#include <windows.h>
#include <sys/timeb.h>
typedef LARGE_INTEGER RECORDED_TIME;
typedef double PROCESSED_TIME;
#define TIME_FORMAT "lf"
uint64_t freq;
void _TIME_init(){
    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    freq = (uint64_t) ((uint64_t)(f.HighPart) << 32) | (uint32_t) f.LowPart;
}
void _TIME(RECORDED_TIME* cRef){
    QueryPerformanceCounter(cRef);
}
PROCESSED_TIME _PROCESSTIME(RECORDED_TIME a, RECORDED_TIME b){
    uint64_t bQ = (uint64_t) ((uint64_t)(b.HighPart) << 32) | (uint32_t) b.LowPart;
    uint64_t aQ = (uint64_t) ((uint64_t)(a.HighPart) << 32) | (uint32_t) a.LowPart;
    uint64_t dt = (bQ-aQ);
    PROCESSED_TIME final = (double)(dt)/(double)freq;
    return final < 0.0L ? 0.0L : final;
}

// Used for time plots output
typedef struct _WriteData {
    char* operation;
    LENGTH n;
    PROCESSED_TIME c;
} WriteData;



// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- HELPERS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>

WriteData NEW_WRITE(char* operation, LENGTH n, PROCESSED_TIME c){
    WriteData writeData = *((WriteData*) malloc(sizeof(WriteData)));
    writeData.operation = operation;
    writeData.n = n;
    writeData.c = c;

    return writeData;
}

void EXPORT_DELTA_TIME(FILE **output, WriteData wd, bool newLine){

    if (strcmp(wd.operation, "make") == 0){
        fprintf(output[0], "%zu | %lf", wd.n, wd.c);
        if (newLine) fprintf(output[0], "\n");
    }
    if (strcmp(wd.operation, "push_left") == 0){
        fprintf(output[1], "%zu | %lf", wd.n, wd.c);
        if (newLine) fprintf(output[1], "\n");
    }
    if (strcmp(wd.operation, "push_right") == 0){
        fprintf(output[2], "%zu | %lf", wd.n, wd.c);
        if (newLine) fprintf(output[2], "\n");
    }
    if (strcmp(wd.operation, "pop_left") == 0){
        fprintf(output[3], "%zu | %lf", wd.n, wd.c);
        if (newLine) fprintf(output[3], "\n");
    }
    if (strcmp(wd.operation, "pop_right") == 0){
        fprintf(output[4], "%zu | %lf", wd.n, wd.c);
        if (newLine) fprintf(output[4], "\n");
    }
    if (strcmp(wd.operation, "peek_left") == 0){
        fprintf(output[5], "%zu | %lf", wd.n, wd.c);
        if (newLine) fprintf(output[5], "\n");
    }
    if (strcmp(wd.operation, "peek_right") == 0){
        fprintf(output[6], "%zu | %lf", wd.n, wd.c);
        if (newLine) fprintf(output[6], "\n");
    }
    if (strcmp(wd.operation, "set") == 0){
        fprintf(output[7], "%zu | %lf", wd.n, wd.c);
        if (newLine) fprintf(output[7], "\n");
    }
    if (strcmp(wd.operation, "get") == 0){
        fprintf(output[8], "%zu | %lf", wd.n, wd.c);
        if (newLine) fprintf(output[8], "\n");
    }
    if (strcmp(wd.operation, "reverse") == 0){
        fprintf(output[9], "%zu | %lf", wd.n, wd.c);
        if (newLine) fprintf(output[9], "\n");
    }
    if (strcmp(wd.operation, "empty") == 0){
        fprintf(output[10], "%zu | %lf", wd.n, wd.c);
        if (newLine) fprintf(output[10], "\n");
    }
    if (strcmp(wd.operation, "size") == 0){
        fprintf(output[11], "%zu | %lf", wd.n, wd.c);
    }

}

void OPERATION(FILE** output, Reflection* list, char* operation){
    RECORDED_TIME _rec, rec;
    LENGTH n = size(list);
    if (strcmp(operation, "size") == 0){
        _TIME(&_rec);
        size(list);
        _TIME(&rec);
    } else if (strcmp(operation, "empty") == 0){
        _TIME(&_rec);
        empty(list);
        _TIME(&rec);
    } else if (strcmp(operation, "reverse") == 0){
        _TIME(&_rec);
        reverse(list);
        _TIME(&rec);
    } else if (strcmp(operation, "get") == 0){
        if (n == 0) return;
        _TIME(&_rec);
        get(list, (LENGTH) rand() % n);
        _TIME(&rec);
    } else if (strcmp(operation, "set") == 0){
        if (n == 0) return;
        _TIME(&_rec);
        set(list, (LENGTH) rand() % n, rand());
        _TIME(&rec);
    } else if (strcmp(operation, "peek_left") == 0){
        if (n == 0) return;
        _TIME(&_rec);
        peek_left(list);
        _TIME(&rec);
    } else if (strcmp(operation, "peek_right") == 0){
        if (n == 0) return;
        _TIME(&_rec);
        peek_right(list);
        _TIME(&rec);
    } else if (strcmp(operation, "push_left") == 0){
        _TIME(&_rec);
        push_left(list, rand());
        _TIME(&rec);
    } else if (strcmp(operation, "push_right") == 0){
        _TIME(&_rec);
        push_right(list, rand());
        _TIME(&rec);
    } else if (strcmp(operation, "pop_left") == 0){
        if (n == 0) return;
        _TIME(&_rec);
        pop_left(list);
        _TIME(&rec);
    } else if (strcmp(operation, "pop_right") == 0){
        if (n == 0) return;
        _TIME(&_rec);
        pop_right(list);
        _TIME(&rec);
    }
    EXPORT_DELTA_TIME(output, NEW_WRITE(operation, n, _PROCESSTIME(_rec, rec)), false);
}

int main(){
    printf("<< Grapher >>\n");

    // Initialize others
    printf("> Initializing variables...\n");

    // Timer and randomizer
    _TIME_init();
    srand(time(NULL));

    // Where the output values are stored
    const char *outputs = "../mirror-flower/outputs/";
    FILE **output = malloc(12 * sizeof(FILE));
    FILE *_make = fopen("../mirror-flower/outputs/make.txt", "w+");
    FILE *_peek_l = fopen("../mirror-flower/outputs/peek_l.txt", "w+");
    FILE *_peek_r = fopen("../mirror-flower/outputs/peek_r.txt", "w+");
    FILE *_push_l = fopen("../mirror-flower/outputs/push_l.txt", "w+");
    FILE *_push_r = fopen("../mirror-flower/outputs/push_r.txt", "w+");
    FILE *_set = fopen("../mirror-flower/outputs/set.txt", "w+");
    FILE *_get = fopen("../mirror-flower/outputs/get.txt", "w+");
    FILE *_empty = fopen("../mirror-flower/outputs/empty.txt", "w+");
    FILE *_size = fopen("../mirror-flower/outputs/size.txt", "w+");
    FILE *_reverse = fopen("../mirror-flower/outputs/reverse.txt", "w+");
    FILE *_pop_r = fopen("../mirror-flower/outputs/pop_r.txt", "w+");
    FILE *_pop_l = fopen("../mirror-flower/outputs/pop_l.txt", "w+");
    output[0] = _make;
    output[1] = _push_l;
    output[2] = _push_r;
    output[3] = _pop_l;

    output[4] = _pop_r;
    output[5] = _peek_l;
    output[6] = _peek_r;
    output[7] = _set;

    output[8] = _get;
    output[9] = _reverse;
    output[10] = _empty;
    output[11] = _size;

    printf("> Done.\n");


    // Main Tester
    printf("> Generating plot points...\n");
    Reflection* list;

    // Make Test
    for (int n = 0; n < 10001; n++){
        DATA* seq = (DATA*) malloc(n*sizeof(DATA));
        for (int i = 0; i < n; i++){
            seq[i] = (LENGTH) rand();
        }
        RECORDED_TIME _rec, rec;
        _TIME(&_rec);
        list = make(n, seq);
        _TIME(&rec);
        EXPORT_DELTA_TIME(output, NEW_WRITE("make", n, _PROCESSTIME(_rec, rec)), false);
    }
    list = make(0, NULL);

    // Insertion/Deletion Test
    LENGTH upper = 20000;
    for (int i = 0; i < upper; i++){
        OPERATION(output, list, "push_left");
        OPERATION(output, list, "peek_left");
        OPERATION(output, list, "peek_right");
        OPERATION(output, list, "get");
        OPERATION(output, list, "set");
        OPERATION(output, list, "size");
        OPERATION(output, list, "empty");
        OPERATION(output, list, "reverse");
    }
    while (size(list) > 0){
        OPERATION(output, list, "pop_left");
        OPERATION(output, list, "peek_left");
        OPERATION(output, list, "peek_right");
        OPERATION(output, list, "get");
        OPERATION(output, list, "set");
        OPERATION(output, list, "size");
        OPERATION(output, list, "empty");
        OPERATION(output, list, "reverse");
    }
    for (int i = 0; i < upper; i++){
        OPERATION(output, list, "push_right");
        OPERATION(output, list, "peek_left");
        OPERATION(output, list, "peek_right");
        OPERATION(output, list, "get");
        OPERATION(output, list, "set");
        OPERATION(output, list, "size");
        OPERATION(output, list, "empty");
        OPERATION(output, list, "reverse");
    }
    while (size(list) > 0){
        OPERATION(output, list, "pop_right");
        OPERATION(output, list, "peek_left");
        OPERATION(output, list, "peek_right");
        OPERATION(output, list, "get");
        OPERATION(output, list, "set");
        OPERATION(output, list, "size");
        OPERATION(output, list, "empty");
        OPERATION(output, list, "reverse");
    }

    upper = (rand() % 1000) + 1500;
    for (int i = 0; i < upper; i++){
        OPERATION(output, list, "push_right");
    }

    // Randomizer Test
    upper = 20000;
    char* operations[11] = {"size", "empty", "reverse", "get", "set", "peek_left", "peek_right", "push_left", "push_right", "pop_left", "pop_right"};
    for (int i = 0; i < upper; i++){
        OPERATION(output, list, operations[rand() % 11]);
    }

    printf("> Done.\n");
}