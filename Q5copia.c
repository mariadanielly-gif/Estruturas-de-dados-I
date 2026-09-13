/*
 * Vetor de Ponteiros para Struct com raylib
 * ---------------------------------------------------------------
 * Atividade final: reúne todos os conceitos das atividades
 * anteriores em um pequeno "sistema de entidades" (jogador,
 * inimigos e itens).
 *
 * A diferença central para a atividade4 é a forma como a coleção é
 * guardada: em vez de um vetor de struct (bloco contíguo), aqui
 * temos um VETOR DE PONTEIROS PARA STRUCT (Entidade *vetor[N]).
 * Cada posição do vetor guarda apenas um ENDEREÇO; a struct em si
 * fica em um bloco de memória alocado individualmente com malloc.
 * Isso permite, por exemplo, remover uma entidade "no meio" do
 * vetor apenas trocando ponteiros (rápido), sem precisar mover
 * structs inteiras na memória.
 *
 * Conceitos praticados (revisão de todas as atividades):
 *   - ponteiros e aritmética de ponteiros
 *   - alocação dinâmica (malloc/free) de cada struct individual
 *   - struct (Entidade) com campos variados
 *   - enum (TipoEntidade) para diferenciar jogador/inimigo/item
 *   - union (ExtraEntidade) para guardar, no mesmo espaço, o dano
 *     de um inimigo OU o valor de um item, conforme o TipoEntidade
 *   - ponteiro para struct (Entidade *) manipulado por funções
 *   - vetor de ponteiros para struct (Entidade *vetor[N])
 *
 * Compilar (Linux, com raylib instalada):
 *   gcc atividade5.c -o atividade5 -lraylib -lm -lpthread -ldl -lrt -lX11
 */

#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define LARGURA_JANELA  800
#define ALTURA_JANELA   600
#define RAIO_JOGADOR    20.0f
#define MAX_ENTIDADES   30
#define TOTAL_INIMIGOS  5
#define TOTAL_ITENS     6

typedef enum {
    ENTIDADE_JOGADOR,
    ENTIDADE_INIMIGO,
    ENTIDADE_ITEM
} TipoEntidade;

typedef union {
    int dano;   
    int valor;  
} ExtraEntidade;

typedef struct {
    TipoEntidade  tipo;
    Vector2       pos;
    float         raio;
    int           vida;
    Color         cor;
    ExtraEntidade extra;
} Entidade;

Entidade *vetorEntidades[MAX_ENTIDADES];
int totalEntidades = 0;

Entidade *criarEntidade(TipoEntidade tipo, Vector2 pos) {
    Entidade *e = (Entidade *)malloc(sizeof(Entidade));
    if (e == NULL) return NULL;

    e->tipo  = tipo;
    e->pos   = pos;
    e->raio  = (tipo == ENTIDADE_JOGADOR) ? RAIO_JOGADOR
             : (tipo == ENTIDADE_INIMIGO) ? 15.0f : 8.0f;

    switch (tipo) {
        case ENTIDADE_JOGADOR:
            e->vida = 100;
            e->cor  = BLUE;
            break;
        case ENTIDADE_INIMIGO:
            e->vida       = 40;
            e->cor        = MAROON;
            e->extra.dano = GetRandomValue(5, 15);
            break;
        case ENTIDADE_ITEM:
            e->vida        = 1;
            e->cor         = GOLD;
            e->extra.valor = GetRandomValue(5, 20);
            break;
    }
    return e;
}

void adicionarEntidade(Entidade *e) {
    if (e == NULL || totalEntidades >= MAX_ENTIDADES) return;
    vetorEntidades[totalEntidades] = e;
    totalEntidades++;
}

void removerEntidade(int indice) {
    if (indice < 0 || indice >= totalEntidades) return;

    free(vetorEntidades[indice]);
    vetorEntidades[indice] = vetorEntidades[totalEntidades - 1];
    vetorEntidades[totalEntidades - 1] = NULL;
    totalEntidades--;
}

bool colidiu(Entidade *a, Entidade *b) {
    if (a == NULL || b == NULL) return false;
    float dx = a->pos.x - b->pos.x;
    float dy = a->pos.y - b->pos.y;
    float distancia2 = dx * dx + dy * dy;
    float somaRaios2 = (a->raio + b->raio) * (a->raio + b->raio);
    return distancia2 <= somaRaios2;
}

