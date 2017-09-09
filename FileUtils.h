//
// Created by ak on 9/1/2017.
//

#ifndef DATASECURITY_FILEUTILS_H
#define DATASECURITY_FILEUTILS_H

#include <stdio.h>
#include<stdlib.h>


FILE *getFileREADER(char *fileName) {

    FILE *FP;
    FP = fopen(fileName, "r");

    if (FP == NULL) {

        printf(" [E] Error opening File.\n");
        return NULL;
    }

    return FP;
}

FILE *getFileWRITER(char *fileName) {

    FILE *FP;
    FP = fopen(fileName, "w");

    if (FP == NULL) {

        printf("[E] Error opening File. \n");
        return NULL;
    }

    return FP;
}

int get_total_char_count(char *file_name) {
    FILE *FP = getFileREADER(file_name);
    char ch = '\0';
    int N = 0;
    while ((ch = fgetc(FP)) != EOF) { N++; }
    fclose(FP);
    return N;
}

#endif //DATASECURITY_FILEUTILS_H
