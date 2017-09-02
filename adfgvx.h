//
// Created by ak on 9/2/2017.
//

#ifndef DATASECURITY_ADFGVX_H
#define DATASECURITY_ADFGVX_H

#include <mem.h>
#include "FileUtils.h"
#include "StatUtils.h"


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
    char *decoded_file;
    strcpy(decoded_file, msgFileName);
    strcat(decoded_file, "_adfgvx.decrypted");

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
            fputc(adfgvx[r_index][c_index], FWP);
        }
    }
    fclose(FP);
    fclose(FWP);
    return decoded_file;
}

void columnar_transposition_attack(char *cipherSrcFile) {

    int *freq = getFrequencyENG(cipherSrcFile), charCount = freq[26], i, j;

    for (i = 1; i <= charCount; i++) {

        for (j = 1; j <= charCount; j++) {

            int checker = ceil((double) i * j);

            if (checker >= charCount && checker <= charCount * 2) {

                printf(" #### Trying: %d by %d \n", i, j);

                char to_permute[i][j], ch;
                int a = 0, b = 0, k = 0, l = 0, m, n;
                for (a = 0; a < i; a++) for (b = 0; b < j; b++) to_permute[a][b] = '\0';

                FILE *FP = getFileREADER(cipherSrcFile);
                while ((ch = fgetc(FP)) != EOF) {
                    if (l > (j - 1)) {
                        k++;
                        l = 0;
                    }
                    to_permute[k][l] = ch;
                    l++;
                }

                char new_cipher_file_name[50];
                strcpy(new_cipher_file_name, cipherSrcFile);
                strcat(new_cipher_file_name, "TEMP");

                FILE *FP_Decoded = getFileWRITER(new_cipher_file_name);
                for (m = 0; m < j; m++) {
                    for (n = 0; n < i; n++) {
                        int c = (int) to_permute[n][m];
                        if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {

                            fputc(c, FP_Decoded);
                        }
                    }
                }

                fclose(FP_Decoded);
                fclose(FP);

                char *decoded_file = decode_adfgvx(new_cipher_file_name);
                float psiValue = psi(decoded_file);
                printf("PSI Value: %f\n", psiValue);
                printf("-------------------------------------------------------------------------\n");
            }

        }
    }

}

#endif //DATASECURITY_ADFGVX_H
