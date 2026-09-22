#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nome[50];
    float preco;
    int N;
} Fruta;

    FILE * arq = fopen("fruta.txt", "w");
        if (arq == NULL) { exit(1);};


    for (int i = 0; i < N; i++) {
    printf("Digite o nome da fruta: ");
    scanf(" %[^\n]", nome);
    printf("Digite o preco da fruta: ");
    scanf("%f", &preco);
    

    fprintf(arq, "%s\t%.2f", nome, preco);
}
fclose(arq);

int main(void) {
    Fruta f;
}

 preenche(&f);
    printf("\nNome: %s\n", f.nome);
  
    
    return 0;
}