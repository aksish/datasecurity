#include <stdio.h>
#include "StatUtils.h"

int main(int arlen, char **args) {
    printf("Running...\n");

    float pHi = phi(args[1]);
    printf("Phi Value: %f", pHi);

    float pSi = psi(args[1]);
    printf("Psi Value: %f", pSi);
    return 0;
}