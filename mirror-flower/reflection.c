/*
    << Water Moon. >>
    Will your Reflection be the same as mine?
*/
// --------------------------------------------------------- >>
/* ----------------------------------------- <<
            #||-- OUTPUT --||#
<< ----------------------------------------- */
/*
- After the test, the tester will output OUTPUT_FILE
- OUTPUT_FILE is a .txt file composed of Fields (OPERATION | N | DELTATIME)
It is used to graph N (current size when OPERATION was done) against DELTATIME (in milliseconds) to judge whether the graph of OPERATION is
constant, linear, or logarithmic in nature.
*/
#include "test_settings_r.h"
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>
// Do not edit past this point!
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>

#include "../H_global.h"
#include <stdio.h>
#include <string.h>

#define MAX_NUMBER_DIGITS 25



// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- MISCELLANEOUS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>


// TIMER for outputting execution time plots
// TODO: implement other os later


#if TIME_COMPLEXITY_GRAPH_DISPLAY == true
    #define DISPLAY true
    #define STATUS "enabled"
#else
    #define DISPLAY false
    #define STATUS "disabled"
#endif



#if CHECK_FOR_EFFICIENCY == true
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
#else
    typedef int RECORDED_TIME;
    typedef int PROCESSED_TIME;
    #define TIME_FORMAT "d"
    void _TIME_init(){

    }
    void _TIME(RECORDED_TIME* cRef){
        *cRef = 0;
    }
    PROCESSED_TIME _PROCESSTIME(RECORDED_TIME a, RECORDED_TIME b){
        return 0;
    }
#endif


// Used for time plots output
typedef struct _WriteData {
    char* operation;
    LENGTH n;
    PROCESSED_TIME c;
} WriteData;

// Used for tests

typedef struct _TestData {
    char* path;
    size_t totalTests;
    char*** tests;
} TestData;


// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- HELPERS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>

// Gets tests and stores in an array.
TestData getTests(char* path){
    FILE *f = fopen(path, "r");
    size_t lines = 0;
    bool newline = true;

    // temporary buffer for getting each character one at a time
    char* testBuffer = (char*) malloc(2*sizeof(char));

    // get the number of lines first, then go back to beginning of file
    while (true){
        while (fgets(testBuffer, 2, f) != NULL){
            if (testBuffer[0] == '\n'){
                newline = true;
            }
            if (newline == true){
                newline = false;
                lines++;
            }
        }
        if (ferror(f)){
            fprintf(stderr, "Test get error");
            exit(1);
        } else {
            break;
        }
    }
    rewind(f);


    char*** tests = (char***) malloc(lines*sizeof(char**));
    size_t i = 0;
    while (true){
        long seekBack = ftell(f);
        size_t length = 0;
        bool nlSkip = false;
        bool eof = false;

        // get the length line first, then go back to beginning of line
        while (fgets(testBuffer, 2, f) != NULL){
            if (testBuffer[0] == '\n'){
                nlSkip = true;
                break;
            }
            length++;
        }
        if (ferror(f)){
            fprintf(stderr, "Test get error");
            exit(1);
        } else if (!nlSkip){
            eof = true;
        }
        fseek(f, seekBack, SEEK_SET);


        // split the line by "|" delimeter, and store in an array of strings of length 4 {OPERATION, ARG1, ARG2, RESULT}
        char* buffer = (char*) malloc((length+1)*sizeof(char));
        fgets(buffer, length+1, f);
        
        char** testLine = (char**) malloc(4*sizeof(char*));
        char* token = strtok(buffer, "|");
        size_t j = 0;
        while (token != NULL){
            char* testInfo = (char*) malloc((strlen(token)+1)*sizeof(char));
            strcpy(testInfo, token);
            testLine[j] = testInfo;
            j++;
            token = strtok(NULL, "|");
        }
        tests[i] = testLine;
        i++;

        if (eof == true){
            break;
        }
        if (nlSkip == true){
            fgets(testBuffer, 2, f); // fseek +2 for '\n' is weird so lets just do this lol
        }
        free(buffer);
    }
    free(testBuffer);
    fclose(f);

    char* pathCopy = (char*) malloc((strlen(path) + 1) * sizeof(char));
    strcpy(pathCopy, path);
    TestData testData = *((TestData*) malloc(sizeof(TestData)));
    testData.path = pathCopy;
    testData.totalTests = lines;
    testData.tests = tests;
    return testData;
}

