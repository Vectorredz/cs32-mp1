#include <stdio.h>
#include <stdlib.h>

void export_delta_time(int *seq){
    int *ret_det = malloc(10 * sizeof(int));
    for (int testNums = 0; testNums < 10; testNums++){
        ret_det[testNums] =  seq[testNums];
        printf("%d \n", ret_det[testNums]);
    }

    FILE *file = fopen("deldata.txt", "w");

    for (int i = 0; i < 5; i++){
        fprintf(file, "%d\n", ret_det[i]);
    }
    
    fclose(file);
}

int main(){
    int *seq = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++){
        seq[i] = i;
    }
    export_delta_time(seq);
}