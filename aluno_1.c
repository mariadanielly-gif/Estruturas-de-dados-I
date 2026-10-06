#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int mat;
    char nome[50], curso[30];
    float media;
} Aluno;

void cadastrar() {
    FILE *f = fopen("alunos.dat", "ab");
    Aluno a;
    printf("Matricula: ");
    scanf("%d", &a.mat);
    printf("Nome: ");
    scanf("%s", a.nome);
    printf("Curso: ");
    scanf("%s", a.curso);
    printf("Media: ");
    scanf("%f", &a.media);

    fwrite(&a, sizeof(Aluno), 1, f);
    fclose(f);
}

void listar() {
    FILE *f = fopen("alunos.dat", "rb");
    Aluno a;
    if (!f) return;
    while (fread(&a, sizeof(Aluno), 1, f))
        printf("Mat: %d | Nome: %s | Curso: %s | Media: %.2f\n", a.mat, a.nome, a.curso, a.media);
    fclose(f);
}

void buscar_ou_alterar(int mat, int alterar) {
    FILE *f = fopen("alunos.dat", "rb+");
    Aluno a;
    if (!f) return;
    while (fread(&a, sizeof(Aluno), 1, f)) {
        if (a.mat == mat) {
            if (alterar) {
                printf("Nova media: ");
                scanf("%f", &a.media);
                fseek(f, -(long)sizeof(Aluno), SEEK_CUR);
                fwrite(&a, sizeof(Aluno), 1, f);
                printf("Media alterada!\n");
            } else {
                printf("Encontrado: %s | Curso: %s | Media: %.2f\n", a.nome, a.curso, a.media);
            }
            fclose(f);
            return;
        }
    }
    printf("Aluno nao encontrado.\n");
    fclose(f);
}

int main() {
    int op, mat;
    do {
        printf("\n1-Cadastrar 2-Listar 3-Buscar 4-Alterar Media 0-Sair: ");
        scanf("%d", &op);
        if (op == 1) cadastrar();
        else if (op == 2) listar();
        else if (op == 3 || op == 4) {
            printf("Matricula: ");
            scanf("%d", &mat);
            buscar_ou_alterar(mat, op == 4);
        }
    } while (op != 0);
    return 0;
}