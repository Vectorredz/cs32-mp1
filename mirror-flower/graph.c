#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <windows.h>
#include <sys/timeb.h>
#include "../H_global.h"

// Implementation selection
#if IMPLEMENTATION == DOUBLY_LINKED_lIST
    #include "../doubly-linked-list/d-linked-list.c"
    typedef list Reflection;
#elif IMPLEMENTATION == DYNAMIC_ARRAY
    #include "../dynamic-array/dynamic-array.c"
    typedef dynamic_array Reflection;
#elif IMPLEMENTATION == SKIP_LIST
    #include "../skip-list/skip-list.c"
    typedef SkipList Reflection;
#elif IMPLEMENTATION == TREE_SEQUENCE
    #include "../tree-sequence/p_tree_list.c"
    typedef PTreeList Reflection;
#endif

// Timer definitions
typedef LARGE_INTEGER RECORDED_TIME;
typedef double PROCESSED_TIME;
uint64_t freq;

// Timer initialization
void _TIME_init() {
    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    freq = (uint64_t)((uint64_t)(f.HighPart) << 32) | (uint32_t)f.LowPart;
}

void _TIME(RECORDED_TIME* cRef) {
    QueryPerformanceCounter(cRef);
}

PROCESSED_TIME _PROCESSTIME(RECORDED_TIME a, RECORDED_TIME b) {
    uint64_t bQ = (uint64_t)((uint64_t)(b.HighPart) << 32) | (uint32_t)b.LowPart;
    uint64_t aQ = (uint64_t)((uint64_t)(a.HighPart) << 32) | (uint32_t)a.LowPart;
    uint64_t dt = (bQ - aQ);
    PROCESSED_TIME final = (double)(dt) / (double)freq;
    return final < 0.0L ? 0.0L : final;
}

// Data structure for writing results
typedef struct _WriteData {
    char* operation;
    LENGTH n;
    PROCESSED_TIME c;
} WriteData;

WriteData NEW_WRITE(char* operation, LENGTH n, PROCESSED_TIME c) {
    WriteData writeData = *((WriteData*)malloc(sizeof(WriteData)));
    writeData.operation = operation;
    writeData.n = n;
    writeData.c = c;
    return writeData;
}

void EXPORT_DELTA_TIME(FILE** output, WriteData wd, bool newLine) {
    FILE* target = NULL;
    
    if (strcmp(wd.operation, "make") == 0) target = output[0];
    else if (strcmp(wd.operation, "push_left") == 0) target = output[1];
    else if (strcmp(wd.operation, "push_right") == 0) target = output[2];
    else if (strcmp(wd.operation, "pop_left") == 0) target = output[3];
    else if (strcmp(wd.operation, "pop_right") == 0) target = output[4];
    else if (strcmp(wd.operation, "peek_left") == 0) target = output[5];
    else if (strcmp(wd.operation, "peek_right") == 0) target = output[6];
    else if (strcmp(wd.operation, "set") == 0) target = output[7];
    else if (strcmp(wd.operation, "get") == 0) target = output[8];
    else if (strcmp(wd.operation, "reverse") == 0) target = output[9];
    else if (strcmp(wd.operation, "empty") == 0) target = output[10];
    else if (strcmp(wd.operation, "size") == 0) target = output[11];

    if (target) {
        fprintf(target, "%zu | %.9lf", wd.n, wd.c);
        if (newLine) fprintf(target, "\n");
    }
}

