#include <stdio.h>
#include <stdlib.h>

int main(void){
	int linha=3, coluna=3, i, j; 
	int ** matriz = (int **) malloc(linha* sizeof(int*));	
for (i = 0; i < linha; i++) {
	matriz[i] = (int *)malloc(coluna* sizeof(int));
}
for (i = 0; i < linha; i++) {
	for(int j=0; j<coluna; j++);{
	matriz[i][j] = i*3 + j;
	printf("%d \t", matriz[i][j]);
}
printf("\n");
}
	return 0;
	}