/*
    << Water Moon. >>
    Look into the Mirror...
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
#include "RSettings.h"
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>
// Do not edit past this point!
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>


// TODO: implement other os later
// #include <windows.h>
// #include <sys/timeb.h>
// typedef LARGE_INTEGER RECORDED_TIME;
// typedef double PROCESSED_TIME;
// #define TIME_FORMAT "lf"
// uint64_t freq;
// void _TIME_init(){
//     LARGE_INTEGER f;
//     QueryPerformanceFrequency(&f);
//     freq = (uint64_t) ((uint64_t)(f.HighPart) << 32) | (uint32_t) f.LowPart;
// }
// void _TIME(RECORDED_TIME* cRef){
//     QueryPerformanceCounter(cRef);
// }
// PROCESSED_TIME _PROCESSTIME(RECORDED_TIME a, RECORDED_TIME b){
//     uint64_t bQ = (uint64_t) ((uint64_t)(b.HighPart) << 32) | (uint32_t) b.LowPart;
//     uint64_t aQ = (uint64_t) ((uint64_t)(a.HighPart) << 32) | (uint32_t) a.LowPart;
//     uint64_t dt = (bQ-aQ);
//     PROCESSED_TIME final = (double)(dt * 1000.0L)/(double)freq;
//     return final < 0.0L ? 0.0L : final;
// }
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





#include "../H_global.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_NUMBER_DIGITS 25

void getTests(char* inputFileName, int* tRef, char**** testsRef){
    FILE *f = fopen(inputFileName, "r");
    int lines = 0;
    bool newline = true;

    char* testBuffer = (char*) malloc(2*sizeof(char));
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

    int i = 0;
    while (true){
        long seekBack = ftell(f);
        int length = 0;
        bool nlSkip = false;
        bool eof = false;
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
        
        char* buffer = (char*) malloc((length+1)*sizeof(char));
        fgets(buffer, length+1, f);
        
        char** testLine = (char**) malloc(4*sizeof(char*));
        char* token = strtok(buffer, "|");
        int j = 0;
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



char* getAllElementsAsResult(Reflection* list){
    LENGTH n = size(list);
    if (n == 0){
        return "EMPTY";
    }

    LENGTH numberOfCommas = n-1;
    LENGTH m = numberOfCommas+(n*MAX_NUMBER_DIGITS)+1;
    char* mRESULT = (char*) malloc(m*sizeof(char));
    LENGTH m0 = 0;
    for (LENGTH i = 0; i < n; i++){
        DATA data = get(list, i);
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
    return b == false ? "0" : "1";
}


void VERIFY(int lineNum, char* operation, char* RESULT, char* mRESULT){
    if (strcmp(mRESULT, RESULT) != 0){
        printf("!! FAULTY OUTPUT:: %s\n", mRESULT);
        printf("> !! Failed Operation !!\n");
        printf(":: line %d\n:: operation %s\n", lineNum+1, operation);
        assert(0);
    }
    free(mRESULT);
}

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

int main(){
    printf("<< Water Moon. >>\nLook into the Mirror...\n");
    printf("> Getting tests for ((" INPUT_FILE ")) ...\n");
    int totalTests = 0;
    char*** tests = NULL;
    getTests(INPUT_FILE, &totalTests, &tests);
    printf("> Done.\n");
    
    printf("> Initializing variables...\n");
    _TIME_init();
    int totalOperations = 0;
    for (int testNum = 0; testNum < totalTests; testNum++){
        char** testLine = tests[testNum];
        char* operation = testLine[0];
        if (strcmp(operation, "LAYER") != 0 && strcmp(operation, "LAYERFIN") != 0){
            totalOperations++;
        }
    }
    WRITEDATA* writeDataLines = (WRITEDATA*) malloc(totalOperations*sizeof(WRITEDATA));
    printf("> Done.\n");

    printf("> Conducting test operations...\n");
    FILE* f = fopen(OUTPUT_FILE, "w+");
    Reflection* list = NULL;
    int opCounter = 0;
    for (int testNum = 0; testNum < totalTests; testNum++){
        char** testLine = tests[testNum];
        char* operation = testLine[0];
        char* arg1 = testLine[1];
        char* arg2 = testLine[2];
        char* RESULT = testLine[3];
        
        if (strcmp(operation, "LAYER") == 0){
            printf("> !! LAYER %s !!\n", arg1);
        } else if (strcmp(operation, "LAYERFIN") == 0){
            printf("> !! LAYER %s Passed. !!\n", arg1);
        } else {
            LENGTH n = list != NULL ? size(list) : 0;
            RECORDED_TIME c, _c;

            if (strcmp(operation, "make") == 0){
                n = strToLength(arg1);
                DATA* seq = (DATA*) malloc(n*sizeof(DATA));
                char* token = strtok(arg2, ",");
                int i = 0;
                while (token != NULL){
                    seq[i] = strToData(token);
                    i++;
                    token = strtok(NULL, ",");
                }

                _TIME(&_c);
                list = make(n, seq);
                _TIME(&c);
                if (strcmp("X", RESULT) != 0){
                    VERIFY(testNum, operation, RESULT, getAllElementsAsResult(list));
                }

            } else if (strcmp(operation, "size") == 0){
                _TIME(&_c);
                LENGTH listSize = size(list);
                _TIME(&c);
                if (strcmp("X", RESULT) != 0){
                    VERIFY(testNum, operation, RESULT, lengthToStr(listSize));
                }

            } else if (strcmp(operation, "empty") == 0){
                _TIME(&_c);
                bool listEmpty = empty(list);
                _TIME(&c);
                if (strcmp("X", RESULT) != 0){
                    VERIFY(testNum, operation, RESULT, boolToStr(listEmpty));
                }

            } else if (strcmp(operation, "reverse") == 0){
                _TIME(&_c);
                reverse(list);
                _TIME(&c);
                if (strcmp("X", RESULT) != 0){
                    VERIFY(testNum, operation, RESULT, getAllElementsAsResult(list));
                }

            } else if (strcmp(operation, "get") == 0){
                LENGTH i = strToLength(arg1);

                _TIME(&_c);
                DATA data = get(list, i);
                _TIME(&c);
                if (strcmp("X", RESULT) != 0){
                    VERIFY(testNum, operation, RESULT, dataToStr(data));
                }

            } else if (strcmp(operation, "set") == 0){
                LENGTH i = strToLength(arg1);
                DATA v = strToData(arg2);

                _TIME(&_c);
                set(list, i, v);
                _TIME(&c);
                if (strcmp("X", RESULT) != 0){
                    VERIFY(testNum, operation, RESULT, dataToStr(get(list, i)));
                }

            } else if (strcmp(operation, "peek_left") == 0){
                _TIME(&_c);
                DATA data = peek_left(list);
                _TIME(&c);
                if (strcmp("X", RESULT) != 0){
                    VERIFY(testNum, operation, RESULT, dataToStr(data));
                }

            } else if (strcmp(operation, "peek_right") == 0){
                _TIME(&_c);
                DATA data = peek_right(list);
                _TIME(&c);
                if (strcmp("X", RESULT) != 0){
                    VERIFY(testNum, operation, RESULT, dataToStr(data));
                }

            } else if (strcmp(operation, "push_left") == 0){
                DATA v = strToData(arg1);

                _TIME(&_c);
                push_left(list, v);
                _TIME(&c);
                if (strcmp("X", RESULT) != 0){
                    VERIFY(testNum, operation, RESULT, getAllElementsAsResult(list));
                }

            } else if (strcmp(operation, "push_right") == 0){
                DATA v = strToData(arg1);

                _TIME(&_c);
                push_right(list, v);
                _TIME(&c);
                if (strcmp("X", RESULT) != 0){
                    VERIFY(testNum, operation, RESULT, getAllElementsAsResult(list));
                }

            } else if (strcmp(operation, "pop_left") == 0){
                _TIME(&_c);
                pop_left(list);
                _TIME(&c);
                if (strcmp("X", RESULT) != 0){
                    VERIFY(testNum, operation, RESULT, getAllElementsAsResult(list));
                }

            } else if (strcmp(operation, "pop_right") == 0){
                _TIME(&_c);
                pop_right(list);
                _TIME(&c);
                if (strcmp("X", RESULT) != 0){
                    VERIFY(testNum, operation, RESULT, getAllElementsAsResult(list));
                }
            }

            WRITEDATA wd = *((WRITEDATA*) malloc(sizeof(WRITEDATA)));
            char* opCopy = (char*) malloc((strlen(operation)+1)*sizeof(char)); //wtf
            wd.operation = strcpy(opCopy, operation);
            wd.n = n;
            wd.c = _PROCESSTIME(_c, c);
            writeDataLines[opCounter] = wd;
            opCounter++;
        }
    }
    printf("> Done.\n");

    printf("> Writing deltatime benchmarks to output...\n");
    for (int opNum = 0; opNum < totalOperations; opNum++){
        WRITE(f, writeDataLines[opNum], opNum < totalOperations-1 ? true : false);
    }
    printf("> Done.\n");

    printf("> Cleanup...\n");
    for (int testNum = 0; testNum < totalTests; testNum++){
        char** testLine = tests[testNum];
        free(testLine);
    }
    free(tests);
    for (int opNum = 0; opNum < totalOperations; opNum++){
        free(writeDataLines[opNum].operation);
    }
    free(writeDataLines);
    fclose(f);
    printf("> Done.\n");

    printf("\n<<<< TESTING SUCCESSFUL >>>>\n");
    printf("? Ah...\n? Despite everything, it's still you.\n\n...a mere illusion.\nA mere Reflection.\n\n");

    printf("!! Time plot points can be found in ((" OUTPUT_FILE ")) !!\n\n");
    
    printf("Look into the Mirror once more, will you?\n");
}
