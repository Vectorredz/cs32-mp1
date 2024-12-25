// --------------------------------------------------------- >>
// Modify both header and typedef to the list to test
#include "PTreeList.c"
typedef PTreeList Reflection;

// --------------------------------------------------------- >>
// Do not edit past this point!
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>



#include "../H_global.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define MAX_NUMBER_DIGITS 25

void getTests(char* fileName, int* tRef, char**** testsRef){
    FILE *f = fopen(fileName, "r");
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
    LENGTH numberOfCommas = n-1;

    LENGTH m = numberOfCommas+(n*MAX_NUMBER_DIGITS)+1;
    char* mRESULT = (char*) malloc(m*sizeof(char));
    LENGTH m0 = 0;
    for (LENGTH i = 0; i < n; i++){
        DATA data = get(list, i);
        m0 += sprintf(&mRESULT[m0], "%" PRIu64, data);
        if (i < n-1){
            m0 += sprintf(&mRESULT[m0], ",");
        }
    }
    return mRESULT;
}

LENGTH strToLength(char* lStr){
    char* e;
    LENGTH l = strtoll(lStr, &e, 10);
    free(e);
    return l;
}
DATA strToData(char* dataStr){
    char* e;
    DATA data = strtoll(dataStr, &e, 10);
    free(e);
    return data;
}
char* lengthToStr(LENGTH l){
    char* lengthStr = (char*) malloc((MAX_NUMBER_DIGITS+1)*sizeof(char));
    sprintf(lengthStr, "%zu", l);
    return lengthStr;
}
char* dataToStr(DATA data){
    char* dataStr = (char*) malloc((MAX_NUMBER_DIGITS+1)*sizeof(char));;
    sprintf(dataStr, "%" PRIu64, data);
    return dataStr;
}
char* boolToStr(bool b){
    return b == false ? "0" : "1";
}

void VERIFY(char* mRESULT, char* RESULT){
    printf("%s, %s\n", mRESULT, RESULT);
    if (strcmp("X", RESULT) != 0) assert(strcmp(mRESULT, RESULT) == 0);
    free(mRESULT);
}
void WRITE(FILE* f, char* operation, LENGTH n, clock_t c, bool newLine){
    fprintf(f, "%s|%zu|%f", operation, n, c);
    if (newLine == true){
        fprintf(f, "\n");
    }
}

int main(){
    int t = 0;
    char*** tests = NULL;
    getTests("test_csv_0.csv", &t, &tests);

    FILE* f = fopen("test_output.csv", "w+");
    Reflection* list = NULL;
    for (int t0 = 0; t0 < t; t0++){
        char** testLine = tests[t0];
        char* operation = testLine[0];
        char* arg1 = testLine[1];
        char* arg2 = testLine[2];
        char* RESULT = testLine[3];

        LENGTH n = list != NULL ? size(list) : 0;
        clock_t c = 0;

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

            c = clock();
            list = make(n, seq);
            c = clock() - c;
            VERIFY(getAllElementsAsResult(list), RESULT);
            
        } else if (strcmp(operation, "size") == 0){
            c = clock();
            LENGTH listSize = size(list);
            c = clock() - c;
            VERIFY(lengthToStr(listSize), RESULT);

        } else if (strcmp(operation, "empty") == 0){
            c = clock();
            bool listEmpty = empty(list);
            c = clock() - c;
            VERIFY(boolToStr(listEmpty), RESULT);
            
        } else if (strcmp(operation, "reverse") == 0){
            c = clock();
            reverse(list);
            c = clock() - c;
            VERIFY(getAllElementsAsResult(list), RESULT);

        } else if (strcmp(operation, "get") == 0){
            LENGTH i = strToLength(arg1);

            c = clock();
            DATA data = get(list, i);
            c = clock() - c;
            VERIFY(dataToStr(data), RESULT);

        } else if (strcmp(operation, "set") == 0){
            LENGTH i = strToLength(arg1);
            DATA v = strToData(arg2);

            c = clock();
            set(list, i, v);
            c = clock() - c;

            VERIFY(dataToStr(get(list, i)), RESULT);
        
        } else if (strcmp(operation, "peek_left") == 0){
            c = clock();
            DATA data = peek_left(list);
            c = clock() - c;
            VERIFY(dataToStr(data), RESULT);
        } else if (strcmp(operation, "peek_right") == 0){
            c = clock();
            DATA data = peek_right(list);
            c = clock() - c;
            VERIFY(dataToStr(data), RESULT);

        } else if (strcmp(operation, "push_left") == 0){
            DATA v = strToData(arg1);

            c = clock();
            push_left(list, v);
            c = clock() - c;

            VERIFY(getAllElementsAsResult(list), RESULT);

        } else if (strcmp(operation, "push_right") == 0){
            DATA v = strToData(arg1);

            c = clock();
            push_right(list, v);
            c = clock() - c;

            VERIFY(getAllElementsAsResult(list), RESULT);

        } else if (strcmp(operation, "pop_left") == 0){
            c = clock();
            pop_left(list);
            c = clock() - c;

            VERIFY(getAllElementsAsResult(list), RESULT);

        } else if (strcmp(operation, "pop_right") == 0){
            c = clock();
            pop_right(list);
            c = clock() - c;

            VERIFY(getAllElementsAsResult(list), RESULT);

        }

        WRITE(f, operation, n, c, t0<t-1 ? true : false);
        free(operation);
        free(arg1);
        free(arg2);
        free(RESULT);
        free(testLine);
    }
    free(tests);
    fclose(f);
}