char* listToResult(Reflection* list, bool includeReversal){
    // Turns a sequence of DATA values into a string delimeted by ","
    LENGTH n;
    DATA* seq;
    TEST_elements(list, &n, &seq);
    if (n == 0){
        // dynamically allocate result for free() consistency
        char* mRESULT = (char*) malloc(6*sizeof(char));
        mRESULT[0] = 'E';
        mRESULT[1] = 'M';
        mRESULT[2] = 'P';
        mRESULT[3] = 'T';
        mRESULT[4] = 'Y';
        mRESULT[5] = '\0';
        return mRESULT;
    }

    LENGTH numberOfCommas = n-1;
    LENGTH m = numberOfCommas+(n*MAX_NUMBER_DIGITS)+1;
    char* mRESULT = (char*) malloc(m*sizeof(char));

    LENGTH m0 = 0;
    for (LENGTH i = 0; i < n; i++){
        LENGTH trueIndex = i;
        if (includeReversal == true){
            trueIndex = TEST_reversed(list) == false ? i : n-1-i;
        }
        DATA data = seq[trueIndex];
        m0 += sprintf(&mRESULT[m0], "%" PRId64, data);
        if (i < n-1){
            m0 += sprintf(&mRESULT[m0], ",");
        }
    }
    return mRESULT;
}

LENGTH strToLength(char* lStr){
    LENGTH l;
    sscanf(lStr, "%llu", &l);
    return l;
}
DATA strToData(char* dataStr){
    DATA data;
    sscanf(dataStr, "%" SCNd64, &data);
    return data;
}
char* lengthToStr(LENGTH l){
    char* lengthStr = (char*) malloc((MAX_NUMBER_DIGITS+1)*sizeof(char));
    sprintf(lengthStr, "%zu", l);
    return lengthStr;
}
char* dataToStr(DATA data){
    char* dataStr = (char*) malloc((MAX_NUMBER_DIGITS+1)*sizeof(char));;
    sprintf(dataStr, "%" PRId64, data);
    return dataStr;
}
char* boolToStr(bool b){
    char* boolStr = (char*) malloc(2*sizeof(char));
    boolStr[0] = b == false ? '0' : '1';
    boolStr[1] = '\0';
    return boolStr;
}

// TODO: use function pointers instead for getting mRESULT? idk
void DISPLAY_LOGS(Reflection* list, char* operation, char* extraOperation){
    if (extraOperation == NULL){
        fprintf(stderr, ":: operation: %s\n", operation);
    } else {
        fprintf(stderr, ":: operation: %s -> %s\n", operation, extraOperation);
    }
    fprintf(stderr, ":: List is reversed? -> %s\n", TEST_reversed(list) == false ? "false" : "true");
    fprintf(stderr, ":: RAW SEQUENCE (reverse is ignored) -> %s\n------\n", listToResult(list, false));
    fprintf(stderr, ":: RAW SEQUENCE (with reverse) -> %s\n", listToResult(list, true));
}
void VERIFY(Reflection* list, char* path, size_t testNum, char* operation, char* RESULT, char* mRESULT, double dt, char* extraOperation, bool checkForCorrectness, bool checkForEfficiency){
    if (checkForCorrectness == true){
        if (strcmp(mRESULT, RESULT) != 0){
            fprintf(stderr, "(( %s ))\n", path);
            fprintf(stderr, "[/] [line %zu]: WA [%lfms]\n", testNum, dt);
            fprintf(stderr, "!! Failed Operation !!\n");
            
            size_t i = 0;
            while (RESULT[i] == mRESULT[i] && RESULT[i] != '\0' && mRESULT[i] != '\0'){
                i++;
            }
            DISPLAY_LOGS(list, operation, extraOperation);
            fprintf(stderr, ":: line %zu\n:: column %zu (char CORRECT: \"%c\", FAULT: \"%c\")\n", testNum, i, RESULT[i], mRESULT[i]);
            fprintf(stderr, ":: FAULTY OUTPUT -> %s\n", mRESULT);
            fprintf(stderr, ":: SUPPOSED OUTPUT -> %s\n", RESULT);
            exit(1);
        }
    }
    if (checkForEfficiency == true){
        if (dt > TLE_BOUND){
            fprintf(stderr, "[+] [line %zu]: TLE [%lfms (> %lf ms)]\n", testNum, dt, TLE_BOUND);
            DISPLAY_LOGS(list, operation, extraOperation);
            exit(1);
        }
    }
}


