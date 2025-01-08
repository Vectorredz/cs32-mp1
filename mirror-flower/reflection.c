/*
    << Water Moon. >>
    Will your Reflection be the same as mine?
*/
// --------------------------------------------------------- >>
/*

-----------------------------------------------
TESTER
-----------------------------------------------
- The tester will display any wrong operations and internal checks, and determine if the list passes all tests.

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
#include <stdbool.h>

#define MAX_NUMBER_DIGITS 25



/*
-----------------------------------------------
MISCELLANEOUS
-----------------------------------------------
*/
#if IMPLEMENTATION == DOUBLY_LINKED_lIST
    #include "../doubly-linked-list/d_linked_list.c"
    typedef list Reflection;
#elif IMPLEMENTATION == DYNAMIC_ARRAY
    #include "../dynamic-array/dynamic_array.c"
    typedef dynamic_array Reflection;
#elif IMPLEMENTATION == SKIP_LIST
    #include "../skip-list/skip-list.c"
    typedef SkipList Reflection;
#elif IMPLEMENTATION == TREE_SEQUENCE
    #include "../tree-sequence/p_tree_list.c"
    typedef PTreeList Reflection;
#endif

// TIMER
// TODO: implement other os later


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



/*
-----------------------------------------------
HELPERS
-----------------------------------------------
*/

// Used for tests
typedef struct _TestData {
    char* path;
    size_t totalTests;
    char*** tests;
} TestData;

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
    boolStr[0] = b == false ? 'f' : 't';
    boolStr[1] = '\0';
    return boolStr;
}

// TODO: use function pointers instead for getting mRESULT? idk
void VERIFY(Reflection* list, char* path, size_t testNum, char* operation, char* RESULT, char* mRESULT, char* extraOperation, bool checkForEfficiency, double dt){
    if (strcmp(mRESULT, RESULT) != 0){
        size_t i = 0;
        while (RESULT[i] == mRESULT[i] && RESULT[i] != '\0' && mRESULT[i] != '\0'){
            i++;
        }
        fprintf(stderr, "---------------------------------------------------------\n");
        fprintf(stderr, "---------------------------------------------------------\n");
        fprintf(stderr, "---------------------------------------------------------\n");
        fprintf(stderr, ":: RAW SEQUENCE (reverse is ignored) -> %s\n", listToResult(list, false));
        fprintf(stderr, "---------------------------------------------------------\n");
        fprintf(stderr, ":: RAW SEQUENCE (with reverse) -> %s\n", listToResult(list, true));
        fprintf(stderr, "---------------------------------------------------------\n");
        fprintf(stderr, "---------------------------------------------------------\n");

        fprintf(stderr, ":: FAULTY OUTPUT -> %s\n", mRESULT);
        fprintf(stderr, "---------------------------------------------------------\n");
        fprintf(stderr, ":: SUPPOSED OUTPUT -> %s\n", RESULT);
        fprintf(stderr, "---------------------------------------------------------\n");
        fprintf(stderr, "---------------------------------------------------------\n");

        fprintf(stderr, "(( %s ))\n", path);
        fprintf(stderr, "[/] [line %zu]: WA [%lfms]\n", testNum, dt);
        fprintf(stderr, "!! Failed Operation !!\n");
        fprintf(stderr, ":: line %zu\n:: column %zu (char FAULT: \"%c\", CORRECT: \"%c\")\n", testNum, i, mRESULT[i], RESULT[i]);
        if (extraOperation == NULL){
            fprintf(stderr, ":: operation: %s\n", operation);
        } else {
            fprintf(stderr, ":: operation: %s -> %s\n", operation, extraOperation);
        }
        fprintf(stderr, ":: List is reversed? -> %s\n", TEST_reversed(list) == false ? "false" : "true");

        fprintf(stderr, "\n? Ah, it shattered... .  .   .\n", RESULT);
        exit(1);
    }
    if (checkForEfficiency == true){
        if (dt > TLE_BOUND){
            fprintf(stderr, "---------------------------------------------------------\n");
            fprintf(stderr, "---------------------------------------------------------\n");
            fprintf(stderr, "---------------------------------------------------------\n");
            fprintf(stderr, ":: RAW SEQUENCE (reverse is ignored) -> %s\n------\n", listToResult(list, false));
            fprintf(stderr, "---------------------------------------------------------\n");
            fprintf(stderr, ":: RAW SEQUENCE (with reverse) -> %s\n", listToResult(list, true));
            fprintf(stderr, "---------------------------------------------------------\n");
            fprintf(stderr, "---------------------------------------------------------\n");

            fprintf(stderr, "(( %s ))\n", path);
            fprintf(stderr, "[+] [line %zu]: TLE [%lfms (> %lf ms)]\n", testNum, dt, TLE_BOUND);
            fprintf(stderr, "!! Too Long Operation !!\n");
            if (extraOperation == NULL){
                fprintf(stderr, ":: operation: %s\n", operation);
            } else {
                fprintf(stderr, ":: operation: %s -> %s\n", operation, extraOperation);
            }
            fprintf(stderr, ":: List is reversed? -> %s\n", TEST_reversed(list) == false ? "false" : "true");

            fprintf(stderr, "\n? Ah, it shattered... .  .   .\n", RESULT);
            exit(1);
        }
    }
}