void desenharEntidade(Entidade *e) {
    if (e == NULL) return;
    DrawCircleV(e->pos, e->raio, e->cor);
    if (e->tipo == ENTIDADE_INIMIGO) {
        DrawText(TextFormat("%d", e->vida), (int)(e->pos.x - 8), (int)(e->pos.y - 26), 14, BLACK);
    }
}

int main(void) {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Atividade 5 - Vetor de Ponteiros para Struct");
    SetTargetFPS(60);

    // Criação e validação do jogador (Índice 0)
    Entidade *jogador = criarEntidade(ENTIDADE_JOGADOR,
                                      (Vector2){ LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f });
    if (jogador == NULL) {
        CloseWindow();
        return 1;
    }
    adicionarEntidade(jogador);

    for (int i = 0; i < TOTAL_INIMIGOS; i++) {
        Vector2 pos = { (float)GetRandomValue(30, LARGURA_JANELA - 30), (float)GetRandomValue(30, ALTURA_JANELA - 30) };
        adicionarEntidade(criarEntidade(ENTIDADE_INIMIGO, pos));
    }

    for (int i = 0; i < TOTAL_ITENS; i++) {
        Vector2 pos = { (float)GetRandomValue(30, LARGURA_JANELA - 30), (float)GetRandomValue(30, ALTURA_JANELA - 30) };
        adicionarEntidade(criarEntidade(ENTIDADE_ITEM, pos));
    }

    int pontuacao = 0;

    while (!WindowShouldClose()) {

        float vel = 250.0f * GetFrameTime();
        if (IsKeyDown(KEY_RIGHT)) jogador->pos.x += vel;
        if (IsKeyDown(KEY_LEFT))  jogador->pos.x -= vel;
        if (IsKeyDown(KEY_UP))    jogador->pos.y -= vel;
        if (IsKeyDown(KEY_DOWN))  jogador->pos.y += vel;

        // Limita o movimento do jogador na tela
        if (jogador->pos.x - jogador->raio < 0) jogador->pos.x = jogador->raio;
        if (jogador->pos.x + jogador->raio > LARGURA_JANELA) jogador->pos.x = LARGURA_JANELA - jogador->raio;
        if (jogador->pos.y - jogador->raio < 0) jogador->pos.y = jogador->raio;
        if (jogador->pos.y + jogador->raio > ALTURA_JANELA) jogador->pos.y = ALTURA_JANELA - jogador->raio;

        // Colisões do jogador com demais entidades
        for (int i = 1; i < totalEntidades; i++) {
            Entidade *e = vetorEntidades[i];
            if (!colidiu(jogador, e)) continue;

            if (e->tipo == ENTIDADE_ITEM) {
                pontuacao += e->extra.valor;
                removerEntidade(i);
                i--; 
            } else if (e->tipo == ENTIDADE_INIMIGO) {
                jogador->vida -= e->extra.dano;
                if (jogador->vida < 0) jogador->vida = 0;
            }
        }

        // Disparo (ESPAÇO): atira no primeiro inimigo encontrado no vetor
        if (IsKeyPressed(KEY_SPACE)) {
            for (int i = 1; i < totalEntidades; i++) {
                Entidade *e = vetorEntidades[i];
                if (e->tipo == ENTIDADE_INIMIGO) {
                    e->vida -= 20;
                    if (e->vida <= 0) {
                        removerEntidade(i);
                    }
                    break;
                }
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            for (int i = 0; i < totalEntidades; i++) {
                desenharEntidade(vetorEntidades[i]);
            }

            DrawText(TextFormat("Vida: %d   Pontuacao: %d", jogador->vida, pontuacao), 10, 10, 22, DARKGRAY);
            DrawText(TextFormat("Entidades ativas: %d", totalEntidades), 10, 34, 18, GRAY);
            DrawText("Setas movem | ESPACO atira | ESC sai", 10, ALTURA_JANELA - 25, 16, GRAY);

        EndDrawing();
    }

    // Libera a memória alocada individualmente para cada entidade
    for (int i = 0; i < totalEntidades; i++) {
        if (vetorEntidades[i] != NULL) {
            free(vetorEntidades[i]);
            vetorEntidades[i] = NULL;
        }
    }

    CloseWindow();
    return 0;
}