#include <stdio.h>
#include "conversor.h"

int main(void) {
    float metros;
    printf("Digite o valor em metros: ");
    scanf("%f", &metros);
    printf("%f cm" , MetrosParaCentimetros(metros));
    return 0;
}