/*
-----------------------------------------------
MAIN TESTER
-----------------------------------------------
*/
int main(){
    printf("<< Water Moon. >>\nWill your Reflection be the same as mine?\n");
    printf("> REFLECTION: List %d\n", IMPLEMENTATION);

    // Get tests first for each file
    printf("> Getting tests for (( " INPUT_DIRECTORY " )) ...\n");
    size_t totalFiles = 6;
    char** files = (char**) malloc(totalFiles*sizeof(char*));
    files[0] = "LAYER0.txt";
    files[1] = "LAYER1.txt";
    files[2] = "LAYER2.txt";
    files[3] = "LAYER3.txt";
    files[4] = "LAYER4.txt";
    files[5] = "LAYER5.txt";

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

        printf("> acquired -> (( %s ))\n", path);

        free(path);
    }
    free(files);
    printf("> Done.\n");
    
    
    // Initialize others
    printf("> Initializing variables...\n");

    // timer
    _TIME_init();

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

                } else if (strcmp(arg1, "TEST") == 0){
                    printf("%s\n", arg2);
                }
                continue;
            }

            // Display new executing line
            if (LINE_DISPLAY == true){
                printf("exec %zu ...\n", testNum+1);
            }
            prevWasMsg = false;
            
            LENGTH n = list != NULL ? size(list) : 0;
            RECORDED_TIME _rec, rec;

            // Get appropriate variables for ARG1/ARG2, execute the desired operation while timing it, and (if checking for correctness) verify the output
            if (strcmp(operation, "make") == 0){
                // Get n and seq args for make() function (seq is delimeted by ",")
                n = strToLength(arg1);
                DATA* seq;
                if (n > 0){
                    seq = (DATA*) malloc(n*sizeof(DATA));
                    
                    char* token = strtok(arg2, ",");
                    size_t i = 0;
                    while (token != NULL){
                        seq[i] = strToData(token);
                        i++;
                        token = strtok(NULL, ",");
                    }
                } else {
                    seq = NULL;
                }

                _TIME(&_rec);
                list = make(n, seq);
                _TIME(&rec);

                extraOperation = "TEST_elements";
                mRESULT = listToResult(list, true);

            } else if (strcmp(operation, "size") == 0){
                _TIME(&_rec);
                LENGTH listSize = size(list);
                _TIME(&rec);

                extraOperation = NULL;
                mRESULT = lengthToStr(listSize);

            } else if (strcmp(operation, "empty") == 0){
                _TIME(&_rec);
                bool listEmpty = empty(list);
                _TIME(&rec);

                extraOperation = NULL;
                mRESULT = boolToStr(listEmpty);

            } else if (strcmp(operation, "reverse") == 0){
                _TIME(&_rec);
                reverse(list);
                _TIME(&rec);

                extraOperation = "TEST_elements";
                mRESULT = listToResult(list, true);

            } else if (strcmp(operation, "get") == 0){
                LENGTH i = strToLength(arg1);

                _TIME(&_rec);
                DATA data = get(list, i);
                _TIME(&rec);

                extraOperation = NULL;
                mRESULT = dataToStr(data);

            } else if (strcmp(operation, "set") == 0){
                LENGTH i = strToLength(arg1);
                DATA v = strToData(arg2);

                _TIME(&_rec);
                set(list, i, v);
                _TIME(&rec);

                extraOperation = "TEST_elements";
                LENGTH n;
                DATA* seq;
                TEST_elements(list, &n, &seq);
                mRESULT = dataToStr(seq[TEST_reversed(list) == false ? i : n-1-i]);

            } else if (strcmp(operation, "peek_left") == 0){
                _TIME(&_rec);
                DATA data = peek_left(list);
                _TIME(&rec);

                extraOperation = NULL;
                mRESULT = dataToStr(data);

            } else if (strcmp(operation, "peek_right") == 0){
                _TIME(&_rec);
                DATA data = peek_right(list);
                _TIME(&rec);

                extraOperation = NULL;
                mRESULT = dataToStr(data);

            } else if (strcmp(operation, "push_left") == 0){
                DATA v = strToData(arg1);

                _TIME(&_rec);
                push_left(list, v);
                _TIME(&rec);

                extraOperation = "TEST_elements";
                mRESULT = listToResult(list, true);

            } else if (strcmp(operation, "push_right") == 0){
                DATA v = strToData(arg1);

                _TIME(&_rec);
                push_right(list, v);
                _TIME(&rec);

                extraOperation = "TEST_elements";
                mRESULT = listToResult(list, true);

            } else if (strcmp(operation, "pop_left") == 0){
                _TIME(&_rec);
                bool popped = pop_left(list);
                _TIME(&rec);
                
                extraOperation = "TEST_elements";
                
                char* b = boolToStr(popped);
                char* numSeq = listToResult(list, true);
                
                mRESULT = (char*) malloc((strlen(b) + strlen(",") + strlen(numSeq) + 1) * sizeof(char));
                strcpy(mRESULT, b);
                strcat(mRESULT, ",");
                strcat(mRESULT, numSeq);
                free(b);
                free(numSeq);

            } else if (strcmp(operation, "pop_right") == 0){
                _TIME(&_rec);
                bool popped = pop_right(list);
                _TIME(&rec);
                
                extraOperation = "TEST_elements";

                char* b = boolToStr(popped);
                char* numSeq = listToResult(list, true);

                mRESULT = (char*) malloc((strlen(b) + strlen(",") + strlen(numSeq) + 1) * sizeof(char));
                strcpy(mRESULT, b);
                strcat(mRESULT, ",");
                strcat(mRESULT, numSeq);
                free(b);
                free(numSeq);

            }

            // For internal tests
            VERIFY(list, path, testNum+1, operation, "success", TEST_internal(list) == true ? "success" : "fail", "TEST_internal", false, 0);

            // Verify the output
            PROCESSED_TIME dt;
            if (CHECK_FOR_EFFICIENCY == true){
                dt = _PROCESSTIME(_rec, rec) * 1000.0;
            } else {
                dt = 0;
            }
            VERIFY(list, path, testNum+1, operation, RESULT, mRESULT, extraOperation, CHECK_FOR_EFFICIENCY, dt);

            if (LINE_DISPLAY == true){
                printf("[O] [line %zu]: AC [%lfms]\n", testNum+1, dt);
            }

            
            free(mRESULT);
            free(testLine[0]);
            free(testLine[1]);
            free(testLine[2]);
            free(testLine[3]);
            free(testLine);

            opCounter++;
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


    // Cleanup
    printf("> Cleanup...\n");

    free(fileTests);

    printf("> Done.\n\n");
    
    printf("---------------------------------------------------------\n");
    printf("<<<< TESTING SUCCESSFUL >>>>\n");
    printf("? Ah...\n? Despite everything, it's still you...\n...A mere Reflection.\n\n");

    
    
    printf("Look into the Mirror once more, will you?\n\n");
}
