//
// Created by ak on 9/1/2017.
//

#ifndef DATASECURITY_FILEUTILS_H
#define DATASECURITY_FILEUTILS_H

#include <stdio.h>
#include<stdlib.h>


FILE *getFileREADER(char *fileName) {

    printf("\n ### Reading File: %s ### \n", fileName);

    FILE *FP;
    FP = fopen(fileName, "r");

    if(FP==NULL){

        printf("Error opening File.\n");
        return NULL;
    }

    return FP;
}

FILE *getFileWRITER(char *fileName) {

    printf("\nReading File: %s\n", fileName);

    FILE *FP;
    FP = fopen(fileName, "w");

    if(FP==NULL){

        printf("Error opening File.");
        return NULL;
    }

    return FP;
}

#endif //DATASECURITY_FILEUTILS_H
