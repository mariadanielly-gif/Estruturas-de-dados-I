#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
} Produto;

// Função para limpar o buffer do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Protótipos das funções
void inicializarArquivoSeNaoExistir();
void listarProdutos();
void consultarProduto();
void alterarQuantidade();
void calcularValorTotal();

int main() {
    // Garante a criação do arquivo com os dados padrão caso não exista
    inicializarArquivoSeNaoExistir();

    int opcao;

    do {
        printf("\n=== SISTEMA DE CONTROLE DE ESTOQUE ===\n");
        printf("1 - Listar produtos\n");
        printf("2 - Consultar produto\n");
        printf("3 - Alterar quantidade\n");
        printf("4 - Calcular valor total do estoque\n");
        printf("5 - Sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("\nOpcao invalida! Digite apenas numeros.\n");
            limparBuffer();
            continue;
        }
        limparBuffer(); // Limpa o Enter restante do buffer

        switch (opcao) {
            case 1:
                listarProdutos();
                break;
            case 2:
                consultarProduto();
                break;
            case 3:
                alterarQuantidade();
                break;
            case 4:
                calcularValorTotal();
                break;
            case 5:
                printf("\nSaindo do sistema...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}

void inicializarArquivoSeNaoExistir() {
    FILE *arquivo = fopen("produtos.txt", "r");
    
    if (arquivo == NULL) {
        arquivo = fopen("produtos.txt", "w");
        if (arquivo != NULL) {
            fprintf(arquivo, "101 Teclado 10 85.50\n");
            fprintf(arquivo, "102 Mouse 15 35.90\n");
            fprintf(arquivo, "103 Monitor 5 899.90\n");
            fprintf(arquivo, "104 Headset 8 120.00\n");
            fclose(arquivo);
            printf("[Info] Arquivo 'produtos.txt' criado com sucesso com os dados iniciais.\n");
        } else {
            printf("[Erro] Nao foi possivel criar o arquivo produtos.txt!\n");
        }
    } else {
        fclose(arquivo);
    }
}

void listarProdutos() {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        printf("\nErro ao abrir o arquivo produtos.txt! Verifique se ele existe na mesma pasta.\n");
        return;
    }

    Produto p;
    printf("\n--- LISTA DE PRODUTOS ---\n");
    printf("%-8s %-20s %-12s %-10s\n", "Codigo", "Nome", "Quantidade", "Preco (R$)");
    printf("-----------------------------------------------------\n");

    while (fscanf(arquivo, "%d %s %d %f", &p.codigo, p.nome, &p.quantidade, &p.preco) == 4) {
        printf("%-8d %-20s %-12d R$ %-8.2f\n", p.codigo, p.nome, p.quantidade, p.preco);
    }

    fclose(arquivo);
}

void consultarProduto() {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        printf("\nErro ao abrir o arquivo produtos.txt!\n");
        return;
    }

    int codigoProcurado, encontrado = 0;
    Produto p;

    printf("\nDigite o codigo do produto: ");
    scanf("%d", &codigoProcurado);
    limparBuffer();

    while (fscanf(arquivo, "%d %s %d %f", &p.codigo, p.nome, &p.quantidade, &p.preco) == 4) {
        if (p.codigo == codigoProcurado) {
            printf("\n--- PRODUTO ENCONTRADO ---\n");
            printf("Codigo: %d\n", p.codigo);
            printf("Nome: %s\n", p.nome);
            printf("Quantidade: %d\n", p.quantidade);
            printf("Preco Unitario: R$ %.2f\n", p.preco);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\nProduto com codigo %d nao foi encontrado.\n", codigoProcurado);
    }

    fclose(arquivo);
}

void alterarQuantidade() {
    FILE *original = fopen("produtos.txt", "r");
    if (original == NULL) {
        printf("\nErro ao abrir o arquivo produtos.txt!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("\nErro ao criar o arquivo temporario!\n");
        fclose(original);
        return;
    }

    int codigoProcurado, novaQtd, encontrado = 0;
    Produto p;

    printf("\nDigite o codigo do produto que deseja alterar: ");
    scanf("%d", &codigoProcurado);
    limparBuffer();

    while (fscanf(original, "%d %s %d %f", &p.codigo, p.nome, &p.quantidade, &p.preco) == 4) {
        if (p.codigo == codigoProcurado) {
            encontrado = 1;
            printf("Produto encontrado: %s (Qtd atual: %d)\n", p.nome, p.quantidade);
            printf("Digite a nova quantidade: ");
            scanf("%d", &novaQtd);
            limparBuffer();
            p.quantidade = novaQtd;
        }
        fprintf(temp, "%d %s %d %.2f\n", p.codigo, p.nome, p.quantidade, p.preco);
    }

    fclose(original);
    fclose(temp);

    if (encontrado) {
        remove("produtos.txt");
        rename("temp.txt", "produtos.txt");
        printf("\nQuantidade atualizada com sucesso!\n");
    } else {
        remove("temp.txt");
        printf("\nProduto com codigo %d nao foi encontrado.\n", codigoProcurado);
    }
}

void calcularValorTotal() {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        printf("\nErro ao abrir o arquivo produtos.txt!\n");
        return;
    }

    Produto p;
    float valorTotalEstoque = 0.0;

    while (fscanf(arquivo, "%d %s %d %f", &p.codigo, p.nome, &p.quantidade, &p.preco) == 4) {
        valorTotalEstoque += p.quantidade * p.preco;
    }

    printf("\n=====================================\n");
    printf("Valor Total do Estoque: R$ %.2f\n", valorTotalEstoque);
    printf("=====================================\n");

    fclose(arquivo);
}