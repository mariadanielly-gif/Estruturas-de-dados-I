#include <stdio.h>
#include <stdlib.h>

#define TOTAL_ALUNOS 5

int main() {
    FILE *arquivo;
    int matricula;
    char nome[100];
    float nota;

    // --- ITEM A: Solicitar dados e armazenar no arquivo alunos.txt ---
    arquivo = fopen("alunos.txt", "w"); // Abre para escrita (cria/sobrescreve)
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return 1;
    }

    printf("=== CADASTRO DE ALUNOS ===\n");
    for (int i = 0; i < TOTAL_ALUNOS; i++) {
        printf("\nAluno %d:\n", i + 1);
        printf("Matricula: ");
        scanf("%d", &matricula);
        
        printf("Nome (sem espaços): ");
        scanf("%s", nome);
        
        printf("Nota final: ");
        scanf("%f", &nota);

        // Escreve os dados no arquivo no formato especificado
        fprintf(arquivo, "%d %s %.1f\n", matricula, nome, nota);
    }

    // --- ITEM B: Fechar o arquivo ---
    fclose(arquivo);
    printf("\nDados gravados e arquivo fechado com sucesso.\n\n");

    // --- ITEM B e C: Reabrir no modo leitura e exibir na tela ---
    arquivo = fopen("alunos.txt", "r"); // Abre para leitura
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return 1;
    }

    printf("=== DADOS LIDOS DO ARQUIVO (alunos.txt) ===\n");
    
    // Lê e exibe os dados enquanto houver registros no arquivo
    while (fscanf(arquivo, "%d %s %f", &matricula, nome, &nota) == 3) {
        printf("Matricula: %d | Nome: %s | Nota: %.1f\n", matricula, nome, nota);
    }

    // Fecha o arquivo novamente após a leitura
    fclose(arquivo);

    return 0;
}