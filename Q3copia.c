#include "raylib.h"
#include <stdlib.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA  600
#define RAIO_JOGADOR   20.0f
#define TOTAL_ITENS    10

typedef enum {
    ITEM_ARMA,
    ITEM_POCAO
} TipoItem;

typedef struct {
    float dano;
    int   alcance;
} DadosArma;

typedef struct {
    int cura;
} DadosPocao;

/* union: em cada Item só um destes dois campos é válido por vez,
 * e isso é decidido pelo campo "tipo" (enum) da struct Item */
typedef union {
    DadosArma  arma;
    DadosPocao pocao;
} DadosItem;

typedef struct {
    Vector2   pos;
    float     raio;
    TipoItem  tipo;
    DadosItem dados;
    bool      coletado;
} Item;

typedef struct {
    Vector2 pos;
    float   raio;
    int     vida;
    float   dano;
} Jogador;

Item *criarItens(int quantidade) {
    Item *itens = (Item *)malloc(quantidade * sizeof(Item));
    if (itens == NULL) return NULL;

    for (int i = 0; i < quantidade; i++) {
        Item *it = (itens + i);
        it->pos      = (Vector2){ (float)GetRandomValue(30, LARGURA_JANELA - 30),
                                  (float)GetRandomValue(30, ALTURA_JANELA - 30) };
        it->raio     = 12.0f;
        it->coletado = false;
        it->tipo     = (TipoItem)GetRandomValue(ITEM_ARMA, ITEM_POCAO);

        if (it->tipo == ITEM_ARMA) {
            it->dados.arma.dano    = (float)GetRandomValue(2, 8);
            it->dados.arma.alcance = GetRandomValue(1, 3);
        } else {
            it->dados.pocao.cura = GetRandomValue(10, 30);
        }
    }
    return itens;
}

void aplicarItem(Jogador *j, Item *item) {
    switch (item->tipo) {
        case ITEM_ARMA:
            j->dano += item->dados.arma.dano;
            break;
        case ITEM_POCAO:
            j->vida += item->dados.pocao.cura;
            break;
    }
    item->coletado = true;
}

bool colidiu(Vector2 a, float raioA, Vector2 b, float raioB) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float distancia2 = dx * dx + dy * dy;
    float somaRaios2 = (raioA + raioB) * (raioA + raioB);
    return distancia2 <= somaRaios2;
}

void desenharItem(Item *item) {
    if (item->coletado) return;
    Color cor = (item->tipo == ITEM_ARMA) ? RED : GREEN;
    DrawCircleV(item->pos, item->raio, cor);
}

int main(void) {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Atividade 3 - Union + Enum + Struct");
    SetTargetFPS(60);

    Jogador jogador = { { LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f }, RAIO_JOGADOR, 100, 5.0f };
    Item *itens = criarItens(TOTAL_ITENS);

    // Validação de segurança para alocação do vetor dinâmico
    if (itens == NULL) {
        CloseWindow();
        return 1;
    }

    while (!WindowShouldClose()) {

        float vel = 250.0f * GetFrameTime();
        if (IsKeyDown(KEY_RIGHT)) jogador.pos.x += vel;
        if (IsKeyDown(KEY_LEFT))  jogador.pos.x -= vel;
        if (IsKeyDown(KEY_UP))    jogador.pos.y -= vel;
        if (IsKeyDown(KEY_DOWN))  jogador.pos.y += vel;

        // Mantém o jogador dentro dos limites da tela
        if (jogador.pos.x - jogador.raio < 0) jogador.pos.x = jogador.raio;
        if (jogador.pos.x + jogador.raio > LARGURA_JANELA) jogador.pos.x = LARGURA_JANELA - jogador.raio;
        if (jogador.pos.y - jogador.raio < 0) jogador.pos.y = jogador.raio;
        if (jogador.pos.y + jogador.raio > ALTURA_JANELA) jogador.pos.y = ALTURA_JANELA - jogador.raio;

        for (int i = 0; i < TOTAL_ITENS; i++) {
            Item *it = (itens + i);
            if (!it->coletado && colidiu(jogador.pos, jogador.raio, it->pos, it->raio)) {
                aplicarItem(&jogador, it);
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            for (int i = 0; i < TOTAL_ITENS; i++) {
                desenharItem(itens + i);
            }

            DrawCircleV(jogador.pos, jogador.raio, BLUE);

            DrawText(TextFormat("Vida: %d   Dano: %.1f", jogador.vida, jogador.dano), 10, 10, 22, DARKGRAY);
            DrawText("Vermelho = arma (aumenta dano) | Verde = pocao (cura)", 10, 34, 18, GRAY);
            DrawText("Setas movem o jogador | ESC sai", 10, ALTURA_JANELA - 25, 16, GRAY);

        EndDrawing();
    }

    free(itens);
    CloseWindow();
    return 0;
}