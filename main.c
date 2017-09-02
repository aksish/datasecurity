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
    freopen("ADFGVX_assignment.LOG", "a+", stdout);
    columnar_transposition_attack(args[1]);

    return 0;
}