// Operation execution and timing
void OPERATION(FILE** output, Reflection* list, char* operation) {
    RECORDED_TIME _rec, rec;
    LENGTH n = size(list);
    
    // Warmup iterations
    for (int i = 0; i < 100; i++) {
        if (strcmp(operation, "size") == 0) size(list);
        else if (strcmp(operation, "empty") == 0) empty(list);
        else if (strcmp(operation, "reverse") == 0) reverse(list);
        else if (strcmp(operation, "get") == 0 && n > 0) get(list, (LENGTH)rand() % n);
        else if (strcmp(operation, "set") == 0 && n > 0) set(list, (LENGTH)rand() % n, rand());
        else if (strcmp(operation, "peek_left") == 0 && n > 0) peek_left(list);
        else if (strcmp(operation, "peek_right") == 0 && n > 0) peek_right(list);
        else if (strcmp(operation, "push_left") == 0) push_left(list, rand());
        else if (strcmp(operation, "push_right") == 0) push_right(list, rand());
        else if (strcmp(operation, "pop_left") == 0 && n > 0) pop_left(list);
        else if (strcmp(operation, "pop_right") == 0 && n > 0) pop_right(list);
    }

    // Actual measurement
    _TIME(&_rec);
    if (strcmp(operation, "size") == 0) {
        size(list);
    } else if (strcmp(operation, "empty") == 0) {
        empty(list);
    } else if (strcmp(operation, "reverse") == 0) {
        reverse(list);
    } else if (strcmp(operation, "get") == 0) {
        if (n == 0) return;
        get(list, (LENGTH)rand() % n);
    } else if (strcmp(operation, "set") == 0) {
        if (n == 0) return;
        set(list, (LENGTH)rand() % n, rand());
    } else if (strcmp(operation, "peek_left") == 0) {
        if (n == 0) return;
        peek_left(list);
    } else if (strcmp(operation, "peek_right") == 0) {
        if (n == 0) return;
        peek_right(list);
    } else if (strcmp(operation, "push_left") == 0) {
        push_left(list, rand());
    } else if (strcmp(operation, "push_right") == 0) {
        push_right(list, rand());
    } else if (strcmp(operation, "pop_left") == 0) {
        if (n == 0) return;
        pop_left(list);
    } else if (strcmp(operation, "pop_right") == 0) {
        if (n == 0) return;
        pop_right(list);
    }
    _TIME(&rec);
    
    EXPORT_DELTA_TIME(output, NEW_WRITE(operation, n, _PROCESSTIME(_rec, rec)), true);
}

int main() {
    printf("<< Grapher >>\n");
    printf("> Initializing variables...\n");

    // Write implementation type
    FILE* implementation = fopen("../mirror-flower/outputs/implementation.txt", "w+");
    if (IMPLEMENTATION == DOUBLY_LINKED_LIST) fprintf(implementation, "DOUBLY_LINKED_LIST");
    else if (IMPLEMENTATION == DYNAMIC_ARRAY) fprintf(implementation, "DYNAMIC_ARRAY");
    else if (IMPLEMENTATION == SKIP_LIST) fprintf(implementation, "SKIP_LIST");
    else if (IMPLEMENTATION == TREE_SEQUENCE) fprintf(implementation, "TREE_SEQUENCE");
    fclose(implementation);

    // Initialize timer and random seed
    _TIME_init();
    srand(time(NULL));

    // Initialize output files
    FILE** output = malloc(12 * sizeof(FILE));
    output[0] = fopen("../mirror-flower/outputs/make.txt", "w+");
    output[1] = fopen("../mirror-flower/outputs/push_l.txt", "w+");
    output[2] = fopen("../mirror-flower/outputs/push_r.txt", "w+");
    output[3] = fopen("../mirror-flower/outputs/pop_l.txt", "w+");
    output[4] = fopen("../mirror-flower/outputs/pop_r.txt", "w+");
    output[5] = fopen("../mirror-flower/outputs/peek_l.txt", "w+");
    output[6] = fopen("../mirror-flower/outputs/peek_r.txt", "w+");
    output[7] = fopen("../mirror-flower/outputs/set.txt", "w+");
    output[8] = fopen("../mirror-flower/outputs/get.txt", "w+");
    output[9] = fopen("../mirror-flower/outputs/reverse.txt", "w+");
    output[10] = fopen("../mirror-flower/outputs/empty.txt", "w+");
    output[11] = fopen("../mirror-flower/outputs/size.txt", "w+");

    printf("> Done.\n");
    printf("> Generating plot points...\n");

    // Test creation with different sizes
    for (LENGTH n = 0; n < 50000; n += 100) {
        DATA* seq = (DATA*)malloc(n * sizeof(DATA));
        for (LENGTH i = 0; i < n; i++) {
            seq[i] = rand();
        }
        RECORDED_TIME _rec, rec;
        _TIME(&_rec);
        Reflection* list = make(n, seq);
        _TIME(&rec);
        EXPORT_DELTA_TIME(output, NEW_WRITE("make", n, _PROCESSTIME(_rec, rec)), true);
        free(seq);
        // Clean up list based on implementation
        // Add appropriate cleanup here
    }

    // Initialize empty list for operations testing
    Reflection* list = make(0, NULL);

    // Test operations with increasing sizes
    LENGTH upper = 20000;
    for (int i = 0; i < upper; i++) {
        char* operations[] = {
            "push_left", "push_right", "pop_left", "pop_right",
            "peek_left", "peek_right", "get", "set",
            "size", "empty", "reverse"
        };
        
        for (int j = 0; j < 11; j++) {
            OPERATION(output, list, operations[j]);
        }
    }

    // Cleanup
    for (int i = 0; i < 12; i++) {
        fclose(output[i]);
    }
    free(output);

    printf("> Done.\n");
    return 0;
}