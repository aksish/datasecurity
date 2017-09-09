//
// Created by ak on 9/2/2017.
//

#ifndef DATASECURITY_ADFGVX_H
#define DATASECURITY_ADFGVX_H

#include <mem.h>
#include "FileUtils.h"
#include "StatUtils.h"
#include "ctype.h"


int get_index_adfgvx(char c, char arr[]) {

    for (int i = 0; i < 6; i++) {
        if (c == arr[i]) {
            return i;
        }
    }

    return -1;
}


char *decode_adfgvx(char *msgFileName, char **adfgvx_table) {

    char index_lower[6] = {'a', 'd', 'f', 'g', 'v', 'x'};
    char index_upper[6] = {'A', 'D', 'F', 'G', 'V', 'X'};

    char *decoded_file;
    strcpy(decoded_file, msgFileName);
    strcat(decoded_file, ".MESSAGE");

    FILE *FP;
    FP = getFileREADER(msgFileName);

    FILE *FWP;
    FWP = getFileWRITER(decoded_file);

    char row;
    while ((row = fgetc(FP)) != EOF) {

        char column = fgetc(FP);

        int r_index = get_index_adfgvx(row, index_lower);
        if (r_index < 0) {
            r_index = get_index_adfgvx(row, index_upper);
        }

        int c_index = get_index_adfgvx(column, index_lower);
        if (c_index < 0) {
            get_index_adfgvx(column, index_upper);
        }

        if (r_index >= 0 && c_index >= 0) {
            fputc(adfgvx_table[r_index][c_index], FWP);
        }
    }
    fclose(FP);
    fclose(FWP);
    return decoded_file;
}

char *strip_non_alpha(char *file_name) {

    FILE *FP;
    FP = getFileREADER(file_name);

    char *dest_file;
    strcpy(dest_file, file_name);
    strcat(dest_file, ".stripped");
    FILE *FWP;
    FWP = getFileWRITER(dest_file);

    char c;
    while ((c = fgetc(FP)) != EOF) {

        if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
            fputc(tolower(c), FWP);
        }
    }
    fclose(FP);
    fclose(FWP);
    return dest_file;
}

int **get_adfgvx_frequency(char *file_name) {

    FILE *FP = getFileREADER(file_name);
    /*
     * Initialize with 36 combinations.
     */

    int **freq, i, j, k = 0, f = 0;
    freq = malloc(sizeof(int) * 36);
    for (f = 0; f < 36; f++) freq[f] = malloc(sizeof(int) * 3);

    char adfgvx[6] = {'a', 'd', 'f', 'g', 'v', 'x'}, m;

    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            freq[k][0] = adfgvx[i];
            freq[k][1] = adfgvx[j];
            freq[k][2] = 0;
            k++;
        }
    }
    char c, c1;
    while ((c = fgetc(FP)) != EOF) {
        c1 = fgetc(FP);
        for (m = 0; m < 36; m++) {
            char cx = freq[m][0], cy = freq[m][1];
            if (c == cx && c1 == cy) {
                freq[m][2] = freq[m][2] + 1;
            }
        }
    }

    return freq;
}

float psi_adfgvx(char *msgFileName) {

    int **freq = get_adfgvx_frequency(msgFileName), i = 0, N = 0;

    for (i = 0; i < 36; i++) N += freq[i][2];

    float psi = 0.0f;

    for (i = 0; i < 36; i++) {
        float f = (float) freq[i][2] / N;
        psi += f * f;
    }
    return psi;
}

void columnar_transposition_attack(char *cipherSrcFile) {

    int *freq = getFrequencyENG(cipherSrcFile), charCount = freq[26], i, j;

    for (i = 1; i <= charCount; i++) {

        for (j = 1; j <= charCount; j++) {

            int checker = ceil((double) i * j);

            if (checker >= charCount && checker <= charCount * 2) {

                //Todo

            }

        }
    }

}

#endif //DATASECURITY_ADFGVX_H
