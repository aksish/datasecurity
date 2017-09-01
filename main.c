#include <stdio.h>
#include "StatUtils.h"

int main(int arlen, char **args) {
    printf("Running...\n");

    float p = psi(args[1]);
    printf("%f", p);
    return 0;
}