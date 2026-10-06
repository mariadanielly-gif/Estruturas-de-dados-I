#include <stdio.h>
#include <stdlib.h>

int main(void){

    FILE *arq = fopen("texto.txt", "w");
    if (arq == NULL) { exit(1);};

int ch;
    int total_caracteres = 0;
    int total_palavras = 0;
    int total_linhas = 0;
    int total_letras_a = 0;
    
    int em_palavra = 0;
    int ultimo_caractere = EOF;

    // Leitura caractere por caractere
    while ((ch = fgetc(arq)) != EOF) {
        total_caracteres++;

        // d) Contagem de letras 'a' e 'A'
        if (ch == 'a' || ch == 'A') {
            total_letras_a++;
        }

        // c) Contagem de linhas
        if (ch == '\n') {
            total_linhas++;
        }

        // b) Contagem de palavras
        if (isspace(ch)) {
            em_palavra = 0;
        } else if (!em_palavra) {
            em_palavra = 1;
            total_palavras++;
        }

        ultimo_caractere = ch;
    }

    // Trata arquivos que têm conteúdo mas não terminam com '\n'
    if (total_caracteres > 0 && ultimo_caractere != '\n') {
        total_linhas++;
    }

    fclose(arq);

    // Exibição dos resultados
    fprintf(arq, "Quantidade de caracteres : %d\n", total_caracteres);
    fprintf(arq, "Quantidade de palavras : %d\n", total_palavras);
    fprintf(arq, "Quantidade de linhas : %d\n", total_linhas);
    fprintf(arq, "Quantidade de letras A : %d\n", total_letras_a);

    return 0;
}