#include <stdio.h>
#include "StatUtils.h"
#include "adfgvx.h"

int main(int arlen, char **args) {
    printf("Running...\n");

//    float pHi = phi(args[1]);
//    printf("Phi Value: %f", pHi);
//
//    float pSi = indexOfCoincidenceENG_kappa(args[1]);
//    printf("Index of Coincidence: %f", pSi);

    // decode_adfgvx(args[1]);

    /*
     * Redirect to file.
     */
//      freopen("AAA_OUTPUT_LOG_NEW_ADFGVX_assignment2.log", "a", stdout);
//    columnar_transposition_attack(args[1]);
//    char *decoded = decode_adfgvx(args[1]);
//    float psiV = psi(decoded);
//
//    printf("\nPSI Value: %f", psiV);

//    int count = get_total_char_count(args[1]);
//    float psi = psi_adfgvx(args[1],count);
//    printf("Original PSI: %f",psi);
//  columnar_transposition_attack(args[1]);
    tyr_3boat_txt(args[1]);
//    strip_non_alpha(args[1]);
    return 0;
}