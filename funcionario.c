#include <stdio.h>
#include <stdlib.h>

int main() {
    int id;
    char nome[50];
    float salario;

        printf("Digite o nome: ");
        scanf("%[^\n]", nome);
        printf("Digite o id: ");
        scanf("%d", &id);
        printf("Digite o salario: ");
        scanf("%f", &salario);
        
        FILE * arq = fopen("entrada.txt", "w");
        if (arq == NULL) { exit(1);};

        fprintf(arq, "%d\t%s\t%f", id, nome, salario);
        fclose(arq);
    return 0;
}