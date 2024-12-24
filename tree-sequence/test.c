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

#define MAX_DATA_DIGITS 20

void getTests(int* tRef, char**** testsRef){
    FILE *f = fopen("test_csv.csv", "r");
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

    LENGTH m = numberOfCommas+(n*MAX_DATA_DIGITS)+1;
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

void WRITE(FILE* f, char* operation, LENGTH n, clock_t c){
    fprintf(f, "%s|%zu|%f", operation, n, c);
}

int main(){
    int t = 0;
    char*** tests = NULL;
    getTests(&t, &tests);

    FILE* f = fopen("output.csv", "w+");
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
            n = atoll(arg1);
            DATA* seq = (DATA*) malloc(n*sizeof(DATA));
            char* token = strtok(arg2, ",");
            int i = 0;
            while (token != NULL){
                seq[i] = atoll(token);
                i++;
                token = strtok(NULL, ",");
            }

            c = clock();
            list = make(n, seq);
            c = clock() - c;
            
            if (strcmp(getAllElementsAsResult(list), RESULT) != 0){
                return -1;
            }
        } else if (operation == "get"){
            c = clock();
            DATA data = get(list, atoll(arg1));
            c = clock() - c;

            char datastr[MAX_DATA_DIGITS];
            sprintf(datastr, "%" PRIu64, data);
            if (strcmp(datastr, RESULT) != 0){
                return -1;
            }
        }

        WRITE(f, operation, n, c);
    }
}
