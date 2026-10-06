#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nome[50];
    float preco;

} Fruta;

void preenche(Fruta * f) {
    char opcao;

    FILE * arq = fopen("fruta.txt", "w");
        if (arq == NULL) { exit(1);
        }
    
    do{
        printf("Digite o nome da fruta: ");
        scanf(" %[^\n]", f->nome);

        printf("Digite o preco da fruta: ");
        scanf("%f", &f->preco);
        
        fprintf(arq, "%s\t%.2f\n", f->nome, f->preco);

        printf("Deseja cadastrar outra fruta? (s/n): ");
        scanf(" %c", &opcao);
        
    } while(opcao == 's' || opcao == 'S');


    fclose(arq);
    printf("Cadastro encerrado e dados salvos com sucesso!\n");
}
int main(void) {
    Fruta f;
    preenche(&f);
    return 0;
}