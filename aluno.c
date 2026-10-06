#include <stdio.h>
#include <stdlib.h>


int main() {
    char nome[50];
    float nota;
    int N;

    FILE * arq = fopen("aluno.txt", "w");
        if (arq == NULL) { exit(1);};

        printf("Digite a quantidade de alunos:\n");
        scanf("%d", &N);
        for (int i = 0; i < N; i++) {
        printf("Digite o nome: ");
        scanf(" %[^\n]", nome);
        printf("Digite a nota: ");
        scanf("%f", &nota);

        fprintf(arq, "%s\t%f", nome, nota);
        }
        fclose(arq);
    return 0;
}