// Used for time plots output
void WRITE(FILE* f, WriteData wd, bool newLine){
    fprintf(f, "%s|%zu|%" TIME_FORMAT, wd.operation, wd.n, wd.c);

    if (newLine == true){
        fprintf(f, "\n");
    }
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
        if (newLine) fprintf(output[11], "\n");
    }

}


// --------------------------------------------------------- >>
/* ----------------------------------------- << 

            ||-- MAIN TESTER --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>
int main(){

    // Write which implementation is currently used
    FILE *implementation = fopen("../mirror-flower/outputs/implementation.txt", "w+");
    if (strcmp(IMPLEMENTATION, "DOUBLY_LINKED_LIST") == 0){
        fprintf(implementation, "DOUBLY_LINKED_LIST");
    }
    if (strcmp(IMPLEMENTATION, "DYNAMIC_ARRAY") == 0){
        fprintf(implementation, "DYNAMIC_ARRAY");
    }
    if (strcmp(IMPLEMENTATION, "SKIP_LIST") == 0){
        fprintf(implementation, "SKIP_LIST");
    }
    if (strcmp(IMPLEMENTATION, "TREE_SEQUENCE") == 0){
        fprintf(implementation, "TREE_SEQUENCE");
    }


    printf("<< Water Moon. >>\nWill your Reflection be the same as mine?\n");

    // Get tests first for each file
    printf("> Getting tests for (( " INPUT_DIRECTORY " )) ...\n");
    size_t totalOperations = 0; // number of actual operations (since a line can contain the custom message which doesn't count as an operation)
    
    size_t totalFiles;
    char** files;
    
    if (LARGE_INPUTS == false){
        totalFiles = 6;
        files = (char**) malloc(totalFiles*sizeof(char*));
        files[0] = "LAYER0.txt";
        files[1] = "LAYER1.txt";
        files[2] = "LAYER2.txt";
        files[3] = "LAYER3.txt";
        files[4] = "LAYER4.txt";
        files[5] = "LAYER5.txt";
    } else {
        totalFiles = 1;
        files = (char**) malloc(totalFiles*sizeof(char*));
        files[0] = "LAYERSPECIAL.txt";
    }

    TestData* fileTests = (TestData*) malloc(totalFiles*sizeof(TestData)); // Tests array for each file
    for (size_t fileNum = 0; fileNum < totalFiles; fileNum++){
        // Get tests
        char* path = (char*) malloc((strlen(INPUT_DIRECTORY) + strlen("/") + strlen(files[fileNum]) + 1) * sizeof(char));
        strcpy(path, INPUT_DIRECTORY);
        strcat(path, "/");
        strcat(path, files[fileNum]);

        printf("> (( %s )) ...\n", path);

        TestData testData = getTests(path);
        fileTests[fileNum] = testData;

        for (size_t testNum = 0; testNum < testData.totalTests; testNum++){
            if (strcmp((testData.tests)[testNum][0], "MSG") != 0){
                totalOperations++;
            }
        }

        printf("> acquired -> (( %s ))\n", path);

        free(path);
    }
    free(files);
    printf("> Done.\n");
    
    
    // Initialize others
    printf("> Initializing variables...\n");

    // timer
    _TIME_init();

    // where the output values are stored
    WriteData* writeDataLines = (WriteData*) malloc(totalOperations*sizeof(WriteData));
    double *deltaTime = malloc(totalOperations * sizeof(double));

    FILE* f = fopen(OUTPUT_FILE, "w+");
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
    printf("> Conducting test operations...\n");
    Reflection* list = NULL;

    size_t opCounter = 0;
    bool prevWasMsg = false;

    RECORDED_TIME _timeGlobal, timeGlobal;
    _TIME(&_timeGlobal);

    for (size_t fileNum = 0; fileNum < totalFiles; fileNum++){
        TestData testData = fileTests[fileNum];
        char* path = testData.path;
        size_t totalTests = testData.totalTests;
        char*** tests = testData.tests;
        for (size_t testNum = 0; testNum < totalTests; testNum++){

            // Get each one first
            char** testLine = tests[testNum];
            char* operation = testLine[0];
            char* arg1 = testLine[1];
            char* arg2 = testLine[2];
            char* RESULT = testLine[3];
            char* mRESULT;
            char* extraOperation;
            bool checkForCorrectness = strcmp("X", RESULT) != 0;
            

            // Clear past 2 lines of output and display new executing line
            if (LINE_DISPLAY == true){
                if (opCounter > 0 && prevWasMsg == false){
                    printf("\033[A\033[K\033[A\033[K");
                }
            }

            // Custom LAYER messages
            if (strcmp(operation, "MSG") == 0){
                prevWasMsg = true;
                if (strcmp(arg1, "LAYER") == 0){
                    printf("!! LAYER %s !!\n", arg2);

                } else if (strcmp(arg1, "LAYERFIN") == 0){
                    printf("!! LAYER %s Passed. !!\n", arg2);

                }
                continue;
            }

            // Display new executing line
            if (LINE_DISPLAY == true){
                printf("exec %zu ...\n", testNum+1);
            }
            prevWasMsg = false;
            
            LENGTH n = list != NULL ? size(list) : 0;
            RECORDED_TIME _time, time;

            // Get appropriate variables for ARG1/ARG2, execute the desired operation while timing it, and (if checking for correctness) verify the output
            if (strcmp(operation, "make") == 0){
                // Get n and seq args for make() function (seq is delimeted by ",")
                n = strToLength(arg1);
                DATA* seq = (DATA*) malloc(n*sizeof(DATA));
                if (n > 0){
                    char* token = strtok(arg2, ",");
                    size_t i = 0;
                    while (token != NULL){
                        seq[i] = strToData(token);
                        i++;
                        token = strtok(NULL, ",");
                    }
                }

                _TIME(&_time);
                list = make(n, seq);
                _TIME(&time);

                extraOperation = "TEST_elements";
                if (checkForCorrectness == true){
                    mRESULT = listToResult(list, true);
                }

            } else if (strcmp(operation, "size") == 0){
                _TIME(&_time);
                LENGTH listSize = size(list);
                _TIME(&time);

                extraOperation = NULL;
                if (checkForCorrectness == true){
                    mRESULT = lengthToStr(listSize);
                }

            } else if (strcmp(operation, "empty") == 0){
                _TIME(&_time);
                bool listEmpty = empty(list);
                _TIME(&time);

                extraOperation = NULL;
                if (checkForCorrectness == true){
                    mRESULT = boolToStr(listEmpty);
                }

            } else if (strcmp(operation, "reverse") == 0){
                _TIME(&_time);
                reverse(list);
                _TIME(&time);

                extraOperation = "TEST_elements";
                if (checkForCorrectness == true){
                    mRESULT = listToResult(list, true);
                }

            } else if (strcmp(operation, "get") == 0){
                LENGTH i = strToLength(arg1);

                _TIME(&_time);
                DATA data = get(list, i);
                _TIME(&time);

                extraOperation = NULL;
                if (checkForCorrectness == true){
                    mRESULT = dataToStr(data);
                }

            } else if (strcmp(operation, "set") == 0){
                LENGTH i = strToLength(arg1);
                DATA v = strToData(arg2);

                _TIME(&_time);
                set(list, i, v);
                _TIME(&time);

                extraOperation = "TEST_elements";
                if (checkForCorrectness == true){
                    LENGTH n;
                    DATA* seq;
                    TEST_elements(list, &n, &seq);
                    mRESULT = dataToStr(seq[TEST_reversed(list) == false ? i : n-1-i]);
                }

            } else if (strcmp(operation, "peek_left") == 0){
                _TIME(&_time);
                DATA data = peek_left(list);
                _TIME(&time);

                extraOperation = NULL;
                if (checkForCorrectness == true){
                    mRESULT = dataToStr(data);
                }

            } else if (strcmp(operation, "peek_right") == 0){
                _TIME(&_time);
                DATA data = peek_right(list);
                _TIME(&time);

                extraOperation = NULL;
                if (checkForCorrectness == true){
                    mRESULT = dataToStr(data);
                }

            } else if (strcmp(operation, "push_left") == 0){
                DATA v = strToData(arg1);

                _TIME(&_time);
                push_left(list, v);
                _TIME(&time);

                extraOperation = "TEST_elements";
                if (checkForCorrectness == true){
                    mRESULT = listToResult(list, true);
                }

            } else if (strcmp(operation, "push_right") == 0){
                DATA v = strToData(arg1);

                _TIME(&_time);
                push_right(list, v);
                _TIME(&time);

                extraOperation = "TEST_elements";
                if (checkForCorrectness == true){
                    mRESULT = listToResult(list, true);
                }

            } else if (strcmp(operation, "pop_left") == 0){
                _TIME(&_time);
                pop_left(list);
                _TIME(&time);
                
                extraOperation = "TEST_elements";
                if (checkForCorrectness == true){
                    mRESULT = listToResult(list, true);
                }

            } else if (strcmp(operation, "pop_right") == 0){
                _TIME(&_time);
                pop_right(list);
                _TIME(&time);
                
                extraOperation = "TEST_elements";
                if (checkForCorrectness == true){
                    mRESULT = listToResult(list, true);
                }

            }

            PROCESSED_TIME dt = _PROCESSTIME(_time, time) * 1000.0;

            // For internal tests
            if (checkForCorrectness == true){
                VERIFY(list, path, testNum+1, operation, "success", TEST_internal(list) == true ? "success" : "fail", dt, "TEST_internal", checkForCorrectness, false);
            }

            // Verify the output
            VERIFY(list, path, testNum+1, operation, RESULT, mRESULT, dt, extraOperation, checkForCorrectness, true);

            if (LINE_DISPLAY == true){
                printf("[O] [line %zu]: AC [%lfms]\n", testNum+1, dt);
            }

            // Finally, write the new benchmark to output array
            char* opCopy = (char*) malloc((strlen(operation)+1)*sizeof(char));
            strcpy(opCopy, operation);
            
            WriteData wd = *((WriteData*) malloc(sizeof(WriteData)));
            wd.operation = opCopy;
            wd.n = n;
            wd.c = dt;
            writeDataLines[opCounter] = wd;
            opCounter++;
            free(mRESULT);
            free(testLine[0]);
            free(testLine[1]);
            free(testLine[2]);
            free(testLine[3]);
            free(testLine);
        }
        
        free(testData.path);
        free(testData.tests);
    }


    _TIME(&timeGlobal);
    if (LINE_DISPLAY == true){
        if (opCounter > 0 && prevWasMsg == false){
            printf("\033[A\033[K\033[A\033[K");
        }
    }
    printf("[O] [GLOBAL]: AC [%lfs]\n", _PROCESSTIME(_timeGlobal, timeGlobal));
    printf("> Well Done.\n");


    // Write all benchmarks to the file
    printf("> Writing deltatime benchmarks to output...\n");
    for (size_t opNum = 0; opNum < totalOperations; opNum++){
        WRITE(f, writeDataLines[opNum], opNum < totalOperations-1 ? true : false);
        EXPORT_DELTA_TIME(output, writeDataLines[opNum], opNum < totalOperations-1 ? true : false);
        free(writeDataLines[opNum].operation);
    }
    printf("> Done.\n");
    

    printf("> Cleanup...\n");
    free(fileTests);
    free(writeDataLines);
    fclose(f);
    printf("> Done.\n\n");
    
    printf("---------------------------------------------------------\n");
    printf("<<<< TESTING SUCCESSFUL >>>>\n");
    printf("Time plot points <%s>\n\n", STATUS);
    if (DISPLAY == true){
        printf("> Opening graphs.py ...\n");
        Sleep(2000);
        printf("> graph.py successfully opened.\n\n");
        Sleep(2000);
        popen("python graph.py", "r");
    }
    printf("? Ah...\n? Despite everything, it's still you...\n...A mere Reflection.\n\n");

    
    
    printf("Look into the Mirror once more, will you?\n\n");
}
