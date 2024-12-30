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
- OUTPUT_FILE is a csv file composed of Fields (OPERATION | N | DELTATIME)
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

            ||-- TIMER --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>


// TIMER for outputting execution time plots
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




// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- HELPERS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>

void getTests(char* inputFileName, size_t* tRef, char**** testsRef){
    FILE *f = fopen(inputFileName, "r");
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
            return;
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
            return;
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

    *tRef = lines;
    *testsRef = tests;
    fclose(f);
}



char* listToResult(Reflection* list, bool includeReversal){
    // Turns a sequence of DATA values into a string delimeted by ","
    LENGTH n;
    DATA* seq;
    TEST_elements(list, &n, &seq);
    if (n == 0){
        return "EMPTY";
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
void VERIFY(Reflection* list, size_t lineNum, char* operation, char* RESULT, char* mRESULT, double dt, char* extraOperation, bool checkForCorrectness, bool checkForEfficiency){
    if (checkForCorrectness == true){
        if (strcmp(mRESULT, RESULT) != 0){
            fprintf(stderr, "[/] [line %zu]: WA [%lfms]\n", lineNum, dt);
            fprintf(stderr, "!! Failed Operation !!\n");
            
            size_t i = 0;
            while (RESULT[i] == mRESULT[i] && RESULT[i] != '\0' && mRESULT[i] != '\0'){
                i++;
            }
            DISPLAY_LOGS(list, operation, extraOperation);
            fprintf(stderr, ":: line %zu\n:: column %zu (char CORRECT: \"%c\", FAULT: \"%c\")\n", lineNum, i, RESULT[i], mRESULT[i]);
            fprintf(stderr, ":: FAULTY OUTPUT -> %s\n", mRESULT);
            fprintf(stderr, ":: SUPPOSED OUTPUT -> %s\n", RESULT);
            exit(1);
        }
    }
    if (checkForEfficiency == true){
        if (dt > TLE_BOUND){
            fprintf(stderr, "[+] [line %zu]: TLE [%lfms (> %lf ms)]\n", lineNum, dt, TLE_BOUND);
            DISPLAY_LOGS(list, operation, extraOperation);
            exit(1);
        }
    }
}

// Used for time plots output
typedef struct _WRITEDATA {
    char* operation;
    LENGTH n;
    PROCESSED_TIME c;
} WRITEDATA;
void WRITE(FILE* f, WRITEDATA wd, bool newLine){
    fprintf(f, "%s|%zu|%" TIME_FORMAT, wd.operation, wd.n, wd.c);
    if (newLine == true){
        fprintf(f, "\n");
    }
}


// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- MAIN TESTER --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>

int main(){
    printf("<< Water Moon. >>\nWill your Reflection be the same as mine?\n");

    // Get tests first
    printf("> Getting tests for ((" INPUT_FILE ")) ...\n");
    size_t totalTests = 0;
    char*** tests = NULL;
    getTests(INPUT_FILE, &totalTests, &tests);
    printf("> Done.\n");
    
    
    printf("> Initializing variables...\n");
    // Initialize timer
    _TIME_init();

    // Get the number of actual operations (since a line can contain the custom "LAYER"/"LAYERFIN" message which doesn't count as an operation)
    size_t totalOperations = 0;
    for (size_t testNum = 0; testNum < totalTests; testNum++){
        char** testLine = tests[testNum];
        char* operation = testLine[0];
        if (strcmp(operation, "LAYER") != 0 && strcmp(operation, "LAYERFIN") != 0){
            totalOperations++;
        }
    }

    // Where the output values are stored
    WRITEDATA* writeDataLines = (WRITEDATA*) malloc(totalOperations*sizeof(WRITEDATA));
    printf("> Done.\n");

    printf("> Conducting test operations...\n");
    FILE* f = fopen(OUTPUT_FILE, "w+");
    Reflection* list = NULL;

    size_t opCounter = 0;
    bool prevWasMsg = false;

    RECORDED_TIME _timeGlobal, timeGlobal;
    _TIME(&_timeGlobal);

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
        if (strcmp(operation, "LAYER") == 0){
            prevWasMsg = true;
            printf("!! LAYER %s !!\n", arg1);
            continue;
        }
        if (strcmp(operation, "LAYERFIN") == 0){
            prevWasMsg = true;
            printf("!! LAYER %s Passed. !!\n", arg1);
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
            char* token = strtok(arg2, ",");
            size_t i = 0;
            while (token != NULL){
                seq[i] = strToData(token);
                i++;
                token = strtok(NULL, ",");
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
            VERIFY(list, testNum+1, operation, "success", TEST_internal(list) == true ? "success" : "fail", dt, "TEST_internal", checkForCorrectness, false);
        }

        // Verify the output
        VERIFY(list, testNum+1, operation, RESULT, mRESULT, dt, extraOperation, checkForCorrectness, true);

        if (LINE_DISPLAY == true){
            printf("[O] [line %zu]: AC [%lfms]\n", testNum+1, dt);
        }

        // Finally, write the new benchmark to output array
        char* opCopy = (char*) malloc((strlen(operation)+1)*sizeof(char));
        strcpy(opCopy, operation);
        
        WRITEDATA wd = *((WRITEDATA*) malloc(sizeof(WRITEDATA)));
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

    _TIME(&timeGlobal);
    if (LINE_DISPLAY == true){
        if (opCounter > 0 && prevWasMsg == false){
            printf("\033[A\033[K\033[A\033[K");
        }
    }
    printf("[O] [GLOBAL]: AC [%lfs]\n", _PROCESSTIME(_timeGlobal, timeGlobal));
    
    printf("> Well Done.\n");

    printf("> Writing deltatime benchmarks to output...\n");
    // Write all benchmarks to the file
    for (size_t opNum = 0; opNum < totalOperations; opNum++){
        WRITE(f, writeDataLines[opNum], opNum < totalOperations-1 ? true : false);
        free(writeDataLines[opNum].operation);
    }
    printf("> Done.\n");

    printf("> Cleanup...\n");
    free(tests);
    free(writeDataLines);
    fclose(f);
    printf("> Done.\n\n");
    
    printf("---------------------------------------------------------\n");
    printf("<<<< TESTING SUCCESSFUL >>>>\n");
    printf("!! Time plot points can be found in ((" OUTPUT_FILE "))\n\n");
    printf("? Ah...\n? Despite everything, it's still you...\n...A mere Reflection.\n\n");

    
    
    printf("Look into the Mirror once more, will you?\n\n");
}
