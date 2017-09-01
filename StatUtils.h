//
// Created by ak on 9/1/2017.
//

#ifndef DATASECURITY_STATUTILS_H
#define DATASECURITY_STATUTILS_H

#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include "FileUtils.h"

int *countDistributionOf40000Words() {

    /*
     * Ref: https://www.math.cornell.edu/~mec/2003-2004/cryptography/subs/frequencies.html
     *
     *
     * Total Letters = 182303;
     */

    int *englishDist = malloc(sizeof(int) * 27);

    englishDist[0] = 14810;
    englishDist[1] = 2715;
    englishDist[2] = 4943;
    englishDist[3] = 7874;
    englishDist[4] = 21912;
    englishDist[5] = 4200;
    englishDist[6] = 3693;
    englishDist[7] = 10795;
    englishDist[8] = 13318;
    englishDist[9] = 188;
    englishDist[10] = 1257;
    englishDist[11] = 7253;
    englishDist[12] = 4761;
    englishDist[13] = 12666;
    englishDist[14] = 14003;
    englishDist[15] = 3316;
    englishDist[16] = 205;
    englishDist[17] = 10977;
    englishDist[18] = 11450;
    englishDist[19] = 16587;
    englishDist[20] = 5246;
    englishDist[21] = 2019;
    englishDist[22] = 3819;
    englishDist[23] = 315;
    englishDist[24] = 3853;
    englishDist[25] = 128;
    englishDist[26] = 182303;

    return englishDist;
}

float correlation(int *dataX, int dataXlen, int *dataY, int dataYlen) {

    int i = 0, upto = dataXlen > dataYlen ? dataYlen : dataXlen;
    float Exy = 0, Ex = 0, Ey = 0, ExSq = 0, EySq = 0;

    for (i = 0; i < upto; i++) {

        float x = 0, y = 0;
        x = dataX[i];
        y = dataY[i];

        Exy += x * y;
        Ex += x;
        Ey += y;

        ExSq += x * x;
        EySq += y * y;
    }

    return (float) (upto * Exy - Ex * Ey)
           / (sqrt(upto * ExSq - Ex * Ex) * sqrt(upto * EySq - Ey * Ey));
}

int *getFrequencyENG(char *msgFileName) {

    int *eng = malloc(sizeof(int) * 27), j, upper = 65, lower = 97, i, l;

    for (j = 0; j < 27; j++) {
        eng[j] = 0;
    }

    FILE *FP;
    FP = getFileREADER(msgFileName);

    char ch;
    while ((ch = fgetc(FP)) != EOF) {

        for (i = 0; i <= 25; i++) {

            int asciOfC = (int) ch;
            if (asciOfC == (upper + i) || asciOfC == (lower + i)) {
                eng[i] += 1;
                eng[26] += 1;
                break;
            }

        }
    }

    for (l = 0; l < 26; l++) {
        printf("[%c : %d], ", l + 65, eng[l]);
    }
    printf("\nSUM : %d\n", eng[26]);
    printf("-------------------\n");

    return eng;
}

float indexOfCoincidenceENG_kappa(char *msgFileName) {

    int *eng = getFrequencyENG(msgFileName), l = 0;

    float Ef = 0.0f, EfFMinusOne = 0.0f;
    for (l = 0; l < 26; l++) {

        Ef += eng[l];
        EfFMinusOne += eng[l] * (eng[l] - 1);
    }

    return EfFMinusOne / (Ef * (Ef - 1));
}

float indexOfCoincidenceENG(char *msgFileName) {
    /*
     * 26 for English
     */
    return indexOfCoincidenceENG_kappa(msgFileName) * 26;
}

float psi(char *msgFileName) {

    int *eng = getFrequencyENG(msgFileName), i;

    float psi = 0.0f, N = (float) eng[26];
    for (i = 0; i < 26; i++) {

        psi += eng[i] / N * eng[i] / N;
    }

    return psi;
}

float phi(char *msgFileName) {

    int *eng = getFrequencyENG(msgFileName), i;
    float phi = 0.0f, N = (float) eng[26];

    for (i = 0; i < 26; i++) {
        float freq = (float) eng[i] / N;
        phi += freq * (freq - 1 / N);
    }

    return phi;
}

float chi(char *msgFileName) {

    int *eng = getFrequencyENG(msgFileName), i, *engExpected = countDistributionOf40000Words();

    float temp = 0.0f;
    for (i = 0; i < 26; i++) {

        temp += (float) eng[i] * engExpected[i];
    }

    return temp / (eng[26] * engExpected[26]);

}

void check_transposition_matrix(char msgFileName) {

    int i, j;

}

int get_index_adfgvx(char c, char arr[]) {

    for (int i = 0; i < 6; i++) {
        if (c == arr[i]) {
            return i;
        }
    }

    return -1;
}

char *decode_adfgvx(char *msgFileName) {

    char index_lower[6] = {'a', 'd', 'f', 'g', 'v', 'x'};
    char index_upper[6] = {'A', 'D', 'F', 'G', 'V', 'X'};
    char adfgvx[6][6] = {
            {'c', 'v', 'l', 'p', '2', 'q'},
            {'d', 'b', 'j', '4', '3', 'r'},
            {'a', 't', '5', 'e', 's', 'y'},
            {'k', 'w', 'f', 'i', 'z', '9'},
            {'8', 'g', '6', 'o', '0', 'x'},
            {'l', '7', 'm', 'h', 'n', 'u'}
    };

    FILE *FP;
    FP = getFileREADER(msgFileName);

    FILE *FWP;
    FWP = getFileWRITER("decrypted.txt");

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

        if( r_index>=0 && c_index>=0 ){
            fputc(adfgvx[r_index][c_index], FWP);
        }
    }

}

#endif //DATASECURITY_STATUTILS_H
