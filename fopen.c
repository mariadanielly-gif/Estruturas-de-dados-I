#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // Função para abrir um arquivo 
    FILE * arq;
    char linha[100];
    arq = fopen("arquivo.txt", "w");
    if (arq == NULL){
        printf("Não e possivel criar o arquivo");
        exit(1);
    }
    else{
        printf("Arquivo criado\n");
    }
    //feof
    while(!feof(arq)){
        fscanf(arq, "%s", linha);
        printf("%s", linha);
    }
    fclose(arq);

    return 0;
}  