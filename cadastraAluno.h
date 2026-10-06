#include <stdio.h>
#include <stdlib.h>

void cadastraAluno(char ** nome, int * mat) {
    printf("Digite o nome do aluno: ");
    scanf("%[^\n]", *nome);
    printf("Digite a matrícula: ");
    scanf("%d", mat);
    
   
}

void imprimeAluno(char * nome, int mat) {
    printf("Nome: %s\n", nome);
    printf("Matrícula: %d\n", mat);
}

int main() {
    char nome[50];
    int mat;

    cadastraAluno(&nome, &mat);
    imprimeAluno(nome, mat);

    return 0;
}