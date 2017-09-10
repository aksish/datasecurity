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

//    int q =0;
//    for(q=0;q<36;q++){
//        printf("%c%c:%d ",freq[q][0],freq[q][1],freq[q][2]);
//    }
//    printf("\n");
    fclose(FP);
    return freq;
}


float psi_adfgvx(char *msgFileName, int total_count) {

    int **freq = get_adfgvx_frequency(msgFileName), i = 0;
    float psi = 0.0f;
    for (i = 0; i < 36; i++) {
        float f = (float) freq[i][2] / total_count;
        psi += f * f;
    }
    return psi;
}

void columnar_transposition_attack(char *cipherSrcFile) {

    int charCount = get_total_char_count(cipherSrcFile), i, j;

    for (i = 1; i <= charCount; i++) {

        for (j = 1; j <= charCount; j++) {
            int checker = i * j;
            if (checker >= charCount && checker <= charCount * 2) {
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

                char new_cipher_file_name[100];
                snprintf(new_cipher_file_name, sizeof(char) * 100, "%s-%iX%i-CIPHER", cipherSrcFile, i, j);

                FILE *FP_NEW = getFileWRITER(new_cipher_file_name);
                for (m = 0; m < j; m++) {
                    for (n = 0; n < i; n++) {
                        int c = (int) to_permute[n][m];
                        if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {

                            fputc(c, FP_NEW);
                        }
                    }
                }

                fclose(FP_NEW);
                fclose(FP);

                float psiValue = psi_adfgvx(new_cipher_file_name, get_total_char_count(new_cipher_file_name)/2);

                char new_new_cipher_file[100];
                snprintf(new_new_cipher_file, sizeof(char) * 100, "%f-%s", psiValue, new_cipher_file_name);
                if (psiValue < 0.04) remove(new_cipher_file_name);
                else rename(new_cipher_file_name, new_new_cipher_file);
                printf("PSI Value: %f\n", psiValue);
            }

        }
    }


}

/*
 * Takes 1099 length substring and calculates psi
 */
void tyr_3boat_txt(char *file_name) {
    FILE *FP = getFileREADER(file_name);
    char *write_file = "3boat10.trial";
    FILE *FW = getFileWRITER(write_file);
    int count = 0;
    char ch;
    int offset = 0;
    while ((ch = fgetc(FP)) != EOF) {
        if (count % 1099 == 0 && count > 0) {
            fclose(FW);
            float psi_value = psi(write_file);
            if(psi_value>0.06 && psi_value<0.061)
                printf("PSI : %f, Offset:%d \n", psi_value, offset);
            offset += 1;
            fseek(FP, offset, SEEK_SET);
            ch = fgetc(FP);
            FW = getFileWRITER(write_file);
        }
        fputc(ch, FW);
        count++;
    }

}

#endif //DATASECURITY_ADFGVX